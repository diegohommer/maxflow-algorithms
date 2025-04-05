#include "ford_fulk.hpp"

int main(int argc, char const *argv[])
{
    Graph graph(std::cin);

    // std::cout << edmonds_karp(graph, graph.get_source(), graph.get_sink()).max_flow << std::endl;
    std::cout << edmonds_karp(graph, graph.get_source(), graph.get_sink()).max_flow << std::endl;

    return 0;
}
