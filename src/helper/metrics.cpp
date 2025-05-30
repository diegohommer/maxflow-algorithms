#include "metrics.hpp"

namespace Metrics
{

long long compute_max_iterations(Graph& graph, int source, Algorithm algo, int upper_limit = 0)
{
    switch (algo) {
        case Algorithm::EdmondsKarp: {
            const int n = graph.get_total_vertices();
            const int m = graph.get_total_arcs();
            return std::max(
                1LL, (static_cast<long long>(n) * m) / 2);  // Use long long for safe multiplication
        }
        case Algorithm::RandomizedDFS: {
            return std::max(1, upper_limit);
        }
        case Algorithm::FattestPath: {
            const int m = graph.get_total_arcs();
            return std::max(1LL, static_cast<long long>(m * std::log2(upper_limit)));
        }
        default:
            return 1;
    }
}

double compute_average(const std::vector<double>& elements, double total)
{
    if (total == 0.0)
        return 0.0;

    double sum = 0.0;
    for (double e : elements) {
        if (e < 0.0 || e > 1.0) {
            throw std::runtime_error("Average input value out of expected range [0,1]");
        }
        sum += e;
    }
    return sum / total;
}

CriticalArcStats compute_critical_arc_stats(Graph& graph)
{
    const int n = graph.get_total_vertices();
    const int m = graph.get_total_arcs();

    double critical_count = 0.0;
    double r_sum = 0.0;

    for (int i = 0; i < n; ++i) {
        for (const Edge& edge : graph.get_outgoing_edges(i)) {
            const double C_a = static_cast<double>(edge.num_criticals);
            const double r_a = C_a / (n / 2.0);

            if (r_a < 0.0 || r_a > 1.0) {
                throw std::runtime_error("Invalid r_a: out of [0, 1] range");
            }

            if (C_a > 0.0) {
                critical_count += 1.0;
                r_sum += r_a;
            }
        }
    }

    const double C_fraction = (m > 0) ? critical_count / m : 0.0;
    const double avg_r_bar = (critical_count > 0.0) ? r_sum / (critical_count * m) : 0.0;

    if (C_fraction < 0.0 || C_fraction > 1.0) {
        throw std::runtime_error("Invalid C: out of [0, 1] range");
    }

    return CriticalArcStats{C_fraction, avg_r_bar};
}

GraphMetrics compute_graph_metrics(Graph& graph, const Algorithm& algo, const FordResult& result)
{
    const int n = graph.get_total_vertices();
    const int m = graph.get_total_arcs();
    const int source = graph.get_source();

    const double max_iterations =
        static_cast<double>(compute_max_iterations(graph, source, algo, result.flow_upper_bound));
    const double r = static_cast<double>(result.iterations) / max_iterations;
    if (r < 0.0 || r > 1.0) {
        throw std::runtime_error("Iterations fractions (r) value out of expected range [0,1]");
    }

    const double time_ms = result.duration_ms;
    const double nor_time = time_ms / static_cast<double>(static_cast<long long>(n) * m * (n + m));

    const double avg_l = std::accumulate(result.stats.path_lengths_per_iter.begin(),
                                         result.stats.path_lengths_per_iter.end(), 0.0) /
                         result.iterations;

    GraphMetrics metrics;
    metrics.n = n;
    metrics.m = m;
    metrics.r = r;
    metrics.time_ms = time_ms;
    metrics.nor_time = nor_time;
    metrics.avg_l = avg_l;

    switch (algo) {
        case Algorithm::EdmondsKarp: {
            const CriticalArcStats crit_stats = compute_critical_arc_stats(graph);
            metrics.C = crit_stats.C;
            metrics.avg_crit = crit_stats.r_bar;
            metrics.avg_s = compute_average(result.stats.s_per_iter, result.iterations);
            metrics.avg_t = compute_average(result.stats.t_per_iter, result.iterations);
            break;
        }
        case Algorithm::RandomizedDFS: {
            metrics.avg_s = compute_average(result.stats.s_per_iter, result.iterations);
            metrics.avg_t = compute_average(result.stats.t_per_iter, result.iterations);
            break;
        }
        case Algorithm::FattestPath: {
            metrics.I = compute_average(result.stats.inserts_per_iter, result.iterations);
            metrics.D = compute_average(result.stats.deletemaxes_per_iter, result.iterations);
            metrics.U = compute_average(result.stats.updates_per_iter, result.iterations);
            break;
        }
        default:
            throw std::runtime_error("Unknown algorithm in compute_graph_metrics");
    }

    return metrics;
}

}  // namespace Metrics
