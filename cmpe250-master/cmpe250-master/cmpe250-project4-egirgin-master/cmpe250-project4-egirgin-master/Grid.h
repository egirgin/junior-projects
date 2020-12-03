//
// Created by emregirgin on 30-Nov-18.
//

#ifndef PROJECT4_GRID_H
#define PROJECT4_GRID_H

#include <vector>
#include <map>

using namespace std;

class Grid{
public:

    int row,column;
    int graph_size;

    vector<vector<pair<int,int>>> adj;

    void print_graph();

    vector<int> dijkstra(pair<int,int> src, pair<int,int> sink);

    int id_calculator(pair<int,int>);

};



#endif //PROJECT4_GRID_H
