#ifndef PATHFINDING_HPP
#define PATHFINDING_HPP

#include "graph.hpp"
#include "heap.hpp"
#include <algorithm>
#include <random>
#include <limits>
#include <queue>
#include <stack>

struct FlowPath {
    std::stack<Edge*> path;
    int bottleneck;
};

FlowPath bfs_path(Graph& graph, int source, int sink);
FlowPath randomized_dfs_path(Graph& graph, int source, int sink);
FlowPath modified_dijkstra_path(Graph& graph, int source, int sink);

#endif
