#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>

#include "node.h"

using namespace std;

// default constructor
Node::Node(){
  this->x_coord = -1;
  this->y_coord = -1;
  this->start_or_end = 0;
  this->num_neigh = 0;
}
// constructor, no neigh_idx
Node::Node( int x_coord, int y_coord ){
  this->x_coord = x_coord;
  this->y_coord = y_coord;
  this->start_or_end = 0;
  this->num_neigh = 0;
}
// full constructor
Node::Node( int x_coord, int y_coord, vector<int> neigh_idx ){
  this->x_coord = x_coord;
  this->y_coord = y_coord;
  this -> neigh_idx = neigh_idx;
  this->num_neigh = neigh_idx.size();
}

// setters and getters
int Node::get_x(){
  return this->x_coord;
}
void Node::set_x( int x ){
  this->x_coord = x;
}
int Node::get_y(){
  return this->y_coord;
}
void Node::set_y( int y ){
  this->y_coord = y;
}
vector<int> Node::get_neigh(){
  return this->neigh_idx;
}
void Node::set_neigh( vector<int> neigh_idx ){
  this->neigh_idx = neigh_idx;
  this->num_neigh = neigh_idx.size();
}
int Node::get_start_or_end(){
  return this->start_or_end;
}
void Node::set_start_or_end( int flag ){
  this->start_or_end = flag;
}
void Node::print_neigh(){
  for( auto i = this->neigh_idx.begin(); i != this->neigh_idx.end(); ++i ){
    cout << *i << " ";
  }
  cout << endl;
}
