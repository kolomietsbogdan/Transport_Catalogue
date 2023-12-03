#include "map_renderer.h"

inline const double EPSILON = 1e-6;

bool IsZero(double value) {
    return std::abs(value) < EPSILON;
}


void RenderMap::AddRouteOnMap(svg::Document& documents, std::vector<svg::Text>& texts_buses, transport_catalogue::domain::Bus* const &bus,const SphereProjector proj, const MapParameter& parameter, unsigned int& number_color) {
    if (bus->stops_.size() != 0) {
        documents.Add(DrawRoute(bus, parameter, proj, number_color));

        svg::Text substrate_bus = DrawSubstrateBus(bus, parameter, proj);
        texts_buses.push_back(substrate_bus);
        svg::Text text_bus = DrawTextBus(bus, parameter, proj, number_color);
        texts_buses.push_back(text_bus);

        if (bus->is_roundtrip_ == false && *bus->stops_.begin() != bus->stops_[static_cast<int>(bus->stops_.size()) / 2]) {
            substrate_bus.SetPosition(proj(bus->stops_[static_cast<int>(bus->stops_.size()) / 2]->coordinates_));
            texts_buses.push_back(std::move(substrate_bus));
            text_bus.SetPosition(proj(bus->stops_[static_cast<int>(bus->stops_.size()) / 2]->coordinates_));
            texts_buses.push_back(std::move(text_bus));
        }

        number_color == parameter.color_palette_.size() - 1 ? number_color = 0 : ++number_color;
    }
}

void RenderMap::AddStopOnMap(std::vector<svg::Circle>& circles_stops, std::vector<svg::Text>& texts_stops, transport_catalogue::domain::Stop* const& stop, const SphereProjector proj, const MapParameter& parameter) {
    if (stop->buses.size() != 0) {
        svg::Circle circle;

        circle.SetCenter(proj(stop->coordinates_));
        circle.SetRadius(parameter.stop_radius_);
        circle.SetFillColor("white");
        circles_stops.push_back(circle);

        svg::Text substrate_stop = DrawSubstrateStop(stop, parameter, proj);
        texts_stops.push_back(substrate_stop);
        svg::Text text_stop = DrawTextStop(stop, parameter, proj);
        texts_stops.push_back(text_stop);
    }
}

std::string RenderMap::MapRenderer(transport_catalogue::catalogue::TransportCatalogue& catalogue, const MapParameter& parameter) {
    std::vector<geo::Coordinates> geo_coords = catalogue.CombineStopCoordinates();
    const SphereProjector proj{ geo_coords.begin(), geo_coords.end(), parameter.width_, parameter.height_, parameter.padding_ };

    svg::Document documents;

    std::vector<svg::Text> texts_buses;
    std::vector<svg::Circle> circles_stops;
    std::vector<svg::Text> texts_stops;

    unsigned int number_color = 0;

    for (const auto& bus : catalogue.GetSortedBusesByName()) {
        AddRouteOnMap(documents, texts_buses, bus, proj, parameter, number_color);
    }

    for (const auto& stop : catalogue.GetSortedStopsByName()) {
        AddStopOnMap(circles_stops, texts_stops, stop, proj, parameter);
    }

    for (const auto&t_b : texts_buses) {
        documents.Add(t_b);
    }
    for (const auto& c_s : circles_stops) {
        documents.Add(c_s);
    }
    for (const auto& t_s : texts_stops) {
        documents.Add(t_s);
    }
    std::ostringstream oss;
    documents.Render(oss);
    return oss.str();
}


svg::Polyline RenderMap::DrawRoute(const transport_catalogue::domain::Bus* const bus, const MapParameter& parameter, const SphereProjector& proj, unsigned int number_color) {
    svg::Polyline route;
    for (const auto& stop : bus->stops_) {
        route.AddPoint(proj(stop->coordinates_));
    }
    route.SetStrokeColor(parameter.color_palette_[number_color]);
    route.SetFillColor(svg::NoneColor);
    route.SetStrokeWidth(parameter.line_width_);
    route.SetStrokeLineCap(svg::StrokeLineCap::ROUND);
    route.SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);
    return route;
}

svg::Text RenderMap::DrawSubstrateBus(const transport_catalogue::domain::Bus* const bus, const MapParameter& parameter, const SphereProjector& proj) {
    svg::Text substrate_bus;
    substrate_bus.SetPosition(proj(bus->stops_[0]->coordinates_));
    substrate_bus.SetOffset(parameter.bus_label_offset_);
    substrate_bus.SetFontSize(parameter.bus_label_font_size_);
    substrate_bus.SetFontFamily("Verdana");
    substrate_bus.SetFontWeight("bold");
    substrate_bus.SetData(bus->bus_name_);

    substrate_bus.SetStrokeColor(parameter.underlayer_color_);
    substrate_bus.SetFillColor(parameter.underlayer_color_);
    substrate_bus.SetStrokeWidth(parameter.underlayer_width_);
    substrate_bus.SetStrokeLineCap(svg::StrokeLineCap::ROUND);
    substrate_bus.SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);
    return substrate_bus;
}

svg::Text RenderMap::DrawTextBus(const transport_catalogue::domain::Bus* const bus, const MapParameter& parameter, const SphereProjector& proj, unsigned int number_color) {
    svg::Text text_bus;
    text_bus.SetPosition(proj(bus->stops_[0]->coordinates_));
    text_bus.SetOffset(parameter.bus_label_offset_);
    text_bus.SetFontSize(parameter.bus_label_font_size_);
    text_bus.SetFontFamily("Verdana");
    text_bus.SetFontWeight("bold");
    text_bus.SetData(bus->bus_name_);

    text_bus.SetFillColor(parameter.color_palette_[number_color]);
    return text_bus;
}

svg::Text RenderMap::DrawSubstrateStop(const transport_catalogue::domain::Stop* const stop, const MapParameter& parameter, const SphereProjector& proj) {
    svg::Text substrate_stop;
    substrate_stop.SetPosition(proj(stop->coordinates_));
    substrate_stop.SetOffset(parameter.stop_label_offset_);
    substrate_stop.SetFontSize(parameter.stop_label_font_size_);
    substrate_stop.SetFontFamily("Verdana");
    substrate_stop.SetData(stop->stop_name_);

    substrate_stop.SetStrokeColor(parameter.underlayer_color_);
    substrate_stop.SetFillColor(parameter.underlayer_color_);
    substrate_stop.SetStrokeWidth(parameter.underlayer_width_);
    substrate_stop.SetStrokeLineCap(svg::StrokeLineCap::ROUND);
    substrate_stop.SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);
    return substrate_stop;
}

svg::Text RenderMap::DrawTextStop(const transport_catalogue::domain::Stop* const stop, const MapParameter& parameter, const SphereProjector& proj) {
    svg::Text text_stop;
    text_stop.SetPosition(proj(stop->coordinates_));
    text_stop.SetOffset(parameter.stop_label_offset_);
    text_stop.SetFontSize(parameter.stop_label_font_size_);
    text_stop.SetFontFamily("Verdana");
    text_stop.SetData(stop->stop_name_);

    text_stop.SetFillColor("black");
    return text_stop;
}
