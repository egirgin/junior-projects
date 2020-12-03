#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>
#include "SurveyClass.h"
using namespace std;


template <class Container>
void split1(const string& str, Container& cont)
{
    istringstream iss(str);
    copy(istream_iterator<string>(iss),
              istream_iterator<string>(),
              back_inserter(cont));
}

float findAmount(const vector<string> words)
{
    float return_value = 0;
    for(int i=0; i<words.size(); i++){
        if (words[i][0] == '$') {
            const char *cstr = (words[i].substr(1)).c_str();
            return_value = strtof(cstr, NULL);
            cout << "return_value: " << return_value << endl;
            break;
        }
    }
    return return_value;
}

int main(int argc, char* argv[]) {
    // below reads the input file
    // in your next projects, you will implement that part as well
    if (argc != 3) {
        cout << "Run the code with the following command: ./project1 [input_file] [output_file]" << endl;
/*
        Node *n1 = new Node("emre",11.1);
        Node *n2 = new Node("alpi",22.2);
        *n1=move(*n2);
*/
        /*
        SurveyClass sc;
        sc.handleNewRecord("emre",11.1);
        sc.handleNewRecord("alpi",22.2);

        SurveyClass sc2;

        cout<<"sc: "<<&sc<<endl;
        cout<<"sc2: "<<&sc2<<endl;

        cout<<"sc list: "<<sc.members<<endl;
        cout<<"sc2 list: "<<sc2.members<<endl;

        sc2=move(sc);

        cout<<"sc: "<<&sc<<endl;
        cout<<"sc2: "<<&sc2<<endl;

        cout<<"sc list: "<<sc.members<<endl;
        cout<<"sc2 list: "<<sc2.members<<endl;
        */
        /*
        LinkedList ls;
        ls.pushTail("emre",11.1);
        ls.pushTail("alpi",22.2);
        LinkedList::printList(ls);
        LinkedList ls1=move(ls);
        cout<<"ls1"<<endl;
        LinkedList::printList(ls1);
        cout<<"ls"<<endl;
        LinkedList::printList(ls);
*/
        //delete ls;
        //ls= nullptr;
        //delete ls1;
        //ls1= nullptr;

        return 1;
    }

    cout << "input file: " << argv[1] << endl;
    cout << "output file: " << argv[2] << endl;





    // here, perform the input operation. in other words,
    // read the file named <argv[1]>
    ifstream infile(argv[1]);
    string line;
    vector<string> input;
    // process first line
    getline(infile, line);
    int N = stoi(line);
    cout << "number of input lines: " << N << endl;

    SurveyClass mySurveyClass;
    for (int i=0; i<N; i++) {
        getline(infile, line);
        cout << "line: " << line << endl;

        vector<string> words;
        split1(line,words);
        string curr_name = words[0];
        float curr_amount = findAmount(words);

        mySurveyClass.handleNewRecord(curr_name, curr_amount);
    }

    cout << "input file has been read" << endl;

    float minExp = mySurveyClass.calculateMinimumExpense();
    float maxExp = mySurveyClass.calculateMaximumExpense();
    float avgExp = mySurveyClass.calculateAverageExpense();

    // here, perform the output operation. in other words,
    // print your results into the file named <argv[2]>
    ofstream myfile;
    myfile.open (argv[2]);
    myfile << minExp << " " << maxExp << " " << avgExp << endl;
    myfile.close();

    cout << "minExp " << minExp << endl;
    cout << "maxExp " << maxExp << endl;
    cout << "avgExp " << avgExp << endl;

    return 0;
}
