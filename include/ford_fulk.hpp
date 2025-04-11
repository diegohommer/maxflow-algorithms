// include/ford_fulk.h
#ifndef FORD_FULK_H
#define FORD_FULK_H

#include "path_finding.hpp"
#include <functional>

using SearchFunction = std::function<FlowPath(Graph&, int, int)>;

enum class Algorithm {
    EdmondsKarp,
    RandomizedDFS,
    FattestPath
};

struct FordResult {
    int max_flow;     // Maximum flow found by the algorithm
    int iterations;   // Number of augmenting path iterations performed (I)
    double r;         // Fraction of maximum possible iterations: I / Ī
    double s_avg;     // Average fraction of vertices visited per iteration (s̄)
    double t_avg;     // Average fraction of edges visited per iteration (t̄)
};

FordResult ford_fulkerson(Graph& graph, int source, int sink, Algorithm algo);
FordResult edmonds_karp(Graph& graph, int source, int sink);
FordResult randomized_ford_fulkerson(Graph& graph, int source, int sink);
FordResult fattest_path(Graph& graph, int source, int sink);
SearchFunction get_search_function(Algorithm algo);
int compute_max_iterations(Graph& graph, int source, Algorithm algo);
int compute_source_capacity_bound(Graph& graph, int source);

#endif // FORD_FULK_H