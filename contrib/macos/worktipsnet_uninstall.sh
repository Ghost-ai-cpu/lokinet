#!/bin/sh
launchctl stop network.worktips.worktipsnet.daemon
launchctl unload /Library/LaunchDaemons/network.worktips.worktipsnet.daemon.plist

killall worktipsnet

rm -rf /Library/LaunchDaemons/network.worktips.worktipsnet.daemon.plist
rm -rf /Applications/WorktipsnetGUI.app
rm -rf /var/lib/worktipsnet
rm -rf /usr/local/worktipsnet/
rm -rf /opt/worktipsnet

