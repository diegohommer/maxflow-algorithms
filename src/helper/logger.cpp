#include "logger.hpp"
#include <iomanip>  // For std::setprecision

namespace Logger {

    void log_stats_header(const Algorithm& algo, std::ofstream& output_file) {
        output_file << "n,m,r,T(µs),normalized_T,avg_l,";
        switch (algo) {
            case Algorithm::EdmondsKarp:
                output_file << "C,avg_crit,avg_s,avg_t";
                break;
            case Algorithm::RandomizedDFS:
                output_file << "avg_s,avg_t";
                break;
            case Algorithm::FattestPath:
                output_file << "I,D,U";
                break;
            default:
                throw std::runtime_error("Unknown algorithm in log_stats_header");
        }
        output_file << std::endl;
    }

    void log_instance_stats(const GraphMetrics& stats, const Algorithm& algo, std::ofstream& output_file) {
        output_file << std::fixed << std::setprecision(12);

        output_file << stats.n << "," 
                    << stats.m << "," 
                    << stats.r << "," 
                    << stats.time_ms << ","
                    << stats.nor_time << ","
                    << stats.avg_l << ",";

        switch (algo) {
            case Algorithm::EdmondsKarp: {
                output_file << stats.C << ","
                            << stats.avg_crit << ","
                            << stats.avg_s << ","
                            << stats.avg_t;
                break;
            }
            case Algorithm::RandomizedDFS: {
                output_file << stats.avg_s << "," 
                            << stats.avg_t;
                break;
            }
            case Algorithm::FattestPath:
                output_file << stats.I << "," 
                            << stats.D << ","
                            << stats.U;
                break;
            default:
                throw std::runtime_error("Unknown algorithm in log_instance_stats");
        }

        output_file << std::endl;
    }

}
