#pragma once 
 
#include "geo.h" 
#include "svg.h" 
#include "json.h" 
#include "transport_catalogue.h"

class Request {
public:
    Request(json::Document input_json)
        : input_(input_json) {}

    const json::Node& GetBaseRequest() const;
    const json::Node& GetStatRequest() const;
    const json::Node& GetRenderSettings() const;
    const json::Node& GetRoutingSettings() const;
    const json::Node& GetSerializationSettings() const;

    void MakeCatalogue(transport_catalogue::TransportCatalogue& TransportCatalogue) const;

private:
    json::Document input_;
    json::Node dumm_{ nullptr };

    struct BusInfo {
        std::vector<std::string_view> stops;
        std::string_view final_stop;
        bool is_roundtrip;
    };

    using StopsDistMap = std::unordered_map<std::string_view, std::unordered_map<std::string_view, int>>;
    using BusesInfoMap = std::unordered_map<std::string_view, BusInfo>;

    void ParseStopAddRequest(transport_catalogue::TransportCatalogue& TransportCatalogue, const json::Dict& request_map, StopsDistMap& stop_to_stops_distance) const;
    void SetStopsDistances(transport_catalogue::TransportCatalogue& TransportCatalogue, const StopsDistMap& stop_to_stops_distance) const;
    void ParseBusAddRequest(const json::Dict& request_map, BusesInfoMap& buses_info) const;
    void BusesAddProcess(transport_catalogue::TransportCatalogue& TransportCatalogue, const BusesInfoMap& buses_info) const;
    void SetFinals(transport_catalogue::TransportCatalogue& TransportCatalogue, const BusesInfoMap& buses_info) const;
};