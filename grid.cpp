#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "node.h"
#include "grid.h"

#define INT_MAX 2147483647

using namespace std;

/*                *\
  *   PUBLIC     *
*\                */
// default constructor
Grid::Grid(){
  this->grid = NULL;
  this->num_rows = 0;
  this->num_cols = 0;
  this->num_paths = 0;
}
// full constructor
Grid::Grid( string grid_path, string node_list_path ){
  // init grid, num_cols, num_rows
  read_grid( grid_path );
  // init nodes, num_nodes
  read_nodes( node_list_path );
  this->num_paths = 0;
}

//destructor
Grid::~Grid(){
  // delete grid
  for ( int i = 0; i < num_rows; i++ ){
    delete [] grid[i];
  }
  delete [] grid;

  // delete nodes
  for ( int i = 0; i < num_nodes; i++ ){
    delete nodes[i];
  }
  delete [] nodes;
}

// method that generates a 2D array given a
// text file and returns a pointer to the array
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
      infile >> this -> grid[j][i];
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
  find_neigh();
}

// method that finds each node's 3 nearest neighbors using Manhattan distance
// and records the index of those neighbors in the neigh_idx private array
void Grid::find_neigh(){
  int** neigh_matrix = create_matrix();
  // loop through all nodes
  for( int i = 0; i < num_nodes; i++ ){
    // array to store Manhattan distances of all other nodes in respect to node at index i
    float distances[num_nodes];
    // loop to compute Manhattan distance of all other nodes
    for( int j = 0; j < num_nodes; j++ ){
      // if to ensure node does not compute Manhattan distance of itself
      if( i != j ){
        distances[j] = manhattan_dist( nodes[i], nodes[j] );
      } else{
        // set the distance of the node being tested equal to the
        //maximum int value so it can never have the lowest score
        distances[j] = INT_MAX;
      }
    }
    // find 3 lowest Manhattan distances in the array and store the
    //index of the score in private nodes array
    float min[3] = { distances[0], distances[1], distances[2] };
    int min_idx[3] = { 0, 1, 2 };
    int max_min_idx = find_max_idx( min );
    for( int j = 3; j < num_nodes; j++ ){
      if( distances[j] < min[max_min_idx] ){
        min[max_min_idx] = j;
        min_idx[max_min_idx] = j;
        max_min_idx = find_max_idx( min );
      }
    }
    // insert node's neighbors into neigh_matrix
    matrix_insert( neigh_matrix, i, min_idx );

  }
  // generate neigh_idx vector based on neigh_matrix
  to_node( neigh_matrix );
  delete_matrix( neigh_matrix );
}

void Grid::set_start_end( int start_idx, int end_idx ){
  for( int i = 0; i < num_nodes; i++ ){
    // set flag if node is start or end point
    if ( i == start_idx ) nodes[i]->set_start_or_end(1);
    if ( i == end_idx ) nodes[i]->set_start_or_end(2);
  }
  this->start_idx = start_idx;
  this->end_idx = end_idx;
}

// method that finds all paths from the start node to the end node
// min depth == 3
// max depth == 5
void Grid::find_all_paths(){
  // Mark all nodes as not visited
  bool *visited = new bool[num_nodes];

  // Create an array to store paths
  int *path = new int[num_nodes];
  int path_index = 0;

  // Initialize all nodes as not visited
  for ( int i = 0; i < num_nodes; i++ )
      visited[i] = false;

  // Call the recursive helper function to print all paths
  int s = start_idx;
  int e = end_idx;
  all_path(s, e, visited, path, path_index);
  num_paths = all_paths.size();
  delete [] path;
  delete [] visited;
  //set_pairs();
}

// getter for a single pair in pairs vector
int* Grid::get_pair( int path_idx, int pair_idx ){
  vector<vector<int>>::iterator v = all_paths.begin();
  v+= path_idx;
  vector<int> v_vec = *v;
  vector<int>::iterator p = v_vec.begin();
  p+= pair_idx;
  int n1 = *p;
  int n2 = *(p+1);
  int* arr = (int*) malloc(sizeof(int)*2);
  arr[0] = n1;
  arr[1] = n2;
  return arr;
}
void Grid::del_pair(int * p){
  free(p);
}
// method that returns a single path at the index passed to it
vector<int> Grid::get_path( int idx ){
  vector< vector<int> >::iterator i = all_paths.begin();
  if ( idx > num_paths ){
    printf("invalid index");
    exit(0);
  }
  i+=idx;
  return *i;
}

int Grid::get_pairs_size(){
  return pairs.size();
}
// method that finds shortest distane between nodes at n1_idx and n2_idx
int Grid::find_distance( int n1_idx , int n2_idx ){
  // TODO FINISH FIND DISTANCE
  Node* n1 = nodes[n1_idx];
  Node* n2 = nodes[n2_idx];
  int dist = n1_idx + n2_idx;
  return dist;
}

