#include "path_finding.hpp"

FlowPath bfs_path(Graph& graph, int source, int sink){
    // Data structures initialization
    std::vector<Edge*> parent(graph.get_total_vertices(), nullptr);
    std::queue<int> q;

    q.push(source);
    bool found_path = false;
        
    // Run BFS to attempt to find a path to sink node
    while(!q.empty() && !found_path){
        int v = q.front();
        q.pop();

        for (Edge& edge : graph.get_neighbors(v)){
            int u = edge.to;

            if((u != source) && (!parent[u]) && (edge.capacity > 0)){
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
    if (!parent[sink]) {
        return FlowPath { std::stack<Edge*>{}, 0 }; 
    }


    int bottleneck = std::numeric_limits<int>::max();
    std::stack<Edge*> path;

    // Trace back from sink to source calculating bottleneck
    for (int current = sink; current != source; ) {
        Edge* edge = parent[current];
        path.push(edge);
        bottleneck = std::min(bottleneck, edge->capacity);
        current = graph.get_reverse(*edge)->to;
    }

    return FlowPath { path, bottleneck };
}

FlowPath randomized_dfs_path(Graph& graph, int source, int sink){
    // Data structures initialization
    std::vector<Edge*> parent(graph.get_total_vertices(), nullptr);
    std::stack<int> q;

    q.push(source);
    bool found_path = false;

    // Random number generator
    static std::random_device rd;
    static std::mt19937 rng(rd());
        
    // Run Randomized DFS to attempt to find a path to sink node
    while(!q.empty() && !found_path){
        int v = q.top();
        q.pop();

        // Get neighbors and shuffle in-place (no copies)
        auto& neighbors = graph.get_neighbors(v);
        std::vector<Edge*> neighbor_ptrs;
        neighbor_ptrs.reserve(neighbors.size());
        for (Edge& e : neighbors) {
            if (e.capacity > 0) neighbor_ptrs.push_back(&e);
        }
        std::shuffle(neighbor_ptrs.begin(), neighbor_ptrs.end(), rng);
        
        for (Edge* edge : neighbor_ptrs) {
            int u = edge->to;

            if((u != source) && (!parent[u]) && (edge->capacity > 0)){
                parent[u] = edge;
        
                if(u == sink){
                    found_path = true;
                    break;
                }
                
                q.push(u);
            }
        }
    }

    // Return empty stack and 0 if didn't find a path
    if (!parent[sink]) {
        return FlowPath { std::stack<Edge*>{}, 0 }; 
    }


    int bottleneck = std::numeric_limits<int>::max();
    std::stack<Edge*> path;

    // Trace back from sink to source calculating bottleneck
    for (int current = sink; current != source; ) {
        Edge* edge = parent[current];
        path.push(edge);
        bottleneck = std::min(bottleneck, edge->capacity);
        current = graph.get_reverse(*edge)->to;
    }

    return FlowPath { path, bottleneck };
}