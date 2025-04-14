#ifndef LOGGER_H
#define LOGGER_H

#include "metrics.hpp"
#include <fstream>
#include <numeric>
#include <stdexcept>

namespace Logger {
    void log_stats_header(const Algorithm& algo, std::ofstream& output_file);
    void log_instance_stats(const GraphMetrics& metrics, const Algorithm& algo, std::ofstream& output_file);
}

#endif // LOGGER_H
