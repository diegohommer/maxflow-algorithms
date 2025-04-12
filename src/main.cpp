#include "ford_fulk.hpp"
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

int single_run_mode(int algorithm_index) {
    Algorithm algo;

    switch (algorithm_index) {
        case 0: algo = Algorithm::EdmondsKarp; break;
        case 1: algo = Algorithm::RandomizedDFS; break;
        case 2: algo = Algorithm::FattestPath; break;
        default: return display_usage_tutorial("");  // Invalid algorithm index
    }

    Graph graph(std::cin);
    FordResult result = ford_fulkerson(graph, graph.get_source(), graph.get_sink(), algo);

    std::cout << result.max_flow << std::endl;
    return 0;
}

int benchmark_mode(char const* input_path, char const* output_name) {
    // Read all .graph files from the input folder into memory
    std::vector<Graph> graphs;
    for (const auto& entry : std::filesystem::directory_iterator(input_path)) {
        if (entry.is_regular_file() && entry.path().extension() == ".graph") {
            std::ifstream input(entry.path());
            Graph graph(input);
            graphs.push_back(std::move(graph));
        }
    }

    // Construct the output file path
    std::filesystem::path output_dir = "./data/outputs/";
    output_dir /= output_name; 
    std::filesystem::create_directories(output_dir.parent_path());

    // Setup output file
    std::ofstream output_file(output_dir);
    if (!output_file.is_open()) {
        std::cerr << "Error: Could not open output file " << output_dir << "\n";
        return 1;
    }

    // Write headers to the output file
    output_file << "Graph,Algorithm Result\n";

    return 0;
}

int main(int argc, char const *argv[]) {
    if (argc != 2 && argc != 4) {
        return display_usage_tutorial(argv[0]);
    }

    if (argc == 2){
        return single_run_mode(std::stoi(argv[1]));
    }
    
    if (argc == 4) {
        return benchmark_mode(argv[2], argv[3]);
    }

    return -1;
}
