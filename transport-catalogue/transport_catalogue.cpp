#include "transport_catalogue.h"

namespace transport_catalogue {

    using namespace std::literals;

    void TransportCatalogue::AddStop(const std::string& name, const geo::Coordinates& coordinates) {
        all_stops_.push_back(Stop(name, coordinates));
        Stop* added_stop = &all_stops_.back();
        stop_to_buses_[added_stop->name];
        stops_list_[added_stop->name] = added_stop;
    }

    void TransportCatalogue::AddBus(const std::string& name, const std::vector<Stop*>& stops, bool is_roundtrip) {
        all_buses_.push_back(Bus(name, stops, is_roundtrip));
        Bus* added_bus = &all_buses_.back();
        for (const Stop* s : stops) {
            stop_to_buses_[s->name][added_bus->name] = added_bus;
        }
        buses_list_[added_bus->name] = added_bus;
    }

    Stop* TransportCatalogue::FindStop(const std::string_view stop) {
        return stops_list_.count(stop) ? stops_list_.at(stop) : nullptr;
    }

    const Stop* TransportCatalogue::FindStop(const std::string_view stop) const {
        return stops_list_.count(stop) ? stops_list_.at(stop) : nullptr;
    }

    Bus* TransportCatalogue::FindBus(const std::string_view bus_name) {
        return buses_list_.count(bus_name) ? buses_list_.at(bus_name) : nullptr;
    }

    const Bus* TransportCatalogue::FindBus(const std::string_view bus_name) const {
        return buses_list_.count(bus_name) ? buses_list_.at(bus_name) : nullptr;
    }

    std::map<std::string_view, Bus*> TransportCatalogue::GetBusesOnStop(const std::string_view stop_name) {
        return stop_to_buses_.at(stop_name);
    }

    const std::map<std::string_view, Bus*> TransportCatalogue::GetBusesOnStop(const std::string_view stop_name) const {
        return stop_to_buses_.at(stop_name);
    }

    void TransportCatalogue::SetDistance(Stop* from, Stop* to, int dist) {
        from->stop_distances[to->name] = dist;
    }

    int TransportCatalogue::GetDistance(const Stop* from, const Stop* to) const {
        if (from->stop_distances.count(to->name)) return from->stop_distances.at(to->name);
        else if (to->stop_distances.count(from->name)) return to->stop_distances.at(from->name);
        else return 0;
    }

    const std::map<std::string_view, Bus*>& TransportCatalogue::GetSortedAllBuses() const {
        return buses_list_;
    }

    const std::map<std::string_view, Stop*>& TransportCatalogue::GetSortedAllStops() const {
        return stops_list_;
    }
}