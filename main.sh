#!usr/bin/bash

# config the environment
if ! [ -x "$(command -v pip)" ]; then
sudo apt install python-pip
sleep 1
pip install numpy
pip install PyYAML
sleep 1
fi

if ! [ -x "$(command -v gnome-terminal)" ]; then
  sudo apt-get update
  sudo apt-get install gnome-terminal
  sleep 1
fi

# make sure we run the scripts in /sim folder
if ! [ ${PWD##*/} = "sim" ];then
   mkdir ../sim
   cp -r . ../sim
   cd ../sim
fi

# initialize log files
if [ -f detectResult.log ]; then
    rm -f detectResult.log
fi

# add object
python spawnObjs.py
sleep 1

# terminal 1: laser server
gnome-terminal -t "laser server" -x bash -c "cd /home/smr/sim/bash_src/;
sh laser.sh;exec bash;"
sleep 1

# terminal 2: run the simulator
gnome-terminal -t "simulator" -x bash -c "cd /home/smr/sim/bash_src/;
sh sim.sh;exec bash;"
sleep 1

# terminal 3: run the client
gnome-terminal -t "client" -x bash -c "cd /home/smr/sim/bash_src/;
sh client.sh;exec bash;"
sleep 1

# terminal 4: run mrc scripts
gnome-terminal -t "mrc" -x bash -c "cd /home/smr/sim/bash_src/;
sh mov.sh;exec bash;"
sleep 1

# terminate
## show processes
echo "processes"
ps -ef | grep "bash" | grep -v grep | awk '{print $2}'
ps
while true;do
echo "Enter 'e' to exit"
read flag
if [ ! -n "$flag" ]
then
    echo "Invalid input"
else
    if [ $flag = 'e' ]
    then
	## clean logg files
        rm -r *.logg
	## restore config files
        if [ -f 388project_temp ]; then
	  rm -f 388project
          mv 388project_temp 388project
	fi
	sleep 1
        cd mrc_src
	if [ -f createWalls_temp ];then
          rm -f createWalls
          mv createWalls_temp createWalls
        fi
	cd ..
	mv /*.log /logs/.
        ## kill all terminal processes
	gnome-terminal -t "close client" -x killall -9 qclient
        gnome-terminal -t "clear all" -x killall -9 bash  
        break
    fi
fi
done
exit 0

