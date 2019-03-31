
  // init memory map for child
  static int* path_idx;
  path_idx = (int*) mmap(NULL, sizeof *path_idx, PROT_READ | PROT_WRITE,
                MAP_SHARED | MAP_ANONYMOUS, -1, 0);


  // create children for each fork
  for( int i = 0; i < grid.get_num_paths(); ++i ){
    *path_idx = i;
    // if child process
    if( fork == 0 ){
      /*                *\
        *     CHILD    *
      *\                */
      // create grandchild for each pair of points in the path
      // ex path [ 1 , 2 , 3 , 4 ]
      // pairs   [1,2],[2,3],[3,4]
      static int* dist;
      dist = (int*) mmap(NULL, sizeof *dist, PROT_READ | PROT_WRITE,
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);

      for( unsigned int j = 0; j < grid.get_path(i).size() - 1; j++ ){
        // if grandchild process
        int* test = grid.get_pair(i,j);
        cout<<test[0]<<test[1]<<endl;
        if( fork() == 0 ){
          /*                *\
            *  GRANDCHILD  *
          *\                */
          // get every pair of nodes and pass to grandchild process
          vector<int>pair = grid.get_path(i);
          int num1 = pair.at( j );
          int num2 = pair.at( j+1 );
          cout<< "num1: " << num1 << "    num2: " << num2 << endl;

          // find shortest distance from num1 to num2
          *dist += grid.find_distance( num1, num2 );


          // write output to file
          char str[10];
          sprintf(str + strlen(str), "%ld", (long) getppid() );
          printf("%s\n",str);
          string fname = out_dir;
          fname.append( str );
          std::ofstream outfile;
          outfile.open(fname, ios_base::app);
          outfile << num1 << " " << num2 << " " <<  endl;

          // wait for all grandchild processes to finish
          exit(0);
        } // end grandchild
      } // end grandchild forking
      for( unsigned int j = 0; j < grid.get_path(i).size() - 1; j++ )
        wait(NULL);
      cout<<"dist"<<*dist<<endl;
      munmap(dist, sizeof *dist);

      /*
      // wait until all child processes finish
      while (( parent_pid = wait(&status) ) > 0);
      munmap(all_paths_idx, sizeof *all_paths_idx);
      */
      exit(0);
    } // end child
  } // end child forking
  for( int i = 0; i < grid.get_num_paths(); ++i  )
    wait(NULL);
  munmap( path_idx, sizeof *path_idx );
  return 0;
}
