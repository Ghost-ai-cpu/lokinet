mkdir worktips$1
cd worktips$1
ln -s ../worktipsnet worktipsnet$1
cp ../worktipsnet.ini .
nano worktipsnet.ini
cd ..
echo "killall -9 worktipsnet$1" >> ../stop.sh
