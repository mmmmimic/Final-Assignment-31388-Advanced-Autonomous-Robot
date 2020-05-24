#!usr/bin/bash
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
## print processes
echo "processes"
ps -ef | grep "bash" | grep -v grep | awk '{print $2}'
ps
while true;do
echo "Enter 'e' to exit"
read flag
echo $flag
    if [ ! -n "$flag" ]
    then
        echo "Invalid input"
    else
        if [ $flag=='e' ]
        then
	  ## clean logg files
          rm -r *.logg
          ## kill all terminal processes
	  gnome-terminal -t "close client" -x killall -9 qclient
          gnome-terminal -t "clear all" -x killall -9 bash  
          break
        fi
    fi
done
exit 0

