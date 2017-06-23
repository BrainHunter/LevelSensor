#!/bin/bash

ETHERSEX_IP=192.168.0.15
ETHERSEX_PORT=2701
LOGFILE=/var/log/fuellstand-raw.sh.log

SENSOR_ID=$1



#VAL1=`echo '!i2c rwd '$SENSOR_ID' 2' | nc -q1 $ETHERSEX_IP $ETHERSEX_PORT | sed -n 's/0x\([[:xdigit:]][[:xdigit:]]\)\([[:xdigit:]][[:xdigit:]]\)/\2\1/p' | awk '{print "ibase=16;"$0}'| bc`
VAL1=`echo '!i2c rwd '$SENSOR_ID' 2' | nc -q1 $ETHERSEX_IP $ETHERSEX_PORT`
VAL2=`echo $VAL1 | awk '{ $num = strtonum($0); $erg = rshift(and($num, 0xFF00),8)+lshift(and($num, 0xFF),8); print $erg}'`

#echo $VAL1
echo $VAL2

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
echo -n $SENSOR_ID >> $LOGFILE
echo -n " " >> $LOGFILE
echo -n $VAL1 >> $LOGFILE
echo -n " " >> $LOGFILE
echo $VAL2 >> $LOGFILE


#if [ $ERROR ];then
#    echo -n ""
#else
#    echo $VAL1
#fi

