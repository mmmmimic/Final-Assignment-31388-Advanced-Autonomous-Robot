#!/usr/bin/bash
cd ~/home/smr/mobotware/aurs-plugins/auzoneobst/
make clean
make
cp objectDetect.so.0 /home/smr/sim/
