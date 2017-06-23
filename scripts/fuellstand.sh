#!/bin/bash

ETHERSEX_IP=192.168.0.15
ETHERSEX_PORT=2701
LOGFILE=/var/log/fuellstand.sh.log

SENSOR=$1

#echo $SENSOR

let VAL1=`echo '!i2c rbd '$SENSOR' 0' | nc -q1 $ETHERSEX_IP $ETHERSEX_PORT`
echo $VAL1

#VAL2=$($VCLIENT -h $HOST -c $1 | sed -n 's/.*\([[:digit:]]\+[\.][[:digit:]]*\).*/\1/p')
#echo $VAL2
#| sed -n '/[[:digit:]]\+\.[[:digit:]]*/p' 
# | sed -e '1d'
#ERROR=`echo -n $VAL1 | grep Fehler` 

#echo printf start
#printf "%d" 0x$VAL1
#echo printf stop


#echo $ERROR
echo -n `date` >> $LOGFILE
echo -n " " >> $LOGFILE
echo $VAL1 >> $LOGFILE


#if [ $ERROR ];then
#    echo -n ""
#else
#    echo $VAL1
#fi

