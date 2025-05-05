#include "graph.hpp"

Graph::Graph(){};

Graph::Graph(std::istream& in) { read_dimacs(in); };

// Copy constructor for the Graph class
Graph::Graph(Graph* graph)
{
    // Copy the basic attributes
    this->num_vertices_ = graph->num_vertices_;
    this->num_arcs_ = graph->num_arcs_;
    this->source = graph->source;
    this->sink = graph->sink;

    // Create a deep copy of the adjacency list
    this->adjacency_list.resize(graph->adjacency_list.size());
    for (size_t i = 0; i < graph->adjacency_list.size(); ++i) {
        for (const Edge& edge : graph->adjacency_list[i]) {
            this->adjacency_list[i].emplace_back(edge.to, edge.capacity, edge.reverse_idx);
        }
    }
}

void Graph::read_dimacs(std::istream& in)
{
    std::string line = "", dummy;
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
    unsigned i = 0;
    // (3) process and store arcs (all "a " lines)
    while (getline(in, line) && i < num_arcs_) {
        if (line.substr(0, 2) == "a ") {
            std::stringstream arc(line);
            unsigned u, v, w;
            char ac;
            arc >> ac >> u >> v >> w;
            // process arc u-v with capacity w
            add_edge(u - 1, v - 1, w);
            ++i;
        }
    }

    // std::cout << get_source() << " "
    //           << get_sink() << " "
    //           << get_total_vertices() << " "
    //           << get_total_arcs() << std::endl;
}

std::vector<Edge>& Graph::get_outgoing_edges(int vertex) { return adjacency_list[vertex]; }

int Graph::compute_upper_flow_bound()
{
    int src_limit = 0, sink_limit = 0;

    // Sum capacities of edges *leaving* the source
    for (const Edge& e : get_outgoing_edges(source)) {
        src_limit += e.capacity;
    }

    // Sum capacities of edges *entering* the sink (via reverse edges of outgoing
    // edges from sink)
    for (const Edge& e : get_outgoing_edges(sink)) {
        sink_limit += adjacency_list[e.to][e.reverse_idx].capacity;
    }

    return std::min(src_limit, sink_limit);
}

int Graph::get_source() const { return this->source; }

int Graph::get_sink() const { return this->sink; }

int Graph::get_total_vertices() const { return this->num_vertices_; }

int Graph::get_total_arcs() const { return this->num_arcs_; }

Edge* Graph::get_forward(int source_vertex, int edge_index)
{
    return &adjacency_list[source_vertex][edge_index];
}

Edge* Graph::get_reverse(Edge edge) { return &adjacency_list[edge.to][edge.reverse_idx]; }

void Graph::add_edge(int origin, int destiny, int capacity)
{
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

void Graph::resize(int n) { adjacency_list.resize(n); }

TournamentGraph::TournamentGraph(std::istream& in) { build_tournament_graph(in); }

void TournamentGraph::build_tournament_graph(std::istream& in)
{
    const int INF = std::numeric_limits<int>::max();
    std::string line;
    std::stringstream linestr;
    std::vector<int> wins;
    int total_teams, remaining_games, team1_wins;

    // (1) Get total number of teams
    std::getline(in, line);
    linestr.str(line);
    linestr >> total_teams;
    wins.resize(total_teams);

    // (2) Compute total number of vertices of the reduction graph
    int pairings_between_other_teams = ((total_teams - 1) * (total_teams - 2)) / 2;
    int team_vertices = total_teams - 1;
    int source_and_sink = 2;
    num_vertices_ = pairings_between_other_teams + team_vertices + source_and_sink;
    resize(num_vertices_);
    source = 0;
    sink = num_vertices_ - 1;

    // (3) Get how many wins each team currently has
    std::getline(in, line);
    linestr.clear();
    linestr.str(line);
    for (int i = 0; i < total_teams; ++i) {
        linestr >> wins[i];
    }

    // (4) Get the remaining games to be played by team 1 and
    //     compute the maximum number of wins they can achieve
    std::getline(in, line);
    linestr.clear();
    linestr.str(line);
    for (int j = 0; j < total_teams; ++j) {
        int remaining_games;
        linestr >> remaining_games;
        wins[0] += remaining_games;
    }

    // (5) Process games between teams [2..total_teams] to build the reduction graph
    team_one_cant_win = false;
    int team_vertex_index = pairings_between_other_teams - 1;
    int pairing_vertex_index = 0;

    for (int i = 1; i < total_teams; ++i) {
        std::getline(in, line);
        linestr.clear();
        linestr.str(line);

        ++team_vertex_index;
        int max_allowed_wins_i = wins[0] - wins[i] - 1;
        if (max_allowed_wins_i < 0) {
            team_one_cant_win = true;
            return;
        }

        // Adding edge from team i to sink (num_vertices - 1)
        this->add_edge(team_vertex_index, sink, max_allowed_wins_i);

        for (int j = i + 1; j < total_teams; ++j) {
            int opponent_vertex = team_vertex_index + j - i;
            int remaining_games;
            linestr >> remaining_games;

            // Connect pairing vertex to source and participating teams
            ++pairing_vertex_index;
            this->add_edge(source, pairing_vertex_index, remaining_games);
            this->add_edge(pairing_vertex_index, team_vertex_index, INF);
            this->add_edge(pairing_vertex_index, opponent_vertex, INF);
        }
    }
}

bool TournamentGraph::team_one_can_win_before_flow() const { return !team_one_cant_win; }

bool TournamentGraph::team_one_can_win_after_flow()
{
    // Sum capacities of edges *leaving* the source
    for (const Edge& e : get_outgoing_edges(get_source())) {
        if (e.capacity > 0) {
            return false;
        }
    }
    return true;
}