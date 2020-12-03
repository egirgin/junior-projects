/*
__author__: Emre Girgin
__date__: 17/Dec/19
__e-mail__:emregirgin171@gmail.com
*/
#include <iostream>
#include <mpi.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <string.h>
#include <sstream>
#include <math.h>
#include <string>


using namespace std;

int main(int argc, char** argv){

  int rank, size, core_size; // core_size : number of bits that a core is going to responsible
  ifstream fileReader;

  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int T;
  sscanf(argv[3], "%d", &T);

  if (rank == 0){

    fileReader.open(argv[1]);

    vector<int> data; // to store bitmap

    if(fileReader.is_open()){
      string line; // a line of a map
      string token; // a single bit

      while(! fileReader.eof() ){ // read file until and of the file line by line
        getline(fileReader, line);
        stringstream lineStream(line);

        //vector< string > tokens; // to store bits of a single line

        while( lineStream >> token){ // read tokens to end of the line
          //tokens.push_back(token);
          if(token.compare("0") == 0){
            data.push_back(0);
          }else if(token.compare("1") == 0){
            data.push_back(1);
          }else{
            cout << "bit couldn't cast: " << token << endl;
          }
        }
      }
    }
    else{
      cout << "File couldn't be opened." << endl;
    }

    fileReader.close();


    int grid_size = 360/ (int) sqrt(size-1);
    core_size = grid_size * grid_size;
    int map_size = (int) sqrt(size-1);

    vector<vector<int> > splitted;

    int is_sent[data.size()] = {0}; // to check overlap

    for(int cores=0; cores<size-1; cores++){
      vector<int> core_data;
      int start_point = 360*grid_size*(cores/map_size) + grid_size*(cores%map_size) ;
      for(int i=0;i<grid_size;i++){
        for(int j=0;j<grid_size;j++){
          if(is_sent[start_point + i*360 + j] == 0){
            core_data.push_back(data[start_point + i*360 + j]);
            is_sent[start_point + i*360 + j] = 1;
          }
          else{
            cout << "ERROR: " << start_point + i*360 + j << " is sent before!" << endl;
          }
        }
      }


      //cout << "core number: " << cores << endl;
      //cout << "from: " << start_point << endl;
      //cout << "to: " << start_point + (grid_size-1)*360 + (grid_size-1) << endl;
      splitted.push_back(core_data);
      //cout << "core data size: " << core_data.size() << endl;
      //cout << "-----" <<endl;
      //cout << "sendind data to: " << cores << endl;

      MPI_Send(&grid_size, 1, MPI_INT, cores+1, 0, MPI_COMM_WORLD); // send one dim. of a core
      MPI_Send(&core_data[0], core_data.size(), MPI_INT, cores+1, 0, MPI_COMM_WORLD); // send the core data
      //cout << "data is sent to " << cores+1 <<". core!" <<endl;
    }
    /* TO CHECK IS THERE A BIT BEING NEVER SENT.
    int counter = 0;

    for(int i =0 ;i< data.size(); i++){
    if(is_sent[i] != 1){
    counter ++;
  }
}
cout << "never sent: " <<counter << endl;
*/

int output[360*360];

for(int i=0;i< map_size;i++){
  for(int q=0;q<grid_size;q++){ // lines of coming data
    for(int j=0;j<map_size;j++){
      int point = i*grid_size*360 + q*360 + j*grid_size;
      // cout << "Master is waiting " <<  i*map_size+j+1 << " to send line " << q << endl;
      MPI_Recv(&output[point], grid_size , MPI_INT, i*map_size+j+1, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
  }
}

ofstream outputFile;
outputFile.open(argv[2]);

for(int i=0;i<360*360;i++){
  outputFile << output[i] << " ";
  if(i%360==359){
    outputFile << endl;
  }
}

outputFile.close();


}
else{
  int grid_size;
  MPI_Recv(&grid_size, 1 , MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // get dimension
  int my_data[grid_size][grid_size];

  //cout << rank << ". core is receiving" << endl;
  MPI_Recv(&my_data, grid_size*grid_size , MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // get core data
  //cout << "data is received by " << rank <<". core!" << endl;

  //calculate row and column of core (zero indexed)
  int row = (rank - 1) / ( (int) sqrt(size-1) );
  //determine neighbors
  int column =  (rank - 1) % ( (int) sqrt(size-1) );

  //size -1 : number of slave cores
  int topNeighbor = rank - (int) sqrt(size-1);
  if(row == 0){ //upmost row
    topNeighbor = rank - (int) sqrt(size-1) + (size -1);
  }
  int bottomNeighbor = rank + (int) sqrt(size-1);
  if(row == ( (int) sqrt(size-1) -1 ) ){ //downmost row
    bottomNeighbor = rank + (int) sqrt(size-1) - (size -1);
  }
  int leftNeighbor = rank - 1;
  if(column == 0){ //if leftmost column
    leftNeighbor = rank -1 + (int) sqrt(size-1);
  }
  int rightNeighbor = rank + 1;
  if(column == ( (int) sqrt(size-1) -1 ) ){ //if rightmost column
    rightNeighbor = rank + 1 - (int) sqrt(size-1);
  }

  for(int iteration=0;iteration < T; iteration++){

    //Edges
    int upper[grid_size];
    int bottom[grid_size];
    int left[grid_size];
    int right[grid_size];

    //Corners
    int upperLeft = my_data[0][0];
    int upperRight = my_data[0][grid_size-1];
    int bottomLeft = my_data[grid_size-1][0];
    int bottomRight = my_data[grid_size-1][grid_size-1];

    //Fill edges
    for(int i=0;i<grid_size;i++){
      upper[i] = my_data[0][i];
    }
    for(int i=0;i<grid_size;i++){
      bottom[i] = my_data[grid_size-1][i];
    }
    for(int i=0;i<grid_size;i++){
      left[i] = my_data[i][0];
    }
    for(int i=0;i<grid_size;i++){
      right[i] = my_data[i][grid_size-1];
    }



    /*
    cout << "My rank is " << rank << " and my top: " << topNeighbor;
    cout << ", bottom: "<< bottomNeighbor;
    cout << ", left: "<< leftNeighbor;
    cout << ", right: "<< rightNeighbor << endl;
    */

    //received data from neighbors
    int fromTop[grid_size];
    int fromBottom[grid_size];
    int fromLeft[grid_size];
    int fromRight[grid_size];

    if(rank%2 == 1){ // sending left and right

      MPI_Send(&left[0], grid_size, MPI_INT, leftNeighbor, 1, MPI_COMM_WORLD); // send left row to left negihbor
      MPI_Send(&right[0], grid_size, MPI_INT, rightNeighbor, 1, MPI_COMM_WORLD); // send right row to right negihbor

      // cout << "Sender: " << rank << " | Receiver: " << leftNeighbor << " and " << rightNeighbor << endl;

      MPI_Recv(&fromRight, grid_size , MPI_INT, rightNeighbor, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(&fromLeft, grid_size , MPI_INT, leftNeighbor, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    }
    else{

      MPI_Recv(&fromRight, grid_size , MPI_INT, rightNeighbor, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(&fromLeft, grid_size , MPI_INT, leftNeighbor, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      MPI_Send(&left[0], grid_size, MPI_INT, leftNeighbor, 1, MPI_COMM_WORLD); // send left row to left negihbor
      MPI_Send(&right[0], grid_size, MPI_INT, rightNeighbor, 1, MPI_COMM_WORLD); // send right row to right negihbor

      // cout << "Sender: " << rank << " | Receiver: " << leftNeighbor << " and " << rightNeighbor << endl;
    }


    if(row%2==0){ // sending top and bottom

      MPI_Send(&upper[0], grid_size, MPI_INT, topNeighbor, 1, MPI_COMM_WORLD); // send upper row to upper negihbor
      MPI_Send(&bottom[0], grid_size, MPI_INT, bottomNeighbor, 1, MPI_COMM_WORLD); // send bottom row to bottom negihbor

      // cout << "Sender: " << rank << " | Receiver: " << topNeighbor << " and " << bottomNeighbor << endl;

      MPI_Recv(&fromBottom, grid_size , MPI_INT, bottomNeighbor, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(&fromTop, grid_size , MPI_INT, topNeighbor, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //get edge from top neighbor

    }
    else{

      MPI_Recv(&fromBottom, grid_size , MPI_INT, bottomNeighbor, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(&fromTop, grid_size , MPI_INT, topNeighbor, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //get edge from top neighbor

      MPI_Send(&upper[0], grid_size, MPI_INT, topNeighbor, 1, MPI_COMM_WORLD); // send upper row to upper negihbor
      MPI_Send(&bottom[0], grid_size, MPI_INT, bottomNeighbor, 1, MPI_COMM_WORLD); // send bottom row to bottom negihbor

      // cout << "Sender: " << rank << " | Receiver: " << topNeighbor << " and " << bottomNeighbor << endl;
    }

    int fromTopLeft, fromTopRight, fromBottomLeft, fromBottomRight;

    //corners
    if(rank%2 == 1){ // odd ranks

      //send topRight of evens
      MPI_Send(&fromTop[0], 1, MPI_INT, leftNeighbor, 2, MPI_COMM_WORLD); // topright of left

      //cout << rank << " sent " << fromTop[0] << " to " << leftNeighbor << endl;

      //send topleft of evens
      MPI_Send(&fromTop[grid_size-1], 1, MPI_INT, rightNeighbor, 2, MPI_COMM_WORLD); // topleft of right

      //cout << rank << " sent " << fromTop[grid_size-1] << " to " << rightNeighbor << endl;

      //get topRight
      MPI_Recv(&fromTopRight, 1 , MPI_INT, rightNeighbor, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      //get topleft
      MPI_Recv(&fromTopLeft, 1 , MPI_INT, leftNeighbor, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      //sent bottomRight of evens
      MPI_Send(&fromBottom[0], 1, MPI_INT, leftNeighbor, 2, MPI_COMM_WORLD);

      //send bottomleft of evens
      MPI_Send(&fromBottom[grid_size-1], 1, MPI_INT, rightNeighbor, 2, MPI_COMM_WORLD);

      //get botomRight
      MPI_Recv(&fromBottomRight, 1 , MPI_INT, rightNeighbor, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      //get bottomleft
      MPI_Recv(&fromBottomLeft, 1 , MPI_INT, leftNeighbor, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    }
    else{ //even ranks

      //get topright
      MPI_Recv(&fromTopRight, 1 , MPI_INT, rightNeighbor, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      //get topleft
      MPI_Recv(&fromTopLeft, 1 , MPI_INT, leftNeighbor, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      //send topright of odds
      MPI_Send(&fromTop[0], 1, MPI_INT, leftNeighbor, 2, MPI_COMM_WORLD);

      //cout << rank << " sent " << fromTop[0] << " to " << leftNeighbor << endl;

      //send topleft of odds
      MPI_Send(&fromTop[grid_size-1], 1, MPI_INT, rightNeighbor, 2, MPI_COMM_WORLD);

      //cout << rank << " sent " << fromTop[grid_size-1] << " to " << rightNeighbor << endl;

      //get bottomRight
      MPI_Recv(&fromBottomRight, 1 , MPI_INT, rightNeighbor, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      //get bottomleft
      MPI_Recv(&fromBottomLeft, 1 , MPI_INT, leftNeighbor, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      //send bottomright of odds
      MPI_Send(&fromBottom[0], 1, MPI_INT, leftNeighbor, 2, MPI_COMM_WORLD);

      //cout << rank << " sent " << fromTop[0] << " to " << leftNeighbor << endl;

      //send bottomLeft of odds
      MPI_Send(&fromBottom[grid_size-1], 1, MPI_INT, rightNeighbor, 2, MPI_COMM_WORLD);

    }

    int new_data[grid_size][grid_size];

    int ones = 0;
    int zeros = 0;

    //inner cells
    for(int i=1;i < grid_size-1;i++){
      for(int j=1; j < grid_size-1;j++){

        if(my_data[i][j] != 0 && my_data[i][j] != 1){
          cout << "Neither 0 nor 1!" << endl;
        }

        if(my_data[i][j] == 0){
          zeros++;
        }else{
          ones++;
        }

        int neighbors = my_data[i-1][j-1] + my_data[i-1][j] + my_data[i-1][j+1] + \
        my_data[i][j+1] + my_data[i+1][j+1] + my_data[i+1][j] + my_data[i+1][j-1] + my_data[i][j-1];
        if(neighbors < 2){
          new_data[i][j] = 0;
        }
        else if(neighbors > 3){
          new_data[i][j] = 0;
        }
        else if(neighbors == 3){
          new_data[i][j] = 1;
        }
        else{
          new_data[i][j] = my_data[i][j];
        }
      }
    }

    //upper cells except corners
    for(int i=1;i<grid_size-1;i++){

      if(my_data[0][i] != 0 && my_data[0][i] != 1){
        cout << "Neither 0 nor 1!" << endl;
      }

      int neighbors = fromTop[i-1] + fromTop[i] + fromTop[i+1] + \
      my_data[0][i+1] + my_data[1][i+1] + my_data[1][i] + my_data[1][i-1] + my_data[0][i-1];

      if(neighbors < 2){
        new_data[0][i] = 0;
      }
      else if(neighbors > 3){
        new_data[0][i] = 0;
      }
      else if(neighbors == 3){
        new_data[0][i] = 1;
      }
      else{
        new_data[0][i] = my_data[0][i];
      }

    }

    //right cells except corners
    for(int i=1;i<grid_size-1;i++){

      if(my_data[i][grid_size-1] != 0 && my_data[i][grid_size-1] != 1){
        cout << "Neither 0 nor 1!" << endl;
      }

      int neighbors = fromRight[i-1] + fromRight[i] + fromRight[i+1] + \
      my_data[i+1][grid_size-1] + my_data[i+1][grid_size-2] + my_data[i][grid_size-2] + my_data[i-1][grid_size-2] + my_data[i-1][grid_size-1];

      if(neighbors < 2){
        new_data[i][grid_size-1] = 0;
      }
      else if(neighbors > 3){
        new_data[i][grid_size-1] = 0;
      }
      else if(neighbors == 3){
        new_data[i][grid_size-1] = 1;
      }
      else{
        new_data[i][grid_size-1] = my_data[i][grid_size-1];
      }

    }

    //bottom cells except corners
    for(int i=1;i<grid_size-1;i++){

      if(my_data[grid_size-1][i] != 0 && my_data[grid_size-1][i] != 1){
        cout << "Neither 0 nor 1!" << endl;
      }

      int neighbors = fromBottom[i-1] + fromBottom[i] + fromBottom[i+1] + \
      my_data[grid_size-1][i+1] + my_data[grid_size-2][i+1] + my_data[grid_size-2][i] + my_data[grid_size-2][i-1] + my_data[grid_size-1][i-1];

      if(neighbors < 2){
        new_data[grid_size-1][i] = 0;
      }
      else if(neighbors > 3){
        new_data[grid_size-1][i] = 0;
      }
      else if(neighbors == 3){
        new_data[grid_size-1][i] = 1;
      }
      else{
        new_data[grid_size-1][i] = my_data[grid_size-1][i];
      }

    }

    //left cells except corners
    for(int i=1;i<grid_size-1;i++){

      if(my_data[i][0] != 0 && my_data[i][0] != 1){
        cout << "Neither 0 nor 1!" << endl;
      }

      int neighbors = fromLeft[i-1] + fromLeft[i] + fromLeft[i+1] + \
      my_data[i+1][0] + my_data[i+1][1] + my_data[i][1] + my_data[i-1][1] + my_data[i-1][0];

      if(neighbors < 2){
        new_data[i][0] = 0;
      }
      else if(neighbors > 3){
        new_data[i][0] = 0;
      }
      else if(neighbors == 3){
        new_data[i][0] = 1;
      }
      else{
        new_data[i][0] = my_data[i][0];
      }

    }

    int upperLeftNeighs = fromTopLeft + fromTop[0] + fromTop[1] +\
    fromLeft[0] + fromLeft[1] + \
    my_data[0][1] + my_data[1][1] + my_data[1][0];

    if(upperLeftNeighs < 2){
      new_data[0][0] = 0;
    }
    else if(upperLeftNeighs > 3){
      new_data[0][0] = 0;
    }
    else if(upperLeftNeighs == 3){
      new_data[0][0] = 1;
    }
    else{
      new_data[0][0] = my_data[0][0];
    }

    int upperRightNeighs = fromTopRight + fromTop[grid_size-1] + fromTop[grid_size-2] +\
    fromRight[0] + fromRight[1] + \
    my_data[0][grid_size-2] + my_data[1][grid_size-2] + my_data[1][grid_size-1];

    if(upperRightNeighs < 2){
      new_data[0][grid_size-1] = 0;
    }
    else if(upperRightNeighs > 3){
      new_data[0][grid_size-1] = 0;
    }
    else if(upperRightNeighs == 3){
      new_data[0][grid_size-1] = 1;
    }
    else{
      new_data[0][grid_size-1] = my_data[0][grid_size-1];
    }

    int bottomRightNeighs = fromBottomRight + fromBottom[grid_size-1] + fromBottom[grid_size-2] +\
    fromRight[grid_size-1] + fromRight[grid_size-2] + \
    my_data[grid_size-1][grid_size-2] + my_data[grid_size-2][grid_size-2] + my_data[grid_size-2][grid_size-1];

    if(bottomRightNeighs < 2){
      new_data[grid_size-1][grid_size-1] = 0;
    }
    else if(bottomRightNeighs > 3){
      new_data[grid_size-1][grid_size-1] = 0;
    }
    else if(bottomRightNeighs == 3){
      new_data[grid_size-1][grid_size-1] = 1;
    }
    else{
      new_data[grid_size-1][grid_size-1] = my_data[grid_size-1][grid_size-1];
    }

    int bottomLeftNeighs = fromBottomLeft + fromBottom[0] + fromBottom[1] +\
    fromLeft[grid_size-1] + fromLeft[grid_size-2] + \
    my_data[grid_size-2][0] + my_data[grid_size-2][1] + my_data[grid_size-1][1];

    if(bottomLeftNeighs < 2){
      new_data[grid_size-1][0] = 0;
    }
    else if(bottomLeftNeighs > 3){
      new_data[grid_size-1][0] = 0;
    }
    else if(bottomLeftNeighs == 3){
      new_data[grid_size-1][0] = 1;
    }
    else{
      new_data[grid_size-1][0] = my_data[grid_size-1][0];
    }

    for(int i=0;i<grid_size;i++){
      for(int j=0;j<grid_size;j++){
        my_data[i][j] = new_data[i][j];
      }
    }

    // cout << "Ones: " << ones << ", Zeros: " << zeros << endl;
  }

  for(int i=0;i< grid_size;i++){
    // cout << "Core " << rank << " is sending line " << i << endl;
    MPI_Send(&my_data[i], grid_size, MPI_INT, 0,99, MPI_COMM_WORLD); // send the core data
  }



}


MPI_Finalize();

}
