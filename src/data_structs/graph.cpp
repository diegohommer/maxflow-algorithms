#include "graph.hpp"

Graph::Graph(std::istream& in) {read_dimacs(in);};

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
            source--;
            break;
        }
    }

    // (2) find sink node (second "n " line)
    while (std::getline(in, line)) {
        if (line.substr(0, 2) == "n ") {
            linestr.clear();
            linestr.str(line);
            linestr >> dummy >> sink;
            sink--;
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

std::vector<Edge>& Graph::get_neighbors(int vertex) {
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

Edge* Graph::get_reverse(Edge edge){
    return &adjacency_list[edge.to][edge.reverse_idx];
}

void Graph::add_edge(int origin, int destiny, int capacity) {
    // Check if reverse edge already exists
    int reverse_idx = -1;
    for (int i = 0; i < adjacency_list[destiny].size(); ++i) {
        if (adjacency_list[destiny][i].to == origin) {
            reverse_idx = i;
            break;
        }
    }

    if (reverse_idx == -1) {
        // Add forward and reverse edges 
        adjacency_list[origin].emplace_back(destiny, capacity, 0, -1);
        adjacency_list[destiny].emplace_back(origin, 0, 0, -1);

        // Make their reverse indexes reference each other
        int forward_idx = adjacency_list[origin].size() - 1;
        reverse_idx = adjacency_list[destiny].size() - 1;

        adjacency_list[origin][forward_idx].reverse_idx = reverse_idx;
        adjacency_list[destiny][reverse_idx].reverse_idx = forward_idx;
    } else {
        // Update existing forward edge's capacity
        int forward_idx = adjacency_list[destiny][reverse_idx].reverse_idx;
        adjacency_list[origin][forward_idx].capacity += capacity;
    }
}

void Graph::resize(int n) {
    adjacency_list.resize(n);
}