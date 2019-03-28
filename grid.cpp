#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>

#include "node.h"
#include "grid.h"

using namespace std;

int manhattan_dist( Node* start_node, Node* end_node );
int find_max_idx( int* array );

// default constructor
Grid::Grid(){
  this->grid = NULL;
  this->num_rows = 0;
  this->num_cols = 0;
}
// full constructor
Grid::Grid( string grid_path, string node_list_path ){
  // init grid, num_cols, num_rows
  this -> read_grid( grid_path );
  // init nodes, num_nodes
  this -> read_nodes( node_list_path );
}

//destructor
Grid::~Grid(){
  // delete grid
  for ( int i = 0; i < this->num_rows; i++ ){
    delete [] this -> grid[i];
  }
  delete [] this -> grid;

  // delete nodes
  for ( int i = 0; i < this->num_nodes; i++ ){
    delete this -> nodes[i];
  }
  delete [] this -> nodes;
}

// method that generates a 2D array given a text file and returns a pointer to the array
void Grid::read_grid( string grid_path ){
  // open file storing grid
  ifstream infile( grid_path );
  // read in dimensions of grid
  int num_rows, num_cols;
  infile >> num_rows >> num_cols;
  // define private variables
  this->num_rows = num_rows;
  this->num_cols = num_cols;
  // delcare and allocate memory for the grid
  this -> grid = new float*[num_rows];
  for(int i = 0; i < num_rows; ++i){
    this -> grid[i] = new float[num_cols];
  }
  // read data from file into the grid
  for ( int i = 0; i< num_rows; i++ )
    for( int j = 0; j < num_cols; j++ )
      infile >> this -> grid[i][j];
}

// method creates nodes and stores them in nodes array
void Grid::read_nodes( string node_list_path ){
  ifstream infile( node_list_path );
  int num_nodes;
  infile >> num_nodes;
  this -> nodes = new Node*[num_nodes];
  this -> num_nodes = num_nodes;
  for ( int i = 0; i < num_nodes; i++ ){
    int x,y;
    infile >> x >> y;
    this -> nodes[i] = new Node();
    this -> nodes[i]->set_x( x );
    this -> nodes[i]->set_y( y );
  }
  this->find_neigh();
}

// method that finds each node's 3 nearest neighbors using Manhattan distance
// and records the index of those neighbors in the neigh_idx private array
void Grid::find_neigh(){
  // loop through all nodes
  for( int i = 0; i < this->num_nodes; i++ ){
    // array to store Manhattan distances of all other nodes in respect to node at index i
    int distances[this->num_nodes];
    // loop to compute Manhattan distance of all other nodes
    for( int j = 0; j < this->num_nodes; j++ ){
      // if to ensure node does not compute Manhattan distance of itself
      if( i != j ){
        distances[j] = manhattan_dist( this->nodes[i], this->nodes[j] );
      } else{
        // set the distance of the node being tested equal to the maximum int value so it can never have the lowest score
        distances[j] = INT8_MAX;
      }
    }
    // find 3 lowest Manhattan distances in the array and store the index of the score in private nodes array
    int min[3] = { distances[0], distances[1], distances[2] };
    int max_min_idx = find_max_idx( min );
    for( int j = 3; j < this->num_nodes; j++ ){
      if( distances[j] < min[max_min_idx] ){
        min[max_min_idx] = j;
        max_min_idx = find_max_idx( min );
      }
    }
    // copy indecies of closest nodes into neigh_idx array, in the node object
    this->nodes[i]->set_neigh( min );
  }
}

void Grid::set_start_end( int start_idx, int end_idx ){
  for( int i = 0; i < this->num_nodes; i++ ){
    // set flag if node is start or end point
    if ( i == start_idx ) this->nodes[i]->set_start_or_end(1);
    if ( i == end_idx ) this->nodes[i]->set_start_or_end(2);
  }
}

// that that prints the 2D array
void Grid::print_grid(){
  cout<<endl;
  for( int i = 0; i < this->num_rows; i++ ){
    for( int j = 0; j < this->num_cols; j++ ){
      cout << this->grid[i][j] << " ";
    }
    cout << endl;
  }
  cout << endl;
  cout << "num_rows: " << this->num_rows << endl;
  cout << "num_cols: " << this->num_cols << endl;
}

// method that prints all nodes coordinates TODO: MAYBE PRINT MORE INFO
void Grid::print_nodes(){
  cout<<endl;
  for( int i = 0; i < this->num_nodes; i++ ){
    cout << "Node[" << i << "]: ";
    cout << "( " << nodes[i]->get_x() << " , " << nodes[i]->get_y() << " )" << endl;
    cout << "start or end flag:  " << nodes[i]->get_start_or_end() <<endl;
    cout << "neighbor indices:   ";
    nodes[i]->print_neigh();
    cout << endl;
  }
}

// methods that prints all information
void Grid::print(){
  this->print_grid();
  this->print_nodes();
}

int manhattan_dist( Node* start_node, Node* end_node ){
  int x1 = start_node->get_x();
  int y1 = start_node->get_y();
  int x2 = end_node->get_x();
  int y2 = end_node->get_y();
  return abs( x1 - x2 ) + abs( y1 - y2 );
}

int find_max_idx( int* array ){
  int max_idx = 0;
  for( int i = 1; i < 3; i++ ){
    if( array[i] > array[max_idx] ) max_idx = i;
  }
  return max_idx;
}
