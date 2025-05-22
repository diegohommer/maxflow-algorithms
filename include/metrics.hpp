#ifndef METRICS_HPP
#define METRICS_HPP

#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdexcept>

#include "ford_fulk.hpp"

// Structure that holds all the metrics related to the performance of the graph
// and the algorithm.
struct GraphMetrics {
    int n;            // Number of vertices in the graph
    int m;            // Number of arcs in the graph
    double r;         // Ratio of iterations to maximum iterations (efficiency)
    double time_ms;   // Duration of the algorithm execution in milliseconds
    double nor_time;  // Normalized duration of the algorithm (based on worst case
                      // complexity)
    double avg_l;     // Average path length (in terms of number of edges) per iteration
    double avg_s;     // Average fraction of vertices visited per iteration (s_i)
    double avg_t;     // Average fraction of arcs visited per iteration (t_i)
    double C;         // Fraction of critical arcs (Edmonds-Karp)
    double avg_crit;  // Average critical arc ratio (Edmonds-Karp)
    double I;         // Average number of inserts (Fattest Path)
    double D;         // Average number of deletemaxes (Fattest Path)
    double U;         // Average number of updates (Fattest Path)

    // Function to incrementally accumulate metrics
    void increment(const GraphMetrics &other)
    {
        n += other.n;
        m += other.m;
        r += other.r;
        time_ms += other.time_ms;
        nor_time += other.nor_time;
        avg_l += other.avg_l;
        avg_s += other.avg_s;
        avg_t += other.avg_t;
        C += other.C;
        avg_crit += other.avg_crit;
        I += other.I;
        D += other.D;
        U += other.U;
    }

    // Function to average the metrics by dividing by the number of runs
    void average(int num_runs)
    {
        if (num_runs > 0) {
            n /= num_runs;
            m /= num_runs;
            r /= num_runs;
            time_ms /= num_runs;
            nor_time /= num_runs;
            avg_l /= num_runs;
            avg_s /= num_runs;
            avg_t /= num_runs;
            C /= num_runs;
            avg_crit /= num_runs;
            I /= num_runs;
            D /= num_runs;
            U /= num_runs;
        }
    }
};

// Structure to hold the critical arc statistics of the graph
struct CriticalArcStats {
    double C;      // Fraction of arcs that were critical at least once
    double r_bar;  // Average criticality among those arcs
};

namespace Metrics
{

// Function to compute the theoretical maximum number of iterations for the
// given algorithm.
long long compute_max_iterations(Graph &graph, int source, Algorithm algo, int upper_limit);

// Function to compute the average value of elements in the range [0,1]
// This is typically used for computing averages like average fraction of
// visited vertices/arcs
double compute_average(const std::vector<double> &elements, double total);

// Function to compute critical arc statistics, specific to the Edmonds-Karp
// algorithm Returns the fraction of critical arcs and their average criticality
CriticalArcStats compute_critical_arc_stats(Graph &graph);

// Function to compute all the metrics for a given graph and algorithm execution
// This function gathers the metrics into a GraphMetrics struct
GraphMetrics compute_graph_metrics(Graph &graph, const Algorithm &algo, const FordResult &result);

}  // namespace Metrics

#endif  // METRICS_H
