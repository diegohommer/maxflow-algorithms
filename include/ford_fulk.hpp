#ifndef FORD_FULK_H
#define FORD_FULK_H

#include <chrono>
#include <functional>

#include "path_finding.hpp"

// Alias for a function pointer type that represents the search function used in
// the Ford-Fulkerson algorithm. It takes a graph, a source vertex, and a sink
// vertex as inputs, and returns a FlowPath.
using SearchFunction = std::function<FlowPath(Graph &, int, int)>;

// Enum representing the available Ford-Fulkerson algorithm types.
enum class Algorithm {
    // A randomized version of the Ford-Fulkerson method using DFS
    RandomizedDFS,

    // The Edmonds-Karp algorithm, a specific implementation of the
    // Ford-Fulkerson method using BFS
    EdmondsKarp,

    // A variation that uses the fattest-path strategy, finding paths
    // with the highest residual capacity
    FattestPath
};

// Structure holding statistics about each iteration during the execution of the
// Ford-Fulkerson algorithm.
struct IterationStats {
    std::vector<double> s_per_iter;  // s_i = n′_i / n: fraction of vertices visited per iteration
    std::vector<double> t_per_iter;  // t_i = m′_i / m: fraction of arcs visited per iteration
    std::vector<int> path_lengths_per_iter;  // ℓ_i: length of each augmenting path

    // Normalized heap operation stats for Fattest Path
    std::vector<double> inserts_per_iter;      // iᵢ = insertsᵢ / n
    std::vector<double> deletemaxes_per_iter;  // dᵢ = deletemaxesᵢ / n
    std::vector<double> updates_per_iter;      // uᵢ = updatesᵢ / m
};

// Structure holding the result of the Ford-Fulkerson algorithm, including the
// maximum flow, the number of iterations, and per-iteration statistics.
struct FordResult {
    int max_flow;           // The maximum flow found by the algorithm
    int flow_upper_bound;   // Upper limit C of flow that can passed from src2sink
    int iterations;         // The total number of augmenting iterations performed
    long long duration_ms;  // Total time taken for the algorithm in milliseconds

    IterationStats stats;  // Statistics gathered during each iteration of the algorithm
};

// The main Ford-Fulkerson algorithm that runs the selected algorithm on the
// graph and computes the maximum flow. It takes the graph, source, sink,
// algorithm type, and whether or not to collect per-iteration statistics.
FordResult ford_fulkerson(Graph &graph, int source, int sink, Algorithm algo,
                          bool should_get_stats);

// A utility function that returns the appropriate search function based on the
// selected algorithm type.
SearchFunction get_search_function(Algorithm algo);

// A utility function that stores the per-iteration statistics for the given
// algorithm execution. It collects the path statistics for each iteration and
// stores them in the IterationStats object.
void store_iteration_stats(IterationStats &stats, const PathStats &path_stats, int n, int m);

#endif  // FORD_FULK_H
