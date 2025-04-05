// include/ford_fulk.h
#ifndef FORD_FULK_H
#define FORD_FULK_H

#include "path_finding.hpp"
#include <functional>

using SearchFunction = std::function<FlowPath(Graph&, int, int)>;

struct FordResult {
    int max_flow;
};

FordResult ford_fulkerson(Graph& graph, int source, int sink, SearchFunction find_path);
FordResult edmonds_karp(Graph& graph, int source, int sink);
FordResult randomized_ford_fulkerson(Graph& graph, int source, int sink);

#endif // FORD_FULK_H