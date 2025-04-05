#include "path_finding.hpp"

FlowPath bfs_path(Graph& graph, int source, int sink){
    // Data structures initialization
    std::vector<Edge*> parent(graph.get_total_vertices(), nullptr);
    std::queue<int> q;

    q.push(source);
    parent[source] = reinterpret_cast<Edge*>(1);
    bool found_path = false;
        
    // Run BFS to attempt to find a path to sink node
    while(!q.empty() && !found_path){
        int v = q.front();
        q.pop();

        for (Edge& edge : graph.get_neighbours(v)){
            int u = edge.to;

            if((parent[u] == nullptr) && (edge.capacity > 0)){
                parent[u] = &edge;

                if(u == sink){
                    found_path = true;
                    break;
                }
                
                q.push(u);
            }
        }
    }

    // Return empty stack and 0 if didn't find a path
    if (parent[sink] == nullptr) {
        return FlowPath { std::stack<Edge*>{}, 0 }; 
    }


    int bottleneck = std::numeric_limits<int>::max();
    std::stack<Edge*> path;

    // Trace back from sink to source calculating bottleneck
    for (int current = sink; current != source; ) {
        Edge* edge = parent[current];
        path.push(edge);
        bottleneck = std::min(bottleneck, edge->capacity);
        current = edge->reverse->to;
    }

    return FlowPath { path, bottleneck };
}