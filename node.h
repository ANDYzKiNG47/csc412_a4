#ifndef NODE_H
#define NODE_H
class Node{

  public:
    Node();
    Node( int x_coord, int y_coord );
    Node( int x_coord, int y_coord, int* neigh_idx );
    int get_x();
    void set_x( int x );
    int get_y();
    void set_y( int y );
    int* get_neigh();
    void set_neigh( int* neigh_idx );
    int get_start_or_end();
    void set_start_or_end( int flag );


  private:
    int x_coord;
    int y_coord;
    int* neigh_idx;
    /*
      0 == not start or end of path ( deault )
      1 == start of path
      2 == end of path
    */
    int start_or_end;
};
#endif
