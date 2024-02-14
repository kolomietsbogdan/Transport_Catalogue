#pragma once

#include "map_renderer.h"
#include "transport_router.h"

class RequestHandler {
public:
    RequestHandler(const transport_catalogue::TransportCatalogue& TransportCatalogue, const transport_catalogue::TransportRouter& router, const renderer::MapRenderer& renderer);

    void Handle(const json::Node& json_doc, std::ostream& output);

    svg::Document RenderMap() const;

private:
    const transport_catalogue::TransportCatalogue& db_;
    const transport_catalogue::TransportRouter& router_;
    const renderer::MapRenderer& renderer_;

    json::Node FindStopRequestProcessing(const json::Dict& request_map);
    json::Node FindBusRequestProcessing(const json::Dict& request_map);
    json::Node BuildMapRequestProcessing(const json::Dict& request_map);
    json::Node BuildRouteRequestProcessing(const json::Dict& request_map);
};