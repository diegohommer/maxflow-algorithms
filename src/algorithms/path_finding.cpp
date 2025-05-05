#include "path_finding.hpp"

FlowPath bfs_path(Graph& graph, int source, int sink)
{
    // Data structures initialization
    std::vector<Edge*> parent_edges(graph.get_total_vertices(), nullptr);
    std::queue<int> q;
    PathStats stats;

    q.push(source);
    bool found_path = false;

    // Run BFS to attempt to find a path to sink node
    while (!q.empty() && !found_path) {
        int v = q.front();
        q.pop();
        stats.visited_verts++;

        for (Edge& edge : graph.get_outgoing_edges(v)) {
            int u = edge.to;
            stats.visited_arcs++;

            if ((u != source) && (!parent_edges[u]) && (edge.capacity > 0)) {
                parent_edges[u] = &edge;

                if (u == sink) {
                    found_path = true;
                    break;
                }

                q.push(u);
            }
        }
    }

    // Return empty stack and 0 if didn't find a path
    if (!parent_edges[sink]) {
        return FlowPath{std::stack<Edge*>{}, 0, stats};
    }

    int bottleneck = std::numeric_limits<int>::max();
    std::stack<Edge*> path;

    // Trace back from sink to source calculating bottleneck
    for (int current = sink; current != source;) {
        Edge* edge = parent_edges[current];
        stats.path_length++;
        path.push(edge);
        bottleneck = std::min(bottleneck, edge->capacity);
        current = graph.get_reverse(*edge)->to;
    }

    return FlowPath{path, bottleneck, stats};
}

FlowPath randomized_dfs_path(Graph& graph, int source, int sink)
{
    // Data structures initialization
    std::vector<Edge*> parent_edges(graph.get_total_vertices(), nullptr);
    std::stack<int> q;
    PathStats stats;

    q.push(source);
    bool found_path = false;

    // Random number generator
    static std::random_device rd;
    static std::mt19937 rng(rd());

    // Run Randomized DFS to attempt to find a path to sink node
    while (!q.empty() && !found_path) {
        int v = q.top();
        q.pop();
        stats.visited_verts++;

        // Get neighbors and shuffle in-place (no copies)
        auto& neighbors = graph.get_outgoing_edges(v);
        std::vector<Edge*> neighbor_ptrs;
        neighbor_ptrs.reserve(neighbors.size());
        for (Edge& e : neighbors) {
            neighbor_ptrs.push_back(&e);
        }
        std::shuffle(neighbor_ptrs.begin(), neighbor_ptrs.end(), rng);

        for (Edge* edge : neighbor_ptrs) {
            int u = edge->to;
            stats.visited_arcs++;

            if ((u != source) && (!parent_edges[u]) && (edge->capacity > 0)) {
                parent_edges[u] = edge;

                if (u == sink) {
                    found_path = true;
                    break;
                }

                q.push(u);
            }
        }
    }

    // Return empty stack and 0 if didn't find a path
    if (!parent_edges[sink]) {
        return FlowPath{std::stack<Edge*>{}, 0, stats};
    }

    int bottleneck = std::numeric_limits<int>::max();
    std::stack<Edge*> path;

    // Trace back from sink to source calculating bottleneck
    for (int current = sink; current != source;) {
        Edge* edge = parent_edges[current];
        stats.path_length++;
        path.push(edge);
        bottleneck = std::min(bottleneck, edge->capacity);
        current = graph.get_reverse(*edge)->to;
    }

    return FlowPath{path, bottleneck, stats};
}

FlowPath modified_dijkstra_path(Graph& graph, int source, int sink)
{
    int num_verts = graph.get_total_vertices();
    PathStats stats;

    // Initialize priority queue (8-Ary MaxHeap) and parent_edges vector
    std::vector<Edge*> parent_edges(graph.get_total_vertices(), nullptr);
    KHeap priority_queue(num_verts, 8);
    priority_queue.insert(source, std::numeric_limits<int>::max(), nullptr);
    stats.inserts++;

    // Run Customized Dijkstra to attempt to find fattest path
    while (priority_queue.get_size() > 0) {
        HeapNode current = priority_queue.deletemax();
        int v = current.vertex;
        stats.visited_verts++;
        stats.deletemaxes++;

        parent_edges[v] = current.incoming_edge;
        if (v == sink)
            break;

        // Process each neighbor of the current vertex
        for (Edge& edge : graph.get_outgoing_edges(v)) {
            int u = edge.to;
            stats.visited_arcs++;

            if ((u != source) && (!parent_edges[u]) && (edge.capacity > 0)) {
                int new_bottleneck = std::min(current.capacity, edge.capacity);
                int existing_bottleneck = priority_queue.get_vertex_cap(u);

                if (existing_bottleneck == -1) {
                    priority_queue.insert(u, new_bottleneck, &edge);
                    stats.inserts++;
                } else if (existing_bottleneck < new_bottleneck) {
                    priority_queue.update(u, new_bottleneck, &edge);
                    stats.updates++;
                }
            }
        }
    }

    // Return empty stack and 0 if didn't find a path
    if (!parent_edges[sink]) {
        return FlowPath{std::stack<Edge*>{}, 0, stats};
    }

    int bottleneck = std::numeric_limits<int>::max();
    std::stack<Edge*> path;

    // Trace back from sink to source calculating bottleneck
    for (int current = sink; current != source;) {
        Edge* edge = parent_edges[current];
        stats.path_length++;
        path.push(edge);
        bottleneck = std::min(bottleneck, edge->capacity);
        current = graph.get_reverse(*edge)->to;
    }

    return FlowPath{path, bottleneck, stats};
}
