#include <filesystem>
#include <fstream>
#include <iomanip>
#include <map>

#include "ford_fulk.hpp"

int display_usage_tutorial(const char* program_name)
{
    std::cerr << "Usage:\n";
    std::cerr << "  1. Single Run Mode (reads tournament from stdin):\n";
    std::cerr << "     " << program_name << "\n\n";
    std::cerr << "  2. Benchmark Mode (reads tournaments from folder):\n";
    std::cerr << "     " << program_name << " <tourn_folder_path> <output_file>\n\n";

    std::cerr << "Arguments:\n";
    std::cerr << "  <tourn_folder_path>      Required for Benchmark Mode. Folder with tournament "
                 "files (.tourn).\n";
    std::cerr << "  <output_file_name>            Required for Benchmark Mode. File name to save "
                 "CSV results.\n\n";

    return -1;
}

int single_run_mode()
{
    TournamentGraph graph(std::cin);

    if (!graph.team_one_can_win_before_flow()) {
        std::cout << "não" << std::endl;
        return 0;
    }

    ford_fulkerson(graph, graph.get_source(), graph.get_sink(), Algorithm::FattestPath, false);

    if (graph.team_one_can_win_after_flow()) {
        std::cout << "sim" << std::endl;
    } else {
        std::cout << "não" << std::endl;
    }

    return 0;
}

int benchmark_mode(const char* folder_path, const char* output_name)
{
    // Prepare output path
    std::filesystem::path output_path = "./data/outputs/";
    output_path /= output_name;
    std::filesystem::create_directories(output_path.parent_path());

    std::ofstream output_file(output_path);
    if (!output_file.is_open())
        return -1;

    // Write CSV header
    output_file << "subdir,total_instances,team1_winnable_count,win_percentage, time(µs)\n";

    // Use directory_iterator instead of recursive if one level below is enough
    for (const auto& dir_entry : std::filesystem::directory_iterator(folder_path)) {
        if (!dir_entry.is_directory())
            continue;

        const auto& subdir_path = dir_entry.path();
        std::string subdir_name = std::filesystem::relative(subdir_path, folder_path).string();

        int total = 0;
        int winnable_count = 0;
        double total_duration = 0.0;

        for (const auto& file_entry : std::filesystem::directory_iterator(subdir_path)) {
            if (!file_entry.is_regular_file() || file_entry.path().extension() != ".tourn")
                continue;

            auto start = std::chrono::high_resolution_clock::now();

            std::ifstream input(file_entry.path());
            if (!input.is_open())
                continue;

            TournamentGraph graph(input);

            bool winnable = false;
            if (graph.team_one_can_win_before_flow()) {
                ford_fulkerson(graph, graph.get_source(), graph.get_sink(), Algorithm::FattestPath,
                               false);
                winnable = graph.team_one_can_win_after_flow();
            }

            auto end = std::chrono::high_resolution_clock::now();
            double duration =
                std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            ++total;
            if (winnable)
                ++winnable_count;
            total_duration += duration;
        }

        if (total == 0)
            continue;

        double win_percentage = 100.0 * winnable_count / total;
        double avg_duration = total_duration / total;

        output_file << subdir_name << "," << total << "," << winnable_count << "," << std::fixed
                    << std::setprecision(2) << win_percentage << "," << std::fixed
                    << std::setprecision(3) << avg_duration << "\n";
    }
    return 0;
}

int main(int argc, char const* argv[])
{
    if (argc != 1 && argc != 3)
        return display_usage_tutorial(argv[0]);

    if (argc == 1) {
        return single_run_mode();
    } else {
        const char* folder_path = argv[1];
        const char* output_name = argv[2];
        return benchmark_mode(folder_path, output_name);
    }
}
