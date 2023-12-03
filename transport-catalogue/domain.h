#pragma once

#include <set>
#include <string>
#include <vector>

#include "geo.h"

namespace transport_catalogue {
    namespace domain {
			struct Stop {
				std::string stop_name_ = {};
				geo::Coordinates coordinates_ = {};
				std::set<std::string_view> buses = {};
			};
			struct Bus {
				std::string bus_name_ = {};
				std::vector<Stop*> stops_ = {};
				bool is_roundtrip_ = false;
			};
    }
}
