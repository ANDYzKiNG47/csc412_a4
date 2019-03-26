#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

#define MAX_NUM 10
using namespace std;
/*
  * PROGRAM THAT GENERATES RANDOM GRID DATA GIVEN:
  * number of row
  * number of columns
  * output path
  *
  * GRID DATA IS A FLOAT RANGING FROM 0.0 TO 10.0
*/
int main( int argc, char** argv ){
  int rows = atoi( argv[1] );
  int cols = atoi( argv[2] );
  string out_path = argv[3];
  float num;
  srand (static_cast <unsigned> (time(0)));
  ofstream outfile;
  outfile.open(out_path);
  outfile << rows << " " << cols << endl;
  for ( int i = 0; i < rows; i++ ){
    for ( int j = 0 ; j < cols; j++ ){
      num = static_cast <float> ( rand() ) / (static_cast <float> ( RAND_MAX / MAX_NUM ));
      outfile << num << " ";
    }
    outfile << endl;
  }
  outfile.close();

}
