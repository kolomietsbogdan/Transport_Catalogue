#pragma once 
 
#include <vector> 
#include <cstdlib> 
 
#include "ranges.h"

namespace graph {

    using VertexId = size_t;
    using EdgeId = size_t;

    template <typename Weight>
    struct Edge {
        std::string name;
        size_t quality;
        VertexId from;
        VertexId to;
        Weight weight;
    };

    template <typename Weight>
    class DirectedWeightedGraph {
    private:
        using IncidenceList = std::vector<EdgeId>;
        using IncidentEdgesRange = ranges::Range<typename IncidenceList::const_iterator>;

    public:
        DirectedWeightedGraph() = default;
        explicit DirectedWeightedGraph(size_t vertex_count);
        explicit DirectedWeightedGraph(std::vector<Edge<Weight>> edges,
            std::vector<std::vector<EdgeId>> incidence_lists);
        EdgeId AddEdge(Edge<Weight>&& edge);

        size_t GetVertexCount() const;
        size_t GetEdgeCount() const;
        const Edge<Weight>& GetEdge(EdgeId edge_id) const;
        IncidentEdgesRange GetIncidentEdges(VertexId vertex) const;

    private:
        std::vector<Edge<Weight>> edges_;
        std::vector<IncidenceList> incidence_lists_;
    };

    template <typename Weight>
    DirectedWeightedGraph<Weight>::DirectedWeightedGraph(size_t vertex_count)
        : incidence_lists_(vertex_count) {
        edges_.reserve(vertex_count * vertex_count);
    }

    template <typename Weight>
    DirectedWeightedGraph<Weight>::DirectedWeightedGraph(std::vector<Edge<Weight>> edges,
        std::vector<std::vector<EdgeId>> incidence_lists)
        : edges_(edges)
        , incidence_lists_(incidence_lists) {}

    template <typename Weight>
    EdgeId DirectedWeightedGraph<Weight>::AddEdge(Edge<Weight>&& edge) {
        edges_.push_back(std::move(edge));
        const EdgeId id = edges_.size() - 1;
        incidence_lists_.at(edges_.back().from).push_back(id);
        return id;
    }

    template <typename Weight>
    size_t DirectedWeightedGraph<Weight>::GetVertexCount() const {
        return incidence_lists_.size();
    }

    template <typename Weight>
    size_t DirectedWeightedGraph<Weight>::GetEdgeCount() const {
        return edges_.size();
    }

    template <typename Weight>
    const Edge<Weight>& DirectedWeightedGraph<Weight>::GetEdge(EdgeId edge_id) const {
        return edges_.at(edge_id);
    }

    template <typename Weight>
    typename DirectedWeightedGraph<Weight>::IncidentEdgesRange
        DirectedWeightedGraph<Weight>::GetIncidentEdges(VertexId vertex) const {
        return ranges::AsRange(incidence_lists_.at(vertex));
    }
}