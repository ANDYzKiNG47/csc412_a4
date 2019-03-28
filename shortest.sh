#!/bin/bash
g++ -g -Wall main.cpp grid.cpp node.cpp -o shortest
# create grid file and output demensions
# TODO- only run this code if script has to generate data
# if data must be generated
# $1 == rows  $2 == cols  $3 == num_nodes  $4 == node1_idx  $5 == node2_idx   $6 == out_path
# generate grid
touch grids/rand_grid.txt
g++ -g -Wall gen_grid.cpp -o gen_grid
./gen_grid $1 $2 grids/rand_grid.txt
# generate nodes
touch nodes/rand_nodes.txt
g++ -g -Wall gen_nodes.cpp -o gen_nodes
./gen_nodes $3 $1 $2 nodes/rand_nodes.txt
# run shortest program with parameters generated
# argv[1] == path to grid file
# argv[2] == path to file that stores list of nodes
# argv[3] == index of first node
# argv[4] == index of second node
# argv[5] == path to output directory
./shortest /home/az47/CSC412/a4/grids/rand_grid.txt /home/az47/CSC412/a4/nodes/rand_nodes.txt $4 $5 $6
# endif
