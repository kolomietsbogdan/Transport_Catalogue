#pragma once

#include <fstream>

#include "map_renderer.h"
#include "transport_router.h"
#include <transport_catalogue.pb.h>

void Serialize(const transport_catalogue::TransportCatalogue& catalogue, const renderer::MapRenderer& renderer, const transport_catalogue::TransportRouter& router, std::ostream& output);

serialize::Stop Serialize(const transport_catalogue::Stop* stop);
serialize::Bus Serialize(const transport_catalogue::Bus* bus);

serialize::RenderSettings GetRenderSettingSerialize(const json::Node& render_settings);
serialize::RouterSettings GetRouterSettingSerialize(const json::Node& router_settings);

serialize::Router Serialize(const transport_catalogue::TransportRouter& router);

std::tuple<transport_catalogue::TransportCatalogue, renderer::MapRenderer, transport_catalogue::TransportRouter, graph::DirectedWeightedGraph<double>, std::map<std::string, graph::VertexId>> Deserialize(std::istream& input);