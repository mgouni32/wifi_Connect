#!/bin/bash

#move cal file
mkdir ./cal; mv localcal.txt ./cal/localcal.txt; chmod 775 ./cal/localcal.txt

#move certs to persistent storage
#mv /wiringPi/certs /data/certs

#working before 11-17-16
#pm2 start /usr/local/bin/node-red --node-args="--max-old-space-size=128" -- -v
#end working code

#run LED control
./ledstatus.o &

#run C program
./pub.o &
