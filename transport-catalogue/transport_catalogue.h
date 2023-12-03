#pragma once 
 
#include <set> 
#include <deque> 
#include <string> 
#include <vector> 
#include <algorithm> 
#include <unordered_map> 
#include <unordered_set> 
 
#include "geo.h" 
#include "domain.h" 
 
namespace transport_catalogue { 
	namespace catalogue { 
		class TransportCatalogue { 
		public: 
			struct BusInfo { 
				int stops_amount = 0; 
				int unique_stops = 0; 
				int route_length = 0; 
				double curvature = 0.0; 
			}; 
 
			struct StopsHasher { 
				size_t operator() (const std::pair<domain::Stop*, domain::Stop*>& stops) const { 
					size_t hash = std::hash<const void*>{}(stops.first); 
					hash = hash * 37 + std::hash<const void*>{}(stops.second); 
					return hash; 
				} 
			}; 
 
			void AddStop(const std::string& stop_name, const geo::Coordinates& coordinates); 
			void AddBus(const std::string& bus_name, const std::vector<domain::Stop*>& stops, bool is_roundtrip); 
			void AddDistanceBetweenStops(const std::string& stop_a, const std::string& stop_b, const int& distance); 
 
			domain::Stop* FindStop(const std::string& stop_name); 
			domain::Bus* FindBus(const std::string& bus_name); 
 
			int CalculatedDistance(const std::pair<domain::Stop*, domain::Stop*>& couple_stops); 
 
			BusInfo GetBusInfo(const std::string& bus_name); 
 
			std::set<std::string_view> GetStopInfo(const std::string& stop_name); 
			std::vector<domain::Bus*> GetSortedBusesByName(); 
			std::vector<domain::Stop*> GetSortedStopsByName(); 
			std::vector<geo::Coordinates> CombineStopCoordinates() const; 
 
		private: 
			std::deque<domain::Stop> stops_; 
			std::unordered_map<std::string_view, domain::Stop*> index_stops_; 
			std::deque<domain::Bus> buses_; 
			std::unordered_map<std::string_view, domain::Bus*> index_buses_; 
			std::unordered_map<std::pair<domain::Stop*, domain::Stop*>, int, StopsHasher> distance_; 
		}; 
	} 
}
