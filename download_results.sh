#!/bin/bash

mkdir -p downloadedResults/results
mkdir -p autogenerated/large/imagesFromScripts
mkdir -p autogenerated/small/imagesFromScripts

rm -f bingyDingBang.csv

sudo scp -r -i /home/thaylo/Desktop/tfreitas.pem ubuntu@54.94.153.231:/tmp/results ./downloadedResults
sudo chmod 777 ./downloadedResults/* -R

mkdir -p autogenerated/small/
mkdir -p autogenerated/large/

cat downloadedResults/results/*/*csv > bingyDingBang.csv

cp downloadedResults/results/*/autogenerated/small/*.m autogenerated/small/
#cp downloadedResults/results/*/autogenerated/large/*.m autogenerated/large/

sudo chmod 777 ./autogenerated/ -R

rm -rf downloadedResults
