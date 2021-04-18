#!/bin/bash
flag=0

if [ "$1" = "-d" ] || [ "$1" = "--debug" ]; then
    g++ -DDEBUG -c Message.cpp prueba.cpp #Compile
else
    g++ -c Message.cpp RadioDevice.cpp RadioPiRouter.cpp prueba.cpp #Compile
fi

if [ $? -ne 0 ]; then
    flag=1
fi

g++ -lpigpio -lwiringPi -o main.out Message.o RadioDevice.o RadioPiRouter.o prueba.o #Link
if [ $? -ne 0 ]; then
    flag=1
fi

if [ ${flag} -eq 0 ]; then 
    sudo ./main.out  #Execute
else
    echo "Error At compilation time"
fi
