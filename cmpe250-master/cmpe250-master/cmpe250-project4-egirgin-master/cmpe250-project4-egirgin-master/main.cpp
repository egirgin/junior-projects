#include <iostream>
#include <fstream>
#include "Grid.h"

using namespace std;

int main(int argc, char* argv[]) {

    clock_t start = clock();

    Grid grd;

    ifstream ifs;
    ifs.open(argv[1]);

    int N,M;
    ifs >> N;
    grd.row = N;
    ifs >> M;
    grd.column = M;
    int grid[N][M];
    int id=0;
    grd.graph_size = M*N;
    for(int i=0;i<N; i++) {
        for (int j = 0; j < M; j++) {
            int value;
            ifs >> value;
            grid[i][j] = value;
            grd.adj.push_back(vector<pair<int,int>>());
            //cout<<id<<endl;
            if(i>0){
                //cout<<"üst komsu: "<<id-M<<" fark:"<<abs(grid[i][j]-grid[i-1][j])<<endl;
                grd.adj[id].push_back(make_pair(abs(grid[i][j]-grid[i-1][j]) , id-M));
                grd.adj[id-M].push_back(make_pair(abs(grid[i][j]-grid[i-1][j]) , id));
            }
            if(j>0){
                //cout<<"sol komsu: "<<id-1<<" fark:"<<abs(grid[i][j]-grid[i][j-1])<<endl;
                grd.adj[id].push_back(make_pair(abs(grid[i][j]-grid[i][j-1]) , id-1));
                grd.adj[id-1].push_back(make_pair(abs(grid[i][j]-grid[i][j-1]) , id));
            }
            id++;
        }
    }


    //grd.print_graph();


    int query;

    ifs>>query;

    int x1,y1,x2,y2;
    ifs>>x1>>y1>>x2>>y2;

    x1--;y1--;x2--;y2--;
    pair<int,int> src(x1,y1);
    pair<int,int> sink(x2,y2);

    ofstream streamf(argv[2]);
        
    vector<int> distances = move(grd.dijkstra(src,sink));
        
    int result = distances[grd.id_calculator(sink)];
        
    streamf << result<<endl;
    
    vector<pair< pair<int,int> , pair<int,int> > > queries;

    for(int i=1;i<query;i++){
        ifs>>x1>>y1>>x2>>y2;
        //queries.push_back(make_pair( make_pair(x1-1,y1-1), make_pair(x2-1,y2-1)));

        x1--;y1--;x2--;y2--;
        pair<int,int> src(x1,y1);
        pair<int,int> sink(x2,y2);

        if(distances[grd.id_calculator(src)] != distances[grd.id_calculator(sink)]){
            streamf<< max(distances[grd.id_calculator(src)],distances[grd.id_calculator(sink)])<<endl;
        }
        else{
            distances = move(grd.dijkstra(src,sink));
        
            streamf << distances[grd.id_calculator(sink)]<<endl;
        }    
    }

    clock_t stop = clock();
    double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    printf("\nTime elapsed: %.5f\n", elapsed);

    return 0;
}