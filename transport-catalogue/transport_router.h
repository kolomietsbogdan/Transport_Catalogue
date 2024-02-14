#pragma once

#include "json.h"
#include "router.h"
#include "json_builder.h"
#include "transport_catalogue.h"

namespace transport_catalogue {

    class TransportRouter {
    public:
        TransportRouter() = default;

        TransportRouter(const json::Node& settings_node);
        TransportRouter(const json::Node& settings_node, const TransportCatalogue& catalogue);
        TransportRouter(const json::Node& settings_node, graph::DirectedWeightedGraph<double> graph, std::map<std::string, graph::VertexId> stop_ids);

        void SetGraph(graph::DirectedWeightedGraph<double>&& graph, std::map<std::string, graph::VertexId>&& stop_ids);
        const graph::DirectedWeightedGraph<double>& BuildGraph(const TransportCatalogue& catalogue);

        json::Array GetEdgesItems(const std::vector<graph::EdgeId>& edges) const;
        std::optional<graph::Router<double>::RouteInfo> GetRouteInfo(const Stop* from, const Stop* to) const;

        size_t GetGraphVertexCount();

        const std::map<std::string, graph::VertexId>& GetStopIds() const;
        const graph::DirectedWeightedGraph<double>& GetGraph() const;

        json::Node GetSettings() const;

        ~TransportRouter() {
            delete router_ptr_;
        }

    private:
        int bus_wait_time_ = 0;
        double bus_velocity_ = 0;

        graph::DirectedWeightedGraph<double> graph_;
        std::map<std::string, graph::VertexId> stop_ids_;

        graph::Router<double>* router_ptr_ = nullptr;

        void SetSettings(const json::Node& settings_node);
    };
}