#include "ford_fulk.hpp"
#include "logger.hpp"
#include <fstream>
#include <filesystem>

int display_usage_tutorial(char const *program_name) {
    std::cerr << "Usage:\n";
    std::cerr << "  1. Single Run Mode (reads graph from stdin):\n";
    std::cerr << "     " << program_name << " <algorithm_index>\n\n";
    std::cerr << "  2. Benchmark Mode (reads graphs from folder):\n";
    std::cerr << "     " << program_name << " <algorithm_index> <graphs_folder_path> <output_file_name>\n\n";
    
    std::cerr << "Arguments:\n";
    std::cerr << "  <algorithm_index>        Required. Choose one of the following:\n";
    std::cerr << "                           0 - Edmonds-Karp (BFS)\n";
    std::cerr << "                           1 - Randomized DFS\n";
    std::cerr << "                           2 - Fattest Path (Modified Dijkstra)\n";
    std::cerr << "  <graphs_folder_path>     Required for Benchmark Mode. Path to folder containing graph files.\n";
    std::cerr << "  <output_file_name>       Required for Benchmark Mode. Name of the output file to save benchmark results (e.g., results.csv).\n\n";
    
    std::cerr << "Examples:\n";
    std::cerr << "  " << program_name << " 0\n";
    std::cerr << "      # Run Edmonds-Karp on a single graph from stdin\n";
    std::cerr << "  " << program_name << " 2 ./graphs/ results.csv\n";
    std::cerr << "      # Run Fattest Path on all graphs in ./graphs/ and write results to results.csv\n";
    return -1;
}

int single_run_mode(Algorithm algo) {
    Graph graph(std::cin);
    FordResult result = ford_fulkerson(graph, graph.get_source(), graph.get_sink(), algo, false);

    std::cout << result.max_flow << std::endl;
    return 0;
}

int benchmark_mode(Algorithm algo, const char* input_path, const char* output_name) {
    // Read all .graph files from the input folder into memory
    std::vector<Graph> graphs;
    for (const auto& entry : std::filesystem::directory_iterator(input_path)) {
        if (entry.is_regular_file() && entry.path().extension() == ".graph") {
            std::ifstream input(entry.path());
            if (!input.is_open()) {
                continue;  // Skip the current file if it can't be opened
            }
            Graph graph(input);
            graphs.push_back(std::move(graph));
        }
    }

    // Construct the output file path
    std::filesystem::path output_dir = "./data/outputs/";
    output_dir /= output_name;
    std::filesystem::create_directories(output_dir.parent_path());  // Ensure the output directory exists

    // Setup output file
    std::ofstream output_file(output_dir);
    if (!output_file.is_open()) {
        return -1;  // Return an error if output file can't be created
    }

    Logger::log_stats_header(algo, output_file); 
    
    // Run and log stats of each graph over the selected algorithm
    for (auto& graph : graphs) {
        FordResult result = ford_fulkerson(graph, graph.get_source(), graph.get_sink(), algo, true);
        GraphMetrics metrics = Metrics::compute_graph_metrics(graph, algo, result);
        Logger::log_instance_stats(metrics, algo, output_file);
    }

    return 0; 
}

int main(int argc, char const *argv[]) {
    if (argc != 2 && argc != 4) {
        return display_usage_tutorial(argv[0]);
    }

    Algorithm algo;
    switch (std::stoi(argv[1])) {
        case 0: algo = Algorithm::EdmondsKarp; break;
        case 1: algo = Algorithm::RandomizedDFS; break;
        case 2: algo = Algorithm::FattestPath; break;
        default: return display_usage_tutorial(argv[0]); 
    }

    if (argc == 2){
        return single_run_mode(algo);
    }
    
    if (argc == 4) {
        return benchmark_mode(algo, argv[2], argv[3]);
    }

    return -1;
}
