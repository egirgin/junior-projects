#!/bin/bash

mpic++ game.cpp -o game

mpirun -np 145 --oversubscribe ./game input.txt output.txt 20
