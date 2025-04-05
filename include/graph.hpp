#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <sstream>
#include <vector>

struct Edge {
    int to, capacity, flow;
    Edge* reverse;
};

class Graph {
public:
    Graph();
    Graph(int n, int m);
    void read_dimacs(std::istream& in);
    std::vector<Edge>& get_neighbours(int vertex);
    int get_source() const;
    int get_sink() const;
    int get_total_vertices() const;
    int get_total_arcs() const;
    void add_edge(int origin, int destiny, int capacity);

private:
    std::vector<std::vector<Edge>> adjacency_list;
    unsigned num_vertices_;
    unsigned num_arcs_;
    int source;
    int sink;

    void resize(int n);
};

#endif // GRAPH_H