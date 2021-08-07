#!/usr/bin/env python3
#
# worktipsnet runtime wrapper
#

from ctypes import *
import configparser
import signal
import time
import threading
import os
import sys
import requests

from pyworktipsnet import rc

lib_file = os.path.join(os.path.realpath('.'), 'libworktipsnet-shared.so')


def log(msg):
    sys.stderr.write("worktipsnet: {}\n".format(msg))
    sys.stderr.flush()


class WorktipsNET(threading.Thread):

    lib = None
    ctx = 0
    failed = False
    up = False

    asRouter = True

    def configure(self, lib, conf, ip=None, port=None, ifname=None, seedfile=None, worktipsd_host=None, worktipsd_port=None):
        log("configure lib={} conf={}".format(lib, conf))
        if not os.path.exists(os.path.dirname(conf)):
            os.mkdir(os.path.dirname(conf))
        try:
            self.lib = CDLL(lib)
        except OSError as ex:
            log("failed to load library: {}".format(ex))
            return False
        if self.lib.llarp_ensure_config(conf.encode('utf-8'), os.path.dirname(conf).encode('utf-8'), True, self.asRouter):
            config = configparser.ConfigParser()
            config.read(conf)
            log('overwrite ip="{}" port="{}" ifname="{}" seedfile="{}" worktipsd=("{}", "{}")'.format(
                ip, port, ifname, seedfile, worktipsd_host, worktipsd_port))
            if seedfile and worktipsd_host and worktipsd_port:
                if not os.path.exists(seedfile):
                    log('cannot access service node seed at "{}"'.format(seedfile))
                    return False
                config['worktipsd'] = {
                    'service-node-seed': seedfile,
                    'enabled': "true",
                    'jsonrpc': "{}:{}".format(worktipsd_host, worktipsd_port)
                }
            if ip:
                config['router']['public-address'] = '{}'.format(ip)
            if port:
                config['router']['public-port'] = '{}'.format(port)
            if ifname and port:
                config['bind'] = {
                    ifname: '{}'.format(port)
                }
            with open(conf, "w") as f:
                config.write(f)
            self.ctx = self.lib.llarp_main_init(conf.encode('utf-8'))
        else:
            return False
        return self.lib.llarp_main_setup(self.ctx, False) == 0

    def inform_fail(self):
        """
        inform worktipsnet crashed
        """
        self.failed = True
        self._inform()

    def inform_up(self):
        self.up = True
        self._inform()

    def _inform(self):
        """
        inform waiter
        """

    def wait_for_up(self, timeout):
        """
        wait for worktipsnet to go up for :timeout: seconds
        :return True if we are up and running otherwise False:
        """
        # return self._up.wait(timeout)

    def signal(self, sig):
        if self.ctx and self.lib:
            self.lib.llarp_main_signal(self.ctx, int(sig))

    def run(self):
        # self._up.acquire()
        self.up = True
        code = self.lib.llarp_main_run(self.ctx)
        log("llarp_main_run exited with status {}".format(code))
        if code:
            self.inform_fail()
        self.up = False
        # self._up.release()

    def close(self):
        if self.lib and self.ctx:
            self.lib.llarp_main_free(self.ctx)


def getconf(name, fallback=None):
    return name in os.environ and os.environ[name] or fallback


def run_main(args):
    seedfile = getconf("WORKTIPS_SEED_FILE")
    if seedfile is None:
        print("WORKTIPS_SEED_FILE was not set")
        return

    worktipsd_host = getconf("WORKTIPS_RPC_HOST", "127.0.0.1")
    worktipsd_port = getconf("WORKTIPS_RPC_PORT", "22023")

    root = getconf("WORKTIPSNET_ROOT")
    if root is None:
        print("WORKTIPSNET_ROOT was not set")
        return

    rc_callback = getconf("WORKTIPSNET_SUBMIT_URL")
    if rc_callback is None:
        print("WORKTIPSNET_SUBMIT_URL was not set")
        return

    bootstrap = getconf("WORKTIPSNET_BOOTSTRAP_URL")
    if bootstrap is None:
        print("WORKTIPSNET_BOOTSTRAP_URL was not set")

    lib = getconf("WORKTIPSNET_LIB", lib_file)
    if not os.path.exists(lib):
        lib = "libworktipsnet-shared.so"
    timeout = int(getconf("WORKTIPSNET_TIMEOUT", "5"))
    ping_interval = int(getconf("WORKTIPSNET_PING_INTERVAL", "60"))
    ping_callback = getconf("WORKTIPSNET_PING_URL")
    ip = getconf("WORKTIPSNET_IP")
    port = getconf("WORKTIPSNET_PORT")
    ifname = getconf("WORKTIPSNET_IFNAME")
    if ping_callback is None:
        print("WORKTIPSNET_PING_URL was not set")
        return
    conf = os.path.join(root, "daemon.ini")
    log("going up")
    worktips = WorktipsNET()
    log("bootstrapping...")
    try:
        r = requests.get(bootstrap)
        if r.status_code == 404:
            log("bootstrap gave no RCs, we are probably the seed node")
        elif r.status_code != 200:
            raise Exception("http {}".format(r.status_code))
        else:
            data = r.content
            if rc.validate(data):
                log("valid RC obtained")
                with open(os.path.join(root, "bootstrap.signed"), "wb") as f:
                    f.write(data)
            else:
                raise Exception("invalid RC")
    except Exception as ex:
        log("failed to bootstrap: {}".format(ex))
        worktips.close()
        return
    if worktips.configure(lib, conf, ip, port, ifname, seedfile, worktipsd_host, worktipsd_port):
        log("configured")

        worktips.start()
        try:
            log("waiting for spawn")
            while timeout > 0:
                time.sleep(1)
                if worktips.failed:
                    log("failed")
                    break
                log("waiting {}".format(timeout))
                timeout -= 1
            if worktips.up:
                log("submitting rc")
                try:
                    with open(os.path.join(root, 'self.signed'), 'rb') as f:
                        r = requests.put(rc_callback, data=f.read(), headers={
                                         "content-type": "application/octect-stream"})
                        log('submit rc reply: HTTP {}'.format(r.status_code))
                except Exception as ex:
                    log("failed to submit rc: {}".format(ex))
                    worktips.signal(signal.SIGINT)
                    time.sleep(2)
                else:
                    while worktips.up:
                        time.sleep(ping_interval)
                        try:
                            r = requests.get(ping_callback)
                            log("ping reply: HTTP {}".format(r.status_code))
                        except Exception as ex:
                            log("failed to submit ping: {}".format(ex))
            else:
                log("failed to go up")
                worktips.signal(signal.SIGINT)
        except KeyboardInterrupt:
            worktips.signal(signal.SIGINT)
            time.sleep(2)
        finally:
            worktips.close()
    else:
        worktips.close()


def main():
    run_main(sys.argv[1:])


if __name__ == "__main__":
    main()
