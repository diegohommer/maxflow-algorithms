#include "ford_fulk.hpp"

FordResult ford_fulkerson(Graph& graph, int source, int sink, Algorithm algo){
    int max_flow = 0;
    int iterations = 0;
    bool exists_path = false;

    SearchFunction find_path = get_search_function(algo);

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
            }
        }

    } while (exists_path);

    double r = double(iterations) / compute_max_iterations(graph, source, algo);
    return FordResult{ max_flow, iterations, r };
}

FordResult edmonds_karp(Graph& graph, int source, int sink) {
    int n = graph.get_total_vertices();
    int m = graph.get_total_arcs();
    int max_it = (n * m) / 2;
    return ford_fulkerson(graph, source, sink, Algorithm::EdmondsKarp);
}

FordResult randomized_ford_fulkerson(Graph& graph, int source, int sink) {
    int max_it = compute_source_capacity_bound(graph, source);
    return ford_fulkerson(graph, source, sink, Algorithm::RandomizedDFS);
}

FordResult fattest_path(Graph& graph, int source, int sink) {
    int C = std::max(1, compute_source_capacity_bound(graph, source));
    int m = graph.get_total_arcs();
    int max_it = int(m * std::log2(C));
    return ford_fulkerson(graph, source, sink, Algorithm::FattestPath);
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