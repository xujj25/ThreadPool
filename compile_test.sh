#!/bin/bash

g++ -std=c++11 ./example/test.cpp -o ./bin/test -I ./include/ -L ./lib/ -lpthread -lthreadpool
