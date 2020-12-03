#include <iostream>
#include <fstream>
#include <string.h>
#include <pthread.h>
#include <time.h>

using namespace std;

int const MAX_NUM_CUST = 300; // number of customers won't exceed 300
int const NUM_ATM = 10; // there are only 10 atm
int NUM_CUST; // to keep the number of remaining customers

pthread_mutex_t atmMutex[NUM_ATM] = {PTHREAD_MUTEX_INITIALIZER}; // mutex for each atm
pthread_mutex_t paymentMutex[5] = {PTHREAD_MUTEX_INITIALIZER}; // mutex for each type of payment
pthread_mutex_t numCustMutex = PTHREAD_MUTEX_INITIALIZER; // to decrease the number of customers remaining
pthread_mutex_t outputMutex = PTHREAD_MUTEX_INITIALIZER; // to write outputfile

int data[MAX_NUM_CUST][5] = {-1}; // keep all the parsed data from the input file

int payments[5] = {0}; // holds total payments

int atmCurrentData[NUM_ATM][5] = {-1}; // current customer of each atm

int atmSituation[NUM_ATM] = {0}; // if customer writing (atm waits until the customer finishs his typing)
// 0 -> no data is loaded
// 1 -> data is loaded

int customerLeave[MAX_NUM_CUST] = {0}; // customer may leave 1, 0 o/w

ofstream outputfile;

string types[5] = {"cableTV", "electricity", "gas", "telecommunication", "water"};

pthread_t customerTID[MAX_NUM_CUST]; // customer threads
pthread_t atmTID[NUM_ATM]; // atm threads

void sleep(long sleeptime){ // sleep in miliseconds
  long milisec = sleeptime; // length of time to sleep, in miliseconds
  struct timespec req = {0};
  req.tv_sec = 0;
  req.tv_nsec = milisec * 1000000L;
  nanosleep(&req, (struct timespec *)NULL);
}

// parse input string and store into global data
void process_line(int customerID, string line){

  data[customerID-1][0] = customerID;

  int sleeptime = stoi(line.substr(0, line.find(","))); // get sleeptime
  line.erase(0, line.find(",")+1); // remove sleeptime from the string
  data[customerID-1][1] = sleeptime;

  int atmNumber = stoi(line.substr(0, line.find(","))); // get atmNumber
  line.erase(0, line.find(",")+1); // remove atmNumber
  data[customerID-1][2] = atmNumber-1; // since atms are 0 indexed

  string type_s = line.substr(0, line.find(",")); // get type

  int type = -1;

  // encode its type
  if(type_s.find("cableTV") != string::npos){
    type = 0;
  }
  else if(type_s.find("electricity") != string::npos){
    type = 1;
  }
  else if(type_s.find("gas") != string::npos){
    type = 2;
  }
  else if(type_s.find("telecommunication") != string::npos){
    type = 3;
  }
  else if(type_s.find("water") != string::npos){
    type = 4;
  }
  else{
    cout << "Unidentified type: " << type_s << endl;
    exit(1);
  }

  line.erase(0, line.find(",")+1); // remove type
  data[customerID-1][3] = type;

  int amount = stoi(line); // remaining is the amount
  data[customerID-1][4] = amount;

}

//Thread for customers
void *useAtm(void *param) { // customer thread
  //param[0] -> customer id
  //param[1] -> sleeptime
  //param[2] -> atmNumber
  //param[3] -> type
  //param[4] -> amount

  int *args = (int *)param;

  sleep(args[1]);

  pthread_mutex_lock(&atmMutex[args[2]]);

  while(customerLeave[args[0]] == 0){
    if(atmSituation[args[2]] == 0){ // if no data loaded to that atm (atm is available)

      for(int i=0;i<5;i++){
        atmCurrentData[args[2]][i] = args[i]; // load data to atm
      }

      atmSituation[args[2]] = 1; // mark atm as loaded with data
      break;
    }
  }

  pthread_mutex_unlock(&atmMutex[args[2]]);

  return NULL;
}

// thread for ATMs
void *waitCustomer(void *param) { // atm's waits until a customer comes

  long temp = (long) param;
  int atmID = (int) temp;
  int currentData[5];
  // 0 -> customerID
  // 1 -> sleeptime
  // 2 -> atmID
  // 3 -> payment type
  // 4 -> amount


  while(NUM_CUST > 0){
    if(atmSituation[atmID] == 1){ // if any data is loaded to atm

      for(int i=0;i<5;i++){
        if(atmCurrentData[atmID][i] == -1){
          cout << "Data load ERROR!" << endl;
          exit(1);
        }
        currentData[i] = atmCurrentData[atmID][i]; // get customer data to local
        atmCurrentData[atmID][i] = -1; // clear data
      }

      if(currentData[2] != atmID){ // if customer's ATM Number and atm id does not match
        cout << "ATM Mismatch ERROR!!!" << endl;
        exit(1);
      }

      pthread_mutex_lock(&paymentMutex[currentData[3]]); // lock that payment type

      payments[currentData[3]] += currentData[4]; // update account

      pthread_mutex_lock(&outputMutex); // lock outputfile

      outputfile << "Customer" << currentData[0] << "," << currentData[4] << "TL," << types[currentData[3]] << endl;

      pthread_mutex_unlock(&outputMutex); // unlock outputfile

      pthread_mutex_unlock(&paymentMutex[currentData[3]]);

      pthread_mutex_lock(&numCustMutex);
      NUM_CUST--; // decrease the number of remaining customers
      pthread_mutex_unlock(&numCustMutex);

      atmSituation[atmID] = 0; // mark atm data is empty
      customerLeave[currentData[0]] = 1; // say Customer to leave

    }

  }

  return NULL;
}

int main(int argc, char** argv){

  ifstream fileReader(argv[1]); // read input file

  if(!fileReader.is_open()){
    cout << "Couldn't open input file!" << endl;
    return 1;
  }

  string file = argv[1];

  outputfile.open(file.substr(0, file.find("."))+"_log.txt");

  string line;
  getline(fileReader, line); // read first line

  int NUM_CUSTOMERS = stoi(line); // get number of customers
  NUM_CUST = NUM_CUSTOMERS; // update global var.

  // create atm threads
  for(long i=0;i<NUM_ATM;i++){ // i -> atm IDs
    pthread_create(&atmTID[i], NULL, &waitCustomer, (void*) i);
  }

  int customerID = 0; // to keep track of the id of the customer

  while(!fileReader.eof()){ // read lines
    getline(fileReader, line);

    if(line.length() <= 1){ // empty line
      break;
    }

    customerID++; // track customer id

    process_line(customerID, line); // parse line to global data

    for(int i=0;i<5;i++){ // check if data is loaded successfuly to global
      if(data[customerID-1][i] == -1){
        cout << "Parsing ERROR!" << endl;
        exit(1);
      }
    }
    //create threads
    pthread_create(&customerTID[customerID-1], NULL, &useAtm, (void*) data[customerID-1]);

  }
  fileReader.close();


  //wait all threads
  for(int i=0;i<customerID;i++){
    pthread_join(customerTID[i], NULL);
  }
  for(int i=0;i<NUM_ATM;i++){
    pthread_join(atmTID[i], NULL);
  }

  outputfile << "All payments are completed." << endl;
  string CapitalTypes[5] = {"CableTV", "Electricity", "Gas", "Telecommunication", "Water"};
  for(int i=0;i<5;i++){
    outputfile << CapitalTypes[i] << ": " <<payments[i] << "TL"<< endl;
  }

  outputfile.close();


  return 0;
}
