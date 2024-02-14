#pragma once  
  
#include "geo.h" 
#include "graph.h" 

namespace domain {

    struct Stop {
        Stop(const std::string& name, const geo::Coordinates& coordinates);
        
        int GetDistance(Stop* to);
        std::string name;
        geo::Coordinates coordinates;
        std::unordered_map<std::string_view, int> stop_distances;
    };

    struct Bus {
        Bus(const std::string& name, std::vector<Stop*> stops, bool is_roundtrip);

        std::string name;
        std::vector<Stop*> stops;
        bool is_roundtrip;
        Stop* final_stop = nullptr;
    };
}