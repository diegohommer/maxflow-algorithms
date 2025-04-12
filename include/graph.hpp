#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <sstream>
#include <vector>

struct Edge {
    int to;                // Destination node
    int capacity;          // Residual capacity of the edge
    int reverse_idx;       // Index of the reverse edge in the adjacency list
    int num_criticals = 0; // Counts how many times this edge was critical

    Edge(int t, int cap, int rev)
    : to(t), capacity(cap), reverse_idx(rev) {}
};

class Graph {
public:
    Graph(std::istream& in);
    void read_dimacs(std::istream& in);
    Edge* get_forward(int source_vertex, int edge_index);
    Edge* get_reverse(Edge edge);
    std::vector<Edge>& get_neighbors(int vertex);
    int get_source() const;
    int get_sink() const;
    int get_total_vertices() const;
    int get_total_arcs() const;

private:
    std::vector<std::vector<Edge>> adjacency_list;
    unsigned num_vertices_;
    unsigned num_arcs_;
    int source;
    int sink;

    void resize(int n);
    void add_edge(int origin, int destiny, int capacity);
};

#endif // GRAPH_H