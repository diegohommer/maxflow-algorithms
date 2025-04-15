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

std::vector<Edge>& Graph::get_outgoing_edges(int vertex) {
    return adjacency_list[vertex];
}

int Graph::compute_upper_flow_bound() {
    int src_limit = 0, sink_limit = 0;

    // Sum capacities of edges *leaving* the source
    for (const Edge& e : get_outgoing_edges(source)) {
        src_limit += e.capacity;
    }

    // Sum capacities of edges *entering* the sink (via reverse edges of outgoing edges from sink)
    for (const Edge& e : get_outgoing_edges(sink)) {
        sink_limit += adjacency_list[e.to][e.reverse_idx].capacity;
    }
    
    return std::min(src_limit, sink_limit);
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

Edge* Graph::get_forward(int source_vertex, int edge_index){
    return &adjacency_list[source_vertex][edge_index];
}

Edge* Graph::get_reverse(Edge edge){
    return &adjacency_list[edge.to][edge.reverse_idx];
}

void Graph::add_edge(int origin, int destiny, int capacity) {
    // Check if forward edge already exists
    int forward_idx = -1;
    for (int i = 0; i < adjacency_list[origin].size(); ++i) {
        if (adjacency_list[origin][i].to == destiny) {
            forward_idx = i;
            break;
        }
    }

    if (forward_idx == -1) {
        // Add new forward and reverse edges
        adjacency_list[origin].emplace_back(destiny, capacity, adjacency_list[destiny].size());
        adjacency_list[destiny].emplace_back(origin, 0, adjacency_list[origin].size() - 1);
        this->num_arcs_++;
    } else {
        // Update existing forward edge
        adjacency_list[origin][forward_idx].capacity += capacity;
    }
}

void Graph::resize(int n) {
    adjacency_list.resize(n);
}