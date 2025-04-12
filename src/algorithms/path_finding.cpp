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

FlowPath modified_dijkstra_path(Graph& graph, int source, int sink){
    int num_verts = graph.get_total_vertices();

    std::vector<Edge*> parent_edges(graph.get_total_vertices(), nullptr);

    // Initialize priority queue (8-Ary MaxHeap)
    KHeap priority_queue(num_verts,8);
    priority_queue.insert(source, std::numeric_limits<int>::max(), nullptr);

    // Run Customized Dijkstra to attempt to find fattest path 
    while (priority_queue.get_size() > 0) {
        HeapNode current = priority_queue.deletemax();
        int v = current.vertex;

        parent_edges[v] = current.incoming_edge;
        if (v == sink) break;
    
        // Process each neighbor of the current vertex
        for (Edge& edge : graph.get_neighbors(v)) {
            int u = edge.to;
    
            if ((u != source) && (!parent_edges[u]) && (edge.capacity > 0)) {
                int new_bottleneck = std::min(current.capacity, edge.capacity);
                int existing_bottleneck = priority_queue.get_vertex_cap(u);
    
                if (existing_bottleneck == -1) {  
                    priority_queue.insert(u, new_bottleneck, &edge);
                } else if (existing_bottleneck < new_bottleneck) {  
                    priority_queue.update(u, new_bottleneck, &edge);
                }
            }
        }
    }

    // Return empty stack and 0 if didn't find a path
    if (!parent_edges[sink]) {
        return FlowPath { std::stack<Edge*>{}, 0 }; 
    }

    int bottleneck = std::numeric_limits<int>::max();
    std::stack<Edge*> path;

    // Trace back from sink to source calculating bottleneck
    for (int current = sink; current != source; ) {
        Edge* edge = parent_edges[current];
        path.push(edge);
        bottleneck = std::min(bottleneck, edge->capacity);
        current = graph.get_reverse(*edge)->to;
    }

    return FlowPath { path, bottleneck };
}