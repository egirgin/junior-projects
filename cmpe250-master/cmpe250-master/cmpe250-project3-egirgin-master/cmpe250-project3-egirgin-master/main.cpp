#include <iostream>
#include "Graph.h"
#include <fstream>
using namespace std;

int main(int argc, char* argv[]) {

    clock_t start = clock();

    int N;
    int storage;
    int key;

    Graph graph;

    ifstream scanner;
    scanner.open(argv[1]);
    scanner>>N;
    for(int i=0;i<N;i++) {
        scanner>>storage;
        Node node(i);
        graph.add_node(node);
        for(int j=0;j<storage;j++) {
            scanner >> key;
            graph.nodes.at(i).add_key(key-1);
        }
    }
    scanner.close();

    graph.tarjans();
    graph.find_breakables();
    //graph.print_breakables();

    //graph.print_clusters();

    //graph.print_nodes();
    vector<int> result;
    ofstream stream_f;
    stream_f.open(argv[2]);
    int num_of_breakables=0;
    map <int, Cluster> :: iterator itr;
    for(itr = graph.clusters.begin();itr != graph.clusters.end(); itr++){
        if(itr->second.break_this){
            num_of_breakables++;
            result.push_back(itr->second.nodes.at(0)+1);
        }
    }
    stream_f<<num_of_breakables<<" ";
    for(int i : result){
        stream_f<<i<<" ";
    }

    stream_f.close();


    clock_t stop = clock();
    double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    printf("\nTime elapsed: %.5f\n", elapsed);

    return 0;
}