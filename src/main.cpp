#include "ford_fulk.hpp"

enum Algorithm {
    EDMONDS_KARP = 0,
    DFS = 1,
    FATTEST_PATH = 2
};

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <algorithm_index>\n";
        std::cerr << "Algorithms:\n";
        std::cerr << "  0 - Edmonds-Karp (BFS)\n";
        std::cerr << "  1 - DFS\n";
        std::cerr << "  2 - Fattest Path\n";
        return 1;
    }

    int algorithm_index = std::stoi(argv[1]);

    Graph graph(std::cin);
    int max_flow = 0;

    switch (algorithm_index) {
        case EDMONDS_KARP:
            max_flow = edmonds_karp(graph, graph.get_source(), graph.get_sink()).max_flow;
            break;
        case DFS:
            max_flow = randomized_ford_fulkerson(graph, graph.get_source(), graph.get_sink()).max_flow;
            break;
        case FATTEST_PATH:
            max_flow = fattest_path(graph, graph.get_source(), graph.get_sink()).max_flow;
            break;
        default:
            std::cerr << "Invalid algorithm index.\n";
            return 1;
    }

    std::cout << max_flow << std::endl;
    return 0;
}
