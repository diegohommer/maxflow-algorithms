#ifndef PATHFINDING_HPP
#define PATHFINDING_HPP

#include "graph.hpp"
#include "heap.hpp"
#include <algorithm>
#include <random>
#include <limits>
#include <queue>
#include <stack>

struct PathStats {
    int visited_verts;
    int visited_arcs;
    int path_length;

    // For fattest path only
    int inserts;
    int deletemaxes;
    int updates;


    // Constructor
    PathStats(int visited_verts = 0, int visited_arcs = 0, int path_length = 0, 
        int inserts = 0, int deletemaxes = 0, int updates = 0)
        : visited_verts(visited_verts), visited_arcs(visited_arcs), path_length(path_length),
            inserts(inserts), deletemaxes(deletemaxes), updates(updates) {}
};

struct FlowPath {
    // Resulting path and it's bottleneck
    std::stack<Edge*> path;
    int bottleneck;
    PathStats stats;
};

FlowPath bfs_path(Graph& graph, int source, int sink);
FlowPath randomized_dfs_path(Graph& graph, int source, int sink);
FlowPath modified_dijkstra_path(Graph& graph, int source, int sink);

#endif
