#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

#include "grid.h"
#include "node.h"

/*
  * argv[1] == path to grid file
  * argv[2] == path to file that stores list of nodes
  * argv[3] == index of first node
  * argv[4] == index of second node
  * argv[5] == path to output directory
*/
using namespace std;
int main( int argc, char** argv ){

  // read in command line arguments
  string grid_path = argv[1];
  string node_list_path = argv[2];
  int start_node_idx = atoi( argv[3] );
  int end_node_idx = atoi( argv[4] );
  string out_dir = argv[5];
  if ( out_dir.back() != '/' ){
    out_dir.append("/");
  }

  // create instance of Grid class
  Grid grid = Grid( grid_path, node_list_path );
  grid.set_start_end( start_node_idx, end_node_idx );
  grid.print();

  return 0;
}
