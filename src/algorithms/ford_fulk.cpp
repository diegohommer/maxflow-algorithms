#include "ford_fulk.hpp"

FordResult ford_fulkerson(Graph& graph, int source, int sink, SearchFunction find_path){
    int max_flow = 0;
    bool exists_path = false;

    do{
        // Look for augmentation path using BFS
        FlowPath bfs_result = find_path(graph, source, sink);
        exists_path = !bfs_result.path.empty();

        if(exists_path){
            int flow = bfs_result.bottleneck;
            max_flow += flow;
            
            // Update residual capacities
            while(!bfs_result.path.empty()){
                Edge* path_edge = bfs_result.path.top();
                bfs_result.path.pop();

                path_edge->flow += flow;
                path_edge->capacity -= flow;
                graph.get_reverse(*path_edge)->capacity += flow;
            }
        }

    }while(exists_path);


    return FordResult { max_flow };
};

FordResult edmonds_karp(Graph& graph, int source, int sink){
    return ford_fulkerson(graph, source, sink, bfs_path);
};

FordResult randomized_ford_fulkerson(Graph& graph, int source, int sink){
    return ford_fulkerson(graph, source, sink, randomized_dfs_path);
};