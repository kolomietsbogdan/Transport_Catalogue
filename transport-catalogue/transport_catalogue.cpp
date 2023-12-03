#include "transport_catalogue.h" 
 
namespace transport_catalogue { 
	namespace catalogue { 
		void TransportCatalogue::AddStop(const std::string& stop_name, const geo::Coordinates& coordinates) { 
			stops_.push_back({ stop_name, coordinates }); 
			index_stops_[stops_.back().stop_name_] = &stops_.back(); 
		} 
 
		void TransportCatalogue::AddBus(const std::string& bus_name, const std::vector<transport_catalogue::domain::Stop*>& stops, bool is_roundtrip) { 
			buses_.push_back({ bus_name, stops, is_roundtrip }); 
			index_buses_[buses_.back().bus_name_] = &buses_.back(); 
		} 
 
		void TransportCatalogue::AddDistanceBetweenStops(const std::string& stop_a, const std::string& stop_b, const int& distance) { 
			distance_[{FindStop(stop_a), FindStop(stop_b)}] = distance; 
		} 
 
		transport_catalogue::domain::Stop* TransportCatalogue::FindStop(const std::string& stop_name) { 
			if (index_stops_.find(stop_name) == index_stops_.end()) { 
				return nullptr; 
			} else { 
				return index_stops_[stop_name]; 
			} 
		} 
 
		transport_catalogue::domain::Bus* TransportCatalogue::FindBus(const std::string& bus_name) { 
			if (index_buses_.find(bus_name) == index_buses_.end()) { 
				return nullptr; 
			} else { 
				return index_buses_[bus_name]; 
			} 
		} 
 
		int TransportCatalogue::CalculatedDistance(const std::pair<transport_catalogue::domain::Stop*, transport_catalogue::domain::Stop*>& couple_stops) { 
			if (distance_.find(couple_stops) == distance_.end()) { 
				return distance_[{ couple_stops.second, couple_stops.first}]; 
			} else { 
				return distance_[couple_stops]; 
			} 
		} 
 
		TransportCatalogue::BusInfo TransportCatalogue::GetBusInfo(const std::string& bus_name) { 
			int stops_amount = static_cast<int>(FindBus(bus_name)->stops_.size()); 
			std::unordered_set<std::string_view> unique_stops; 
			unique_stops.insert(FindBus(bus_name)->stops_[stops_amount - 1]->stop_name_); 
 
			int route_length = 0; 
			double filler_length = 0.0; 
 
			for (int i = 0; i < stops_amount - 1; ++i) { 
				auto stop_a = FindBus(bus_name)->stops_[i]; 
				auto stop_b = FindBus(bus_name)->stops_[i + 1]; 
				filler_length += ComputeDistance(stop_a->coordinates_, stop_b->coordinates_); 
 
				route_length += CalculatedDistance({ stop_a, stop_b }); 
				unique_stops.insert(FindBus(bus_name)->stops_[i]->stop_name_); 
			} 
 
			if (stops_amount % 2 == 1) { 
				bool symmetrical = true; 
				auto start = FindBus(bus_name)->stops_.begin(); 
				auto finish = FindBus(bus_name)->stops_.end() - 1; 
 
				while (start < finish) { 
					if (*start != *finish) { 
						symmetrical = false; 
					} 
					++start; 
					--finish; 
				} 
				if (symmetrical) { 
					auto symmetrical_id = FindBus(bus_name)->stops_[stops_amount / 2]; 
					if (distance_.find({ symmetrical_id, symmetrical_id }) != distance_.end()) { 
						route_length += CalculatedDistance({ symmetrical_id, symmetrical_id }); 
					} 
				} 
			} 
			double curvature = route_length / filler_length; 
 
			return { stops_amount, static_cast<int>(unique_stops.size()), route_length, curvature }; 
		} 
 
		std::set<std::string_view> TransportCatalogue::GetStopInfo(const std::string& stop_name) { 
			return FindStop(stop_name)->buses; 
		} 
 
		std::vector<transport_catalogue::domain::Bus*> TransportCatalogue::GetSortedBusesByName() { 
			std::vector<transport_catalogue::domain::Bus*> sorted_buses; 
			sorted_buses.reserve(index_buses_.size()); 
 
			for (const auto& busPair : index_buses_) { 
				sorted_buses.push_back(busPair.second); 
			} 
			std::sort(sorted_buses.begin(), sorted_buses.end(), [](const transport_catalogue::domain::Bus* a, const transport_catalogue::domain::Bus* b) { 
					return a->bus_name_ < b->bus_name_; 
				}); 
 
			return sorted_buses; 
		} 
 
		std::vector<transport_catalogue::domain::Stop*> TransportCatalogue::GetSortedStopsByName() { 
			std::vector<transport_catalogue::domain::Stop*> sorted_stops; 
			sorted_stops.reserve(index_stops_.size()); 
 
			for (const auto& stopPair : index_stops_) { 
				sorted_stops.push_back(stopPair.second); 
			} 
			std::sort(sorted_stops.begin(), sorted_stops.end(), [](const transport_catalogue::domain::Stop* a, const transport_catalogue::domain::Stop* b) { 
					return a->stop_name_ < b->stop_name_; 
				}); 
 
			return sorted_stops; 
		} 
 
 
		std::vector<geo::Coordinates> TransportCatalogue::CombineStopCoordinates() const { 
			std::vector<geo::Coordinates> all_coordinates; 
			all_coordinates.reserve(stops_.size()); 
 
			for (const auto& stop : stops_) { 
				if (static_cast<int>(stop.buses.size()) != 0) { 
					all_coordinates.push_back(stop.coordinates_); 
				} 
			} 
			all_coordinates.resize(all_coordinates.size()); 
			return all_coordinates; 
		} 
	} 
}
