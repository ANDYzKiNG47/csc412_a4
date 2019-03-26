#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>

#define MIN 0
using namespace std;

int random(int min, int max);

/*
  * PROGRAM THAT GENERATES A FILE OF RANDOM NODES GIVEN:
  * number of nodes to generate
  * size of X axis of grid
  * size of Y axis of grid
  * output file
  *
  * OUTPUT FORMAT IF: num_nodes = 5, x_size = 10, y_size = 9
  * 5             <- number of NODES
  * 3 6           <- X Y
  * 1 8
  * 9 2
  * 4 5
  * 4 7
*/
int main( int argc, char** argv ){
  int num_nodes = atoi( argv[1] );
  int x_size = atoi( argv[2] ) - 1;
  int y_size = atoi( argv[3] ) - 1;
  string out_path = argv[4];
  ofstream outfile;
  outfile.open(out_path);
  outfile << num_nodes << endl;
  for ( int i = 0; i < num_nodes; i++ ){
    int x_num = random( MIN, x_size );
    int y_num = random( MIN, y_size );
    outfile << x_num << " " << y_num << endl;
  }
  outfile.close();


}

int random(int min, int max){
   static bool first = true;
   if (first){
      srand( time(NULL) ); //seeding for the first time only
      first = false;
   }
   return min + rand() % (( max + 1 ) - min);
}
