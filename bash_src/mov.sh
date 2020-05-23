#!usr/bin/bash
# run mrc scripts
cd ../mrc_src
## create map
mrc -s8000 createWalls
mrc -s8000 spawnNodes
wait 1
## run the robot
mrc -s8000 main
