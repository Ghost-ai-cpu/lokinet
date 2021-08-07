#!/bin/bash
set +x
cd worktips1
nohup ./worktipsnet1 $PWD/worktipsnet.ini &
# seed node needs some time to write RC to make sure it's not expired on load for the rest
sleep 1
cd ../worktips2
nohup ./worktipsnet2 $PWD/worktipsnet.ini &
cd ../worktips3
nohup ./worktipsnet3 $PWD/worktipsnet.ini &
cd ../worktips4
nohup ./worktipsnet4 $PWD/worktipsnet.ini &
cd ../worktips5
nohup ./worktipsnet5 $PWD/worktipsnet.ini &
cd ..
tail -f worktips*/nohup.out
