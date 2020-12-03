//
// Created by emregirgin on 17-Nov-18.
//

#ifndef PROJECT3_GRAPH_H
#define PROJECT3_GRAPH_H

#include <vector>
#include <iostream>
#include <map>

using namespace std;

class Node{
public:
    vector<int> keys;
    bool visited;
    int index;
    int id;
    int lowlink;
    bool onStack;
    int cluster_id;
    Node(int i){
        visited=false;
        id=i;
        index = -1;
        lowlink = -1;
        onStack = false;
        cluster_id=-1;
    }
    void add_key(int key){
        keys.push_back(key);
    };
};

class Cluster{
public:
    int cluster_id;
    bool break_this;
    vector<int> nodes;
    Cluster(int cluster_id){
        this->cluster_id = cluster_id;
        break_this=true;
    }
};

class Graph{
public:
    vector<Node> nodes;
    map <int, Cluster> clusters;
    //vector<Cluster> clusters;
    Graph(){

    }
    void add_node(Node& node){
        nodes.push_back(node);
    };
    void tarjans();
    void strongconnect(Node& node);
    void find_breakables();
    void print_breakables();
    void print_clusters();
    void print_nodes(){
        for(int i=0;i<nodes.size();i++){
            cout<<nodes.at(i).id<<" ";
        }
        cout<<endl;
    }
};

#endif //PROJECT3_GRAPH_H
