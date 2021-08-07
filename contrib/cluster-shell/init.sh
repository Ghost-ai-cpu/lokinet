#!/bin/sh
# copy a worktipsnet binary into this cluster
cp ../../worktipsnet .
# generate default config file
./worktipsnet -g -r worktipsnet.ini
# make seed node
./makenode.sh 1
# establish bootstrap
ln -s worktips1/self.signed bootstrap.signed
