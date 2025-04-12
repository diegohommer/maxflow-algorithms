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

struct FordResult {
    int max_flow;     // Maximum flow found by the algorithm
    int iterations;   // Number of augmenting path iterations performed (I)
    signed long duration;  // Number of ms it took for the algorithm to find the solution.  
    double r;         // Fraction of maximum possible iterations: I / Ī
    double s_avg;     // Average fraction of vertices visited per iteration (s̄)
    double t_avg;     // Average fraction of edges visited per iteration (t̄)
};

FordResult ford_fulkerson(Graph& graph, int source, int sink, Algorithm algo);
SearchFunction get_search_function(Algorithm algo);
int compute_max_iterations(Graph& graph, int source, Algorithm algo);
int compute_source_capacity_bound(Graph& graph, int source);

#endif // FORD_FULK_H