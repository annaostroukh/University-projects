#!/bin/bash

#pocet cisel bud zadan nebo 8, pocet procesoru bud zadan nebo 15

if [ $# -lt 1 ];then 				
    numbers=`wc -c numbers | awk '{ print ($1-3) }'`
else
    numbers=$1;
    numbers=$(($numbers*2));
    numbers=$(($numbers-1));
fi;

#preklad cpp zdrojaku
mpic++ --prefix /usr/local/share/OpenMPI -o clapba clapba.cpp


#vyrobeni souboru s random cisly
#dd if=/dev/urandom bs=1 count=$pocet_hodnot of=numbers &> /dev/null

#spusteni
mpirun --quiet --prefix /usr/local/share/OpenMPI -np $numbers clapba

#uklid
rm -f clapba >/dev/null