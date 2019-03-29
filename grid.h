#ifndef GRID_H
#define GRID_H

#include <string>
#include "node.h"

using namespace std;
class Grid{
  public:
    Grid();
    Grid( string grid_path, string node_list_path );
    ~Grid();
    void read_grid( string grid_path );
    void read_nodes( string node_list_path );
    void find_neigh();
    void set_start_end( int start_idx, int end_idx );
    void print_grid();
    void print_nodes();
    void print();
  private:
    float** grid;
    int num_rows;
    int num_cols;
    int num_nodes;
    Node** nodes;
    int find_max_idx( float* array );
    int manhattan_dist( Node* start_node, Node* end_node );
    int** create_matrix();
    void matrix_insert( int** matrix, int index, int* array );
    void to_node( int** matrix );
    void delete_matrix( int** matrix );
};

#endif
