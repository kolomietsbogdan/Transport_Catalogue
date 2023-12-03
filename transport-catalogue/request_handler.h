#pragma once

#include "json_reader.h"
#include "json_builder.h"

class RequestHandler { 
public:
    void Handle(transport_catalogue::catalogue::TransportCatalogue& catalogue, const std::vector<StatRequest>& requests, const std::string& map_to_string); 
}; 
