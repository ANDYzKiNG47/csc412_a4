// C++ libs
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

// C libs
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

// local files
#include "grid.h"
#include "node.h"

/*
  * argv[1] == path to grid file
  * argv[2] == path to file that stores list of nodes
  * argv[3] == index of first node
  * argv[4] == index of second node
  * argv[5] == path to output directory
*/
// TODO: FIND DISTANCE


using namespace std;
int main( int argc, char** argv ){
  /*                *\
    *     INIT     *
  *\                */
  // read in command line arguments
  string grid_path = argv[1];
  //string grid_path = "/home/az47/CSC412/a4/grids/rand_grid.txt";
  string node_list_path = argv[2];
  //string node_list_path = "/home/az47/CSC412/a4/nodes/rand_nodes.txt";
  int start_node_idx = atoi( argv[3] );
  int end_node_idx = atoi( argv[4] );
  string out_dir = argv[5];
  if ( out_dir.back() != '/' ){
    out_dir.append("/");
  }

  // create instance of Grid class
  Grid grid = Grid( grid_path, node_list_path );
  // set the start and end node of the path
  grid.set_start_end( start_node_idx, end_node_idx );
  // find all paths from the start node to the end node
  // visiting at least 3, but no more than 5 nodes
  grid.find_all_paths();
  // print all grid information
  grid.print();

  /*                *\
    *   FORKING    *
  *\                */
  static int* path_num;
  static int* path_dists;
  path_num = (int*) mmap(NULL, sizeof *path_num, PROT_READ | PROT_WRITE,
                MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  *path_num = grid.get_num_paths();
  //init output array for grandchildren
  path_dists = (int*) mmap(NULL, sizeof(int)*grid.get_num_paths(), PROT_READ | PROT_WRITE,
                              MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  for( int i = 0; i < grid.get_num_paths(); i++ ){
    path_dists[i] = 0;
  }
  // create child
  for( int i = 0; i < *path_num; i++ ){
    if( fork() == 0 ){
      // int number of grandchildren to create
      static int* path_len = (int*) mmap(NULL, sizeof *path_len, PROT_READ | PROT_WRITE,
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
      *path_len = grid.get_path( i ).size();
      // global string to share path string
      printf("[son] pid %d from [par] pid %d\n",getpid(),getppid());
      for( int j = 0; j < *(path_len) - 1; j++ ){
        // create grandchild
        if( fork() == 0 ){
          int* test = grid.get_pair(i,j);
          cout<<getppid()<<" : "<< test[0]<<" "<<test[1]<<endl <<flush;
          grid.del_pair(test);
          printf("[grc] pid %d from [son] pid %d\n",getpid(),getppid());
          //find_dist
          path_dists[i] += 1;
          exit(0);
        }
      }
      // wait for all grandchildren to exit
      for( int i = 0; i < *path_num; i++ ){
        wait(NULL);
      }
      munmap(path_len, sizeof *path_len);
      exit(0);
    }
  }
  // wait untill all children exit
  for( int i = 0; i < *path_num; i++ ){
    wait(NULL);
  }
  for( int i = 0; i < grid.get_num_paths(); i++ ){
    cout<<path_dists[i]<<endl;
  }
  //find shortest value in dist
  munmap(path_num, sizeof *path_num);
  munmap(path_dists, sizeof *path_dists );
} // end main
