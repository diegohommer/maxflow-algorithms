#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <sstream>
#include <vector>

struct Edge {
    int to, capacity, flow, reverse_idx;

    Edge(int t, int cap, int f, int rev)
    : to(t), capacity(cap), flow(f), reverse_idx(rev) {}
};

class Graph {
public:
    Graph(std::istream& in);
    void read_dimacs(std::istream& in);
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