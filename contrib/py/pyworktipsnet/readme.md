# pyworktipsnet

worktipsnet with python 3

    # python3 setup.py install

## bootserv

bootserv is a bootstrap server for accepting and serving RCs

    $ gunicorn -b 0.0.0.0:8000 pyworktipsnet.bootserv:app

## pyworktipsnet instance

obtain `libworktipsnet-shared.so` from a worktipsnet build

run (root):
    
    # export WORKTIPSNET_ROOT=/tmp/worktipsnet-instance/
    # export WORKTIPSNET_LIB=/path/to/libworktipsnet-shared.so
    # export WORKTIPSNET_BOOTSTRAP_URL=http://bootserv.ip.address.here:8000/bootstrap.signed
    # export WORKTIPSNET_PING_URL=http://bootserv.ip.address.here:8000/ping
    # export WORKTIPSNET_SUBMIT_URL=http://bootserv.ip.address.here:8000/
    # export WORKTIPSNET_IP=public.ip.goes.here
    # export WORKTIPSNET_PORT=1090
    # export WORKTIPSNET_IFNAME=eth0
    # python3 -m pyworktipsnet