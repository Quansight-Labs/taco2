#!/usr/bin/env bash

set -eu

# First create new repo on GitHub and copy the SSH repo url
# Then run "./scripts/liftoff.sh" from within your local hpp-skel root directory
# and it will create your new local project repo side by side with hpp-skel directory

echo "What is the name of your new project? "
read name
echo "What is the remote repo url for your new project? "
read url

mkdir ../$name
cp -R ../hpp-skel/. ../$name/
cd ../$name/
rm -rf .git
git init

git checkout -b hpp-skel-port
git add .
git commit -m "Port from hpp-skel"
git remote add origin $url
git push -u origin hpp-skel-port

# Perhaps useful for fresh start
# rm -rf include/taco2
# rm -rf include/taco2.hpp
# cp /dev/null CHANGELOG.md
# cp /dev/null README.md