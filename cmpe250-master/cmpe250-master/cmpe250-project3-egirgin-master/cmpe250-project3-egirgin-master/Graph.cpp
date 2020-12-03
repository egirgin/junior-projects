//
// Created by emregirgin on 17-Nov-18.
//

#include "Graph.h"
#include <stack>
#include <iostream>
#include <fstream>
#include <iterator>

using namespace std;

stack<int> myStack;

int index=0;

void Graph::strongconnect(Node& node){

    node.index = index;
    node.lowlink = index;
    index = index+1;
    myStack.push(node.id);
    node.onStack = true;

    for(int id : node.keys){
        if(this->nodes.at(id).index==-1){
            strongconnect(this->nodes.at(id));
            node.lowlink = min(node.lowlink, this->nodes.at(id).lowlink);
        }
        else if(this->nodes.at(id).onStack){
            node.lowlink = min(node.lowlink, this->nodes.at(id).index);
        }
    }
    if(node.lowlink == node.index){
        //this->clusters.push_back(Cluster(node.lowlink));
        this->clusters.insert(pair<int, Cluster> (node.lowlink, Cluster(node.lowlink)));
        node.cluster_id=node.lowlink;
        while(myStack.top()!= node.id){
            this->nodes.at(myStack.top()).onStack = false;
            //cout<<clusters.back().cluster_id<<". Cluster'a, "<<myStack.top()+1<<". node ekleniyor"<<endl;
            nodes.at(myStack.top()).cluster_id=node.lowlink;
            this->clusters.at(node.lowlink).nodes.push_back(myStack.top());
            myStack.pop();
        }

        this->nodes.at(myStack.top()).onStack = false;
        nodes.at(myStack.top()).cluster_id=node.lowlink;
        this->clusters.at(node.lowlink).nodes.push_back(myStack.top());
        myStack.pop();

    }

}

void Graph::tarjans() {

    for(int i = 0; i<nodes.size() ; i++){
        if(nodes[i].index == -1){
            strongconnect(nodes[i]);
        }
    }
    /*
    for(Cluster cl : this->clusters){
        cout<<cl.cluster_id<<". Cluster'ın nodeları:"<<endl;
        for(Node node : cl.nodes){
            cout<<node.id+1<<" ";
        }
        cout<<endl;
    }
     */

}

void Graph::find_breakables() {
    map <int, Cluster> :: iterator itr;
    for(itr = clusters.begin();itr != clusters.end(); itr++){
        for(int node_id : itr->second.nodes){
            for(int key : nodes.at(node_id).keys){
                if(itr->first != nodes.at(key).cluster_id){
                    //cout<<node_id<<". node "<<nodes.at(key).cluster_id<<". cluster'ı açabilir."<<endl;
                    //cout<<"ilk elemanı: "<<clusters.at(nodes.at(key).cluster_id).nodes.at(0)<<endl;
                    clusters.at(nodes.at(key).cluster_id).break_this = false;
                }
            }
        }
    }
}

void Graph::print_breakables() {
    for(int i=0;i<clusters.size();i++){
        if(clusters.at(i).break_this){
            cout<<i<<endl;
        }
    }

}

void Graph::print_clusters() {

    map <int, Cluster> :: iterator itr;
    for(itr = clusters.begin();itr != clusters.end(); itr++){
        cout<<"Cluster id: "<<itr->first<<" ";
        if(itr->second.break_this){
            cout<<"breakable"<<endl;
        }
        else{
            cout <<"not breakable"<<endl;
        }
        for(int node : itr->second.nodes){
            cout<<node<<" ";
        }
        cout<<endl;
    }
    cout<<endl;

    /*
    for(Cluster cl : clusters){
        cout<<"Cluster id: "<<cl.cluster_id<<" ";
        if(cl.break_this){
            cout<<"breakable"<<endl;
        }
        else{
            cout<<"not breakable"<<endl;
        }
        for(int node : cl.nodes){
            cout<<node<<" ";
        }
        cout<<endl;
    }
     */
}