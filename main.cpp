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
// TODO: CHILD & GRANDCHILD PROCESSES

static int* all_paths_idx;

using namespace std;
int main( int argc, char** argv ){

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
    *   forking    *
  *\                */
  // init memory map
  all_paths_idx = (int*) mmap(NULL, sizeof *all_paths_idx, PROT_READ | PROT_WRITE,
                MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  pid_t parent_pid, child_pid;
  int status = 0;

  // create children for each fork
  for( int i = 0; i < grid.get_num_paths(); ++i ){
    // if child process
    if( ( child_pid = fork() ) == 0 ){
      int temp = *all_paths_idx;
      // increment the global variable for next process
      *all_paths_idx += 1;
      // get the node path assigned to process
      vector<int> child_node_path = grid.get_path( temp );
      for( auto i = child_node_path.begin(); i != child_node_path.end(); i++ ) printf( " %d", *i );
      cout << endl;

      exit(0);
    }
  }
  while (( parent_pid = wait(&status) ) > 0);
  printf("%d\n", *all_paths_idx);
  munmap(all_paths_idx, sizeof *all_paths_idx);

  return 0;
}
