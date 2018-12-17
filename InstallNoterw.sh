#!/bin/bash

cd /usr/lib
echo "creating 'Noterw' directory at /usr/lib"
sudo mkdir Noterw
cd Noterw
echo "creating 'NoterwFilePath' text file"
sudo touch NoterwFilePath
sudo chmod -R ugo+rw /usr/lib/Noterw/NoterwFilePath
echo "/home" >> NoterwFilePath
echo "PROCESS COMPLETE"
