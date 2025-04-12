#include "ford_fulk.hpp"

FordResult ford_fulkerson(Graph& graph, int source, int sink, Algorithm algo, bool should_get_stats){
    int max_flow = 0;
    int iterations = 0;
    bool exists_path = false;
    IterationStats stats;

    SearchFunction find_path = get_search_function(algo);

    auto start = std::chrono::high_resolution_clock::now();
    do {
        FlowPath bfs_result = find_path(graph, source, sink);
        exists_path = !bfs_result.path.empty();
        iterations++;

        if (exists_path) {
            int flow = bfs_result.bottleneck;
            max_flow += flow;

            while (!bfs_result.path.empty()) {
                Edge* path_edge = bfs_result.path.top();
                bfs_result.path.pop();

                path_edge->capacity -= flow;
                graph.get_reverse(*path_edge)->capacity += flow;

                if(path_edge->capacity == 0){
                    path_edge->num_criticals++;
                }
            }
        }
        if(should_get_stats)
            log_iteration_stats(stats,bfs_result.stats);

    } while (exists_path);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    double r = 0.0;
    if(should_get_stats)
        r = double(iterations) / compute_max_iterations(graph, source, algo);

    return FordResult{ max_flow, iterations, r, duration, stats };
}

SearchFunction get_search_function(Algorithm algo) {
    switch (algo) {
        case Algorithm::EdmondsKarp:
            return bfs_path;
        case Algorithm::RandomizedDFS:
            return randomized_dfs_path;
        case Algorithm::FattestPath:
            return modified_dijkstra_path;
        default:
            throw std::invalid_argument("Unknown algorithm");
    }
}

int compute_max_iterations(Graph& graph, int source, Algorithm algo) {
    switch (algo) {
        case Algorithm::EdmondsKarp: {
            int n = graph.get_total_vertices();
            int m = graph.get_total_arcs();
            return (n * m) / 2;
        }
        case Algorithm::RandomizedDFS: {
            return compute_source_capacity_bound(graph, source);
        }
        case Algorithm::FattestPath: {
            int C = std::max(1, compute_source_capacity_bound(graph, source));
            int m = graph.get_total_arcs();
            return int(m * std::log2(C));
        }
    }
    return -1;
}

int compute_source_capacity_bound(Graph& graph, int source) {
    int bound = 0;
    for (const Edge& e : graph.get_neighbors(source)) {
        bound += e.capacity;
    }
    return bound;
};

void log_iteration_stats(IterationStats& stats, const PathStats& path_stats){
    if (path_stats.visited_verts != 0)
        stats.visited_vert_per_iter.emplace_back(path_stats.visited_verts);

    if (path_stats.visited_arcs != 0)
        stats.visited_arcs_per_iter.emplace_back(path_stats.visited_arcs);

    if (path_stats.path_length != 0)
        stats.path_lengths_per_iter.emplace_back(path_stats.path_length);

    if (path_stats.inserts != 0)
        stats.inserts_per_iter.emplace_back(path_stats.inserts);

    if (path_stats.deletemaxes != 0)
        stats.deletemaxes_per_iter.emplace_back(path_stats.deletemaxes);

    if (path_stats.updates != 0)
        stats.updates_per_iter.emplace_back(path_stats.updates);

}