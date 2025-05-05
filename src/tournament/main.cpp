#include <filesystem>
#include <fstream>

#include "ford_fulk.hpp"

int display_usage_tutorial(const char* program_name)
{
    std::cerr << "Usage:\n";
    std::cerr << "  1. Single Run Mode (reads tournament from stdin):\n";
    std::cerr << "     " << program_name << " <algorithm_index>\n\n";
    std::cerr << "  2. Benchmark Mode (reads tournaments from folder):\n";
    std::cerr << "     " << program_name
              << " <algorithm_index> <tourn_folder_path> <output_file>\n\n";

    std::cerr << "Arguments:\n";
    std::cerr << "  <algorithm_index>        Required. Choose one of the following:\n";
    std::cerr << "                           0 - Edmonds-Karp (BFS)\n";
    std::cerr << "                           1 - Randomized DFS\n";
    std::cerr << "                           2 - Fattest Path (Modified Dijkstra)\n";
    std::cerr << "  <tourn_folder_path>      Required for Benchmark Mode. Folder with tournament "
                 "files (.tourn).\n";
    std::cerr << "  <output_file_name>            Required for Benchmark Mode. File name to save "
                 "CSV results.\n\n";

    return -1;
}

bool team_one_can_win(Graph& graph)
{
    // Sum capacities of edges *leaving* the source
    for (const Edge& e : graph.get_outgoing_edges(graph.get_source())) {
        if (e.capacity > 0) {
            return false;
        }
    }
    return true;
}

int single_run_mode(Algorithm algo)
{
    Graph graph(std::cin, GraphInputFormat::Tournament);
    FordResult result = ford_fulkerson(graph, graph.get_source(), graph.get_sink(), algo, false);
    if (team_one_can_win(graph)) {
        std::cout << "sim" << std::endl;
    } else {
        std::cout << "não" << std::endl;
    }
    return 0;
}

int benchmark_mode(Algorithm algo, const char* folder_path, const char* output_name)
{
    std::vector<Graph> graphs;

    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
        if (entry.is_regular_file() && entry.path().extension() == ".tourn") {
            std::ifstream input(entry.path());
            if (!input.is_open())
                continue;

            Graph graph(std::cin, GraphInputFormat::Tournament);
            graphs.push_back(std::move(graph));
        }
    }

    std::sort(graphs.begin(), graphs.end(), [](const Graph& a, const Graph& b) {
        if (a.get_total_vertices() == b.get_total_vertices())
            return a.get_total_arcs() < b.get_total_arcs();
        return a.get_total_vertices() < b.get_total_vertices();
    });

    std::filesystem::path output_path = "./data/outputs/";
    output_path /= output_name;
    std::filesystem::create_directories(output_path.parent_path());

    std::ofstream output_file(output_path);
    if (!output_file.is_open())
        return -1;

    for (auto& graph : graphs) {
    }

    return 0;
}

int main(int argc, char const* argv[])
{
    if (argc != 2 && argc != 4)
        return display_usage_tutorial(argv[0]);

    int algo_index = std::atoi(argv[1]);
    if (algo_index < 0 || algo_index > 2)
        return display_usage_tutorial(argv[0]);

    Algorithm algo = static_cast<Algorithm>(algo_index);

    if (argc == 2) {
        return single_run_mode(algo);
    } else {
        const char* folder_path = argv[2];
        const char* output_name = argv[3];
        return benchmark_mode(algo, folder_path, output_name);
    }
}