int Grid::get_num_paths(){
  return num_paths;
}
// method that prints all paths from the start to end node
void Grid::print_all_paths(){
  int idx = 0;
  for( auto i = all_paths.begin(); i != all_paths.end(); ++i ){
    cout << "all_paths[" << idx << "]:  ";
    for( auto j = i->begin(); j != i->end(); ++j ){
      cout << *j << " ";
    }
    cout << endl;
    idx++;
  }
  cout << endl;
}
// method that prints the 2D array & its demensions
void Grid::print_grid(){
  cout<<endl;
  for( int i = 0; i < num_rows; i++ ){
    for( int j = 0; j < num_cols; j++ ){
      for( int k = 0; k < num_nodes; k++ ){
        if ( nodes[k]->get_x() == j && nodes[k]->get_y() == i )
          printf( "\033[1;31m[%d]\033[0m", k );

      }
      printf("%f ", grid[j][i] );
    }
    cout << endl;
  }
  cout << endl;
  cout << "num_rows: " << num_rows << endl;
  cout << "num_cols: " << num_cols << endl;
}

// method that prints all nodes coordinates TODO: MAYBE PRINT MORE INFO
void Grid::print_nodes(){
  cout<<endl;
  for( int i = 0; i < num_nodes; i++ ){
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
  print_grid();
  print_nodes();
  print_all_paths();
}

/*                *\
  *   PRIVATE    *
*\                */

// recursive helper function for find_all_paths
void Grid::all_path( int start, int end, bool visited[], int path[], int &path_index ){
  // Mark the current node and store it in path array
  visited[start] = true;
  path[path_index] = start;
  path_index++;

  // If current vertex is same as destination, then print
  // current path array
  if ( start == end ){
    // temp vector to store values in path array
    vector<int> temp;
    for ( int i = 0; i < path_index; i++ ){
      temp.push_back( path[i] );
    }
    // if the path visits at least 3 nodes, but no more than 5
    // push_back the temp vector to the private all_paths vector of int vectors
    if ( temp.size() >= 3 && temp.size() <= 5 )
      all_paths.push_back( temp );
  // If current node is not destination
  }else{
    vector<int> neighs = nodes[start]->get_neigh();
    // Recur for all the node's neighbors
    for( auto i = neighs.begin(); i != neighs.end(); ++i )
      if ( !visited[*i] )
        all_path( *i, end, visited, path, path_index );
  }
  // Remove current node from path array and mark it as unvisited
  path_index--;
  visited[start] = false;
}
// method to init pairs vector of int vetors, given a path index
void Grid::set_pairs(){
  for( int i = 0; i < num_paths; i ++ ){
    for ( unsigned int j = 0; j < all_paths.at(i).size() - 1 ; j++ ){
      vector<int> temp;
      temp.push_back( all_paths.at( i ).at( j ) );
      temp.push_back( all_paths.at( i ).at( j + 1 ) );
      // using std::find with vector and iterator:
      vector< vector<int> >::iterator it;
      it = find( pairs.begin(), pairs.end(), temp );
      if (it != pairs.end() ){
        continue;
      }
      else{
        pairs.push_back( temp );
      }
    }
  }
}
// function to compute Manhattan distance between 2 nodes
int Grid::manhattan_dist( Node* start_node, Node* end_node ){
  int x1 = start_node->get_x();
  int y1 = start_node->get_y();
  int x2 = end_node->get_x();
  int y2 = end_node->get_y();
  return abs( x1 - x2 ) + abs( y1 - y2 );
}

// helper function that finds the greatest element in an array
// ARRAY LENGTH MUST ALWAYS == 3
int Grid::find_max_idx( float* array ){
  int max_idx = 0;
  for( int i = 1; i < 3; i++ ){
    if( array[i] > array[max_idx] ) max_idx = i;
  }
  return max_idx;
}

// function that creates a 2D array used to keep track of the node's neighbors
// 1 == neighbors
// 0 == not neighbors
int** Grid::create_matrix(){
  int** matrix = new int*[num_nodes];
  for( int i = 0; i < num_nodes; ++i ){
    matrix[i] = new int[num_nodes];
  }
  // set all elements equal to 0
  for( int i = 0; i < num_nodes; i++ ){
    for( int j = 0; j < num_nodes; j++ ){
      matrix[i][j] = 0;
    }
  }
  return matrix;
}
// function that inserts a single node's neighbors into the matrix
// ARRAY LENGTH MUST ALWAYS == 3
void Grid::matrix_insert( int** matrix, int index, int* array ){
  for( int i = 0; i < 3; i++ ){
    int x = array[i];
    matrix[index][x] = 1;
    matrix[x][index] = 1;
  }
}
// function called on a final matrix
// interprets matrix and stores results in neigh_idx vector
void Grid::to_node( int** matrix ){
  for( int i = 0; i < num_nodes; i++ ){
    vector<int> neigh_idx;
    for( int j = 0; j < num_nodes; j++ ){
      if( matrix[i][j] == 1 ){
        neigh_idx.push_back( j );
      }
    }
    nodes[i]->set_neigh( neigh_idx );
  }
}
// function to delete matrix after use
void Grid::delete_matrix( int** matrix ){
  for( int i = 0; i < num_nodes; ++i ){
    delete [] matrix[i];
  }
  delete [] matrix;
}
