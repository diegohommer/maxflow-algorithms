#include "ford_fulk.hpp"

int display_usage_tutorial(char const *program_name) {
    std::cerr << "Usage: " << program_name << " <algorithm_index> [optional_graphs_folder_path]\n\n";
    std::cerr << "Modes:\n";
    std::cerr << "  1. Single Run Mode (reads graph from stdin):\n";
    std::cerr << "     " << program_name << " <algorithm_index>\n\n";
    std::cerr << "  2. Benchmark Mode (reads graphs from folder):\n";
    std::cerr << "     " << program_name << " <algorithm_index> <graphs_folder_path>\n\n";
    std::cerr << "Arguments:\n";
    std::cerr << "  <algorithm_index>              Required. Choose one of the following:\n";
    std::cerr << "    0 - Edmonds-Karp (BFS)\n";
    std::cerr << "    1 - Randomized DFS\n";
    std::cerr << "    2 - Fattest Path (Modified Dijkstra)\n";
    std::cerr << "  [graphs_folder_path]           Optional. Path to a folder for benchmarking mode.\n\n";
    std::cerr << "Examples:\n";
    std::cerr << "  " << program_name << " 0                # Reads a graph from stdin\n";
    std::cerr << "  " << program_name << " 2 ./graphs/      # Runs benchmark mode on all graphs in folder\n";
    return 1;
}

int run_benchmark(int argc, char const *argv[]) {
    // Placeholder for benchmarking logic
    std::cerr << "Benchmarking not yet implemented.\n";
    return 0;
}

int main(int argc, char const *argv[]) {
    if (argc != 2 && argc != 3) {
        return display_usage_tutorial(argv[0]);
    }
    
    if (argc == 3) {
        return run_benchmark(argc, argv);
    }

    int algorithm_index = std::stoi(argv[1]);
    Algorithm algo;

    switch (algorithm_index) {
        case 0: algo = Algorithm::EdmondsKarp; break;
        case 1: algo = Algorithm::RandomizedDFS; break;
        case 2: algo = Algorithm::FattestPath; break;
        default: return display_usage_tutorial(argv[0]);
    }

    Graph graph(std::cin);
    FordResult result = ford_fulkerson(graph, graph.get_source(), graph.get_sink(), algo);

    std::cout << result.max_flow << std::endl;
    return 0;
}
