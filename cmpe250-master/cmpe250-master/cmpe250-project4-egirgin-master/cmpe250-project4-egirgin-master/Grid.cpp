//
// Created by emregirgin on 30-Nov-18.
//

#include "Grid.h"
#include <iostream>
#include <queue>
#include <algorithm>

using namespace std;

void Grid::print_graph(){

    for(int i=0;i<adj.size();i++){
        cout << i<<": ";
        for(int j=0;j<adj[i].size();j++){
            cout<< adj[i][j].second<<"--"<<adj[i][j].first<<" , ";
        }
        cout<<endl;
        cout<<"-------------------"<<endl;
    }
}

int Grid::id_calculator(pair<int,int> coordinates){
    return coordinates.first*column+coordinates.second;
}


typedef pair<int,int > edges;

vector<int> Grid::dijkstra(pair<int,int> src, pair<int,int> sink) {

    priority_queue< edges, vector <edges> , greater<edges> > heap;

    vector<int> dist (adj.size(), 1000000001);

    int src_id = id_calculator(src);

    heap.push(make_pair(0,src_id));
    dist[src_id] = 0;

    while (!heap.empty()) {
        int u = heap.top().second;
        heap.pop();

        for (int i = 0; i < adj[u].size(); i++) {

            int v = adj[u][i].second;
            int weight = adj[u][i].first;

            if (dist[v] > max(dist[u], weight)) {
                dist[v] = max(dist[u], weight);
                heap.push(make_pair(dist[v], v));
            }
        }
    }

    return dist;
}
