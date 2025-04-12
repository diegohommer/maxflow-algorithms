// include/ford_fulk.h
#ifndef FORD_FULK_H
#define FORD_FULK_H

#include "path_finding.hpp"
#include <functional>
#include <chrono>

using SearchFunction = std::function<FlowPath(Graph&, int, int)>;

enum class Algorithm {
    EdmondsKarp,
    RandomizedDFS,
    FattestPath
};

struct IterationStats {
    std::vector<int> visited_vert_per_iter;  // n′_i: number of vertices visited per iteration
    std::vector<int> visited_arcs_per_iter;  // m′_i: number of arcs visited per iteration
    std::vector<int> path_lengths_per_iter;  // ℓ_i: length of each augmenting path

    // For Fattest Path (Dijkstra-style)
    std::vector<int> inserts_per_iter;
    std::vector<int> deletemaxes_per_iter;
    std::vector<int> updates_per_iter;
};

struct FordResult {
    int max_flow;             // Maximum flow found by the algorithm
    int iterations;           // I: total number of augmenting iterations
    long long duration_ms;    // Time in milliseconds

    double r;                 // I / Ī: ratio to theoretical max iterations

    int total_vertices;       // Total number of vertices (n)
    int total_arcs;           // Total number of arcs (m)

    IterationStats stats;     // Per-iteration statistics
};

FordResult ford_fulkerson(Graph& graph, int source, int sink, Algorithm algo);
SearchFunction get_search_function(Algorithm algo);
int compute_max_iterations(Graph& graph, int source, Algorithm algo);
int compute_source_capacity_bound(Graph& graph, int source);
void log_iteration_stats(IterationStats stats, PathStats path_stats);

#endif // FORD_FULK_H