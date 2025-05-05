#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <numeric>
#include <stdexcept>

#include "metrics.hpp"

namespace Logger
{
void log_stats_header(const Algorithm &algo, std::ofstream &output_file);
void log_instance_stats(const GraphMetrics &metrics, const Algorithm &algo,
                        std::ofstream &output_file);
}  // namespace Logger

#endif  // LOGGER_H
