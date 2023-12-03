#pragma once

#include "geo.h"
#include "svg.h"
#include "json.h"
#include "json_reader.h"
#include "transport_catalogue.h"

bool IsZero(double value);

class SphereProjector {
public:
    template <typename PointInputIt>
    SphereProjector(PointInputIt points_begin, PointInputIt points_end,
        double max_width, double max_height, double padding) : padding_(padding) {
        if (points_begin == points_end) {
            return;
        }

        const auto [left_it, right_it] = std::minmax_element(points_begin, points_end,
            [](auto lhs, auto rhs) { return lhs.lng < rhs.lng; });
        min_lon_ = left_it->lng;
        const double max_lon = right_it->lng;

        const auto [bottom_it, top_it] = std::minmax_element(points_begin, points_end,
            [](auto lhs, auto rhs) { return lhs.lat < rhs.lat; });
        const double min_lat = bottom_it->lat;
        max_lat_ = top_it->lat;

        std::optional<double> width_zoom;
        if (!IsZero(max_lon - min_lon_)) {
            width_zoom = (max_width - 2 * padding) / (max_lon - min_lon_);
        }

        std::optional<double> height_zoom;
        if (!IsZero(max_lat_ - min_lat)) {
            height_zoom = (max_height - 2 * padding) / (max_lat_ - min_lat);
        }

        if (width_zoom && height_zoom) {
            zoom_coeff_ = std::min(*width_zoom, *height_zoom);
        } else if (width_zoom) {
            zoom_coeff_ = *width_zoom;
        } else if (height_zoom) {
            zoom_coeff_ = *height_zoom;
        }
    }

    svg::Point operator()(geo::Coordinates coords) const {
        return {
            (coords.lng - min_lon_) * zoom_coeff_ + padding_,
            (max_lat_ - coords.lat) * zoom_coeff_ + padding_
        };
    }

private:
    double padding_;
    double min_lon_ = 0;
    double max_lat_ = 0;
    double zoom_coeff_ = 0;
};

class RenderMap { 
public: 
    void AddRouteOnMap(svg::Document& documents, std::vector<svg::Text>& texts_buses, transport_catalogue::domain::Bus* const& bus, const SphereProjector proj, const MapParameter& parameter, unsigned int& number_color); 
    void AddStopOnMap(std::vector<svg::Circle>& circles_stops, std::vector<svg::Text>& texts_stops, transport_catalogue::domain::Stop* const& stop, const SphereProjector proj, const MapParameter& parameter); 
 
    std::string MapRenderer(transport_catalogue::catalogue::TransportCatalogue& catalogue, const MapParameter& parameter); 
    
    svg::Polyline DrawRoute(const transport_catalogue::domain::Bus* const bus, const MapParameter& parameter, const SphereProjector& proj, unsigned int number_color); 
    svg::Text DrawSubstrateBus(const transport_catalogue::domain::Bus* const bus, const MapParameter& parameter, const SphereProjector& proj); 
    svg::Text DrawTextBus(const transport_catalogue::domain::Bus* const bus, const MapParameter& parameter, const SphereProjector& proj, unsigned int number_color); 
    svg::Text DrawSubstrateStop(const transport_catalogue::domain::Stop* const stop, const MapParameter& parameter, const SphereProjector& proj); 
    svg::Text DrawTextStop(const transport_catalogue::domain::Stop* const stop, const MapParameter& parameter, const SphereProjector& proj); 
}; 
