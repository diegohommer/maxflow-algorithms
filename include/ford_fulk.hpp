// include/ford_fulk.h
#ifndef FORD_FULK_H
#define FORD_FULK_H

#include "path_finding.hpp"

struct FordResult {
    int max_flow;
};

FordResult edmonds_karp(Graph& graph, int source, int sink);

#endif // FORD_FULK_H