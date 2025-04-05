#ifndef PATHFINDING_HPP
#define PATHFINDING_HPP

#include "graph.hpp"
#include <limits>
#include <queue>
#include <stack>

struct FlowPath {
    std::stack<Edge*> path;
    int bottleneck;
};

FlowPath bfs_path(Graph& graph, int source, int sink);
FlowPath dfs_path(Graph& graph, int source, int sink);
FlowPath fattest_path(Graph& graph, int source, int sink);

#endif
