#pragma once

#include <concepts>
#include <ranges>

#include <map>
#include <set>

#include "challenge.h"

namespace challenge100 {

template <class Vertex = int, class Weight = double>
    requires std::totally_ordered<Vertex>
struct graph {
    using vertex_type = Vertex;
    using weight_type = Weight;
    using neighbor_type = std::pair<Vertex, Weight>;
    using neighbor_list_type = std::vector<neighbor_type>;

    void add_edge(Vertex const source, Vertex const target, Weight const weight, bool const bidirectional = true)
    {
        adjacency_list[source].emplace_back(target, weight);
        if (bidirectional)
            adjacency_list[target].emplace_back(source, weight);
    }

    size_t size() const {
        return adjacency_list.size();
    }
    auto vertices() const {
        return adjacency_list | std::views::keys;
    }
    auto neighbors(Vertex const& v) const {
        auto found = adjacency_list.find(v);
        return make_optional_if(found != std::ranges::end(adjacency_list),
                [&found]() { return std::ranges::ref_view(found->second); });
    }

    constexpr inline static Weight Infinity = std::numeric_limits<Weight>::infinity();

  private:
    std::map<vertex_type, neighbor_list_type> adjacency_list;
};

template <class Vertex, class Weight>
void shortest_path(graph<Vertex, Weight> const& g, Vertex const source,
        std::map<Vertex, Weight>& min_distance, std::map<Vertex, Vertex>& previous)
{
    min_distance.clear();
    std::ranges::for_each(g.vertices(), [&min_distance, &source](auto const& v) {
            min_distance.emplace(v, source == v ? Weight{0} : graph<Vertex, Weight>::Infinity);
            });

    previous.clear();
    std::set<std::pair<Weight, Vertex>> vertex_queue;
    vertex_queue.emplace(min_distance[source], source);

    while (!vertex_queue.empty()) {
        auto first = std::ranges::begin(vertex_queue);
        auto [dist, u] = *first;
        vertex_queue.erase(first);
        auto neighbors = g.neighbors(u);
        if (!neighbors)
            return;
        //auto const& neighbors = g.neighbors(u);
        for (auto const& [v, w] : *neighbors) {
            auto dist_via_u = dist + w;
            if (dist_via_u < min_distance[v]) {
                vertex_queue.erase(std::make_pair(min_distance[v], v));
                min_distance[v] = dist_via_u;
                previous[v] = u;
                vertex_queue.emplace(dist_via_u, v);
            }
        }
    }
}

template <class Vertex>
void build_path(std::map<Vertex, Vertex> const& prev, Vertex const v,
        std::vector<Vertex>& result) {
    result.push_back(v);
    auto pos = prev.find(v);
    if (pos == std::ranges::end(prev))
        return;

    build_path(prev, pos->second, result);
}

template <class Vertex>
std::vector<Vertex> build_path(std::map<Vertex, Vertex> const& prev, Vertex const v) {
    std::vector<Vertex> ret;
    build_path(prev, v, ret);
    std::reverse(std::ranges::begin(ret), std::ranges::end(ret));
    return ret;
}

template <class Vertex>
void print_path(std::vector<Vertex> const& path) {
    std::ranges::for_each(std::views::iota(0u, path.size()), [&path](auto const& i) {
            std::cout << path[i];
            if (i < path.size() - 1)
                std::cout << " -> ";
            });
}

} // ns: challenge100
