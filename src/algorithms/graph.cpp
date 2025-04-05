#include "graph.hpp"

Graph::Graph() {};
Graph::Graph(int n, int m) : num_vertices_(n), num_arcs_(m) {resize(n);};

void Graph::read_dimacs(std::istream& in) {
    std::string line="", dummy;
    std::stringstream linestr;

    // (0) find line starting with "p max"
    while (std::getline(in, line)) {
        if (line.substr(0, 5) == "p max") {
            linestr.clear();
            linestr.str(line);
            linestr >> dummy >> dummy >> num_vertices_ >> num_arcs_;
            break;
        }
    }

    // (1) find source node (first "n " line)
    while (std::getline(in, line)) {
        if (line.substr(0, 2) == "n ") {
            linestr.clear();
            linestr.str(line);
            linestr >> dummy >> source;
            break;
        }
    }

    // (2) find sink node (second "n " line)
    while (std::getline(in, line)) {
        if (line.substr(0, 2) == "n ") {
            linestr.clear();
            linestr.str(line);
            linestr >> dummy >> sink;
            break;
        }
    }

    resize(num_vertices_);
    unsigned i=0;
    // (3) process and store arcs (all "a " lines)
    while (getline(in,line) && i < num_arcs_) {
        if (line.substr(0,2) == "a ") {
            std::stringstream arc(line);
            unsigned u,v,w;
            char ac;
            arc >> ac >> u >> v >> w;
            // process arc u-v with capacity w
            add_edge(u-1, v-1, w);
            ++i;
        }
    }

    // std::cout << get_source() << " " 
    //           << get_sink() << " " 
    //           << get_total_vertices() << " "
    //           << get_total_arcs() << std::endl;
  }

std::vector<Edge>& Graph::get_neighbours(int vertex) {
    return adjacency_list[vertex];
}

int Graph::get_source() const{
    return this->source;
}

int Graph::get_sink() const{
    return this->sink;
}

int Graph::get_total_vertices() const {
    return this->num_vertices_;
}

int Graph::get_total_arcs() const {
    return this->num_arcs_;
}

void Graph::add_edge(int origin, int destiny, int capacity) {
    // Check if reverse edge already exists
    Edge* existing_reverse = nullptr;
    for (Edge& e : adjacency_list[destiny]) {
        if (e.to == origin) {
            existing_reverse = &e;
            break;
        }
    }

    // Add forward edge
    adjacency_list[origin].push_back({destiny, capacity, 0, nullptr});
    Edge& edge = adjacency_list[origin].back();

    if (existing_reverse) {
        // Reuse existing reverse edge
        edge.reverse = existing_reverse;
        existing_reverse->reverse = &edge;
    } else {
        // Create new reverse edge
        adjacency_list[destiny].push_back({origin, 0, 0, nullptr});
        Edge& reverse_edge = adjacency_list[destiny].back();

        edge.reverse = &reverse_edge;
        reverse_edge.reverse = &edge;
    }
}

void Graph::resize(int n) {
    adjacency_list.resize(n);
}