#ifndef PATHFINDING_HPP
#define PATHFINDING_HPP

#include "graph.hpp"
#include "heap.hpp"
#include <algorithm>
#include <random>
#include <limits>
#include <queue>
#include <stack>

// Structure to hold the statistics of a single augmenting path found during the algorithm's execution.
// This includes information about the vertices, arcs, and the length of the path, along with additional statistics
// for specific algorithms like Fattest Path.
struct PathStats {
    int visited_verts;   // Number of visited vertices during the path search
    int visited_arcs;    // Number of visited arcs during the path search
    int path_length;     // Length of the augmenting path

    // For Fattest Path only (Dijkstra-style algorithm)
    int inserts;         // Number of insertions into the heap
    int deletemaxes;     // Number of deletions of the maximum capacity edge from the heap
    int updates;         // Number of heap updates during the search

    // Constructor to initialize the statistics
    PathStats(int visited_verts = 0, int visited_arcs = 0, int path_length = 0, 
        int inserts = 0, int deletemaxes = 0, int updates = 0)
        : visited_verts(visited_verts), visited_arcs(visited_arcs), path_length(path_length),
          inserts(inserts), deletemaxes(deletemaxes), updates(updates) {}
};

// Structure to represent a flow path found in the graph. It stores the actual path (as a stack of edges),
// the bottleneck capacity (the smallest capacity in the path), and the associated statistics for the path search.
struct FlowPath {
    std::stack<Edge*> path;  // Stack of edges representing the flow path
    int bottleneck;          // The bottleneck capacity (minimum capacity in the path)
    PathStats stats;         // Path statistics (e.g., number of visited vertices/arcs, path length)
};

// Function declarations for different pathfinding strategies used in the Ford-Fulkerson algorithm.

// Performs a breadth-first search (BFS) to find an augmenting path in the graph.
FlowPath bfs_path(Graph& graph, int source, int sink);

// Performs a randomized depth-first search (DFS) to find an augmenting path in the graph.
FlowPath randomized_dfs_path(Graph& graph, int source, int sink);

// Performs a modified Dijkstra's algorithm to find the fattest path in the graph (path with the highest bottleneck).
FlowPath modified_dijkstra_path(Graph& graph, int source, int sink);

#endif
