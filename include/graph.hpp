#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <limits>
#include <sstream>
#include <vector>

// Edge structure representing a directed edge with capacity, reverse index, and
// critical edge count
struct Edge {
    int to;                 // Destination node of the edge
    int capacity;           // Residual capacity of the edge (how much capacity is left)
    int reverse_idx;        // Index of the reverse edge in the adjacency list
    int num_criticals = 0;  // Times this edge was critical in the flow algorithm

    Edge(int t, int cap, int rev) : to(t), capacity(cap), reverse_idx(rev) {}
};

enum class GraphInputFormat { Dimacs, Tournament };

// Graph class represents a directed graph using an adjacency list and supports
// various graph operations
class Graph
{
   public:
    // Default constructor that does nothing
    Graph();

    // Constructor that reads the graph data from an input stream (e.g., a file or
    // stdin)
    Graph(std::istream &in);

    // Constructor to create a graph from another graph
    Graph(Graph *graph);

    // Function to read a DIMACS-format graph from an input stream
    void read_dimacs(std::istream &in);

    // Retrieves the forward edge from a given vertex and edge index in the
    // adjacency list
    Edge *get_forward(int source_vertex, int edge_index);

    // Retrieves the reverse edge corresponding to a given edge in the adjacency
    // list
    Edge *get_reverse(Edge edge);

    // Retrieves the outgoing edges of a given vertex
    std::vector<Edge> &get_outgoing_edges(int vertex);

    // A utility function to compute an upper bound for the flow that can be sent
    int compute_upper_flow_bound();

    // Retrieves the source vertex of the graph
    int get_source() const;

    // Retrieves the sink vertex of the graph
    int get_sink() const;

    // Retrieves the total number of vertices in the graph
    int get_total_vertices() const;

    // Retrieves the total number of arcs (edges) in the graph
    int get_total_arcs() const;

   protected:
    // Adjacency list to store the graph's edges: adjacency_list[vertex] contains
    // edges from that vertex
    std::vector<std::vector<Edge>> adjacency_list;

    // Number of vertices and edges (arcs) in the graph
    unsigned num_vertices_;
    unsigned num_arcs_;

    // The source and sink vertices used in flow algorithms (such as
    // Ford-Fulkerson)
    int source;
    int sink;

    // Helper function to resize the graph's adjacency list to accommodate a new
    // number of vertices
    void resize(int n);

    // Helper function to add a directed edge between two vertices with a
    // specified capacity
    void add_edge(int origin, int destiny, int capacity);
};

class TournamentGraph : public Graph
{
   public:
    // Constructor that reads tournament instance to build graph from stdin
    TournamentGraph(std::istream &in);

    // Builds a flow graph from tournament input to check if team 1 can still win.
    // Uses a max-flow reduction with the following vertex layout:
    // - Vertex 0: Source
    // - Vertices 1 to P: Pairing vertices (games between teams 2..n)
    // - Vertices P+1 to P+T: Team vertices (excluding team 1)
    // - Vertex P+T: Sink
    void build_tournament_graph(std::istream &in);

    // Retrieves if team one can win the tournament before computing maxflow
    bool team_one_can_win_before_flow() const;

    // Retrieves if team one can win the tournament after computing maxflow
    bool team_one_can_win_after_flow();

   private:
    // Tournament specific field for checking if the team 1 can't win
    bool team_one_cant_win;
};

#endif  // GRAPH_H
