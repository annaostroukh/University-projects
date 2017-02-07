#!/bin/bash

#pocet cisel bud zadan nebo 8, pocet procesoru bud zadan nebo 15
if [ $# -lt 1 ];then 
    pocet_hodnot=8;
    pocet_procesoru=15;
    
else
    pocet_hodnot=$1;
    pocet_procesoru=$2;
fi;

#preklad cpp zdrojaku
mpic++ --prefix /usr/local/share/OpenMPI -o mes mes.cpp


#vyrobeni souboru s random cisly
dd if=/dev/urandom bs=1 count=$pocet_hodnot of=numbers &> /dev/null

#spusteni
mpirun --quiet --prefix /usr/local/share/OpenMPI -np $pocet_procesoru mes 

#uklid
rm -f mes numbers