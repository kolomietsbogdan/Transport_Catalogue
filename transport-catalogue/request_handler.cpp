#include "request_handler.h"

void RequestHandler::Handle(transport_catalogue::catalogue::TransportCatalogue& catalogue, const std::vector<StatRequest>& requests, const std::string& map_to_string) {
	json::Builder builder;
	builder.StartArray();

	for (const auto& request : requests) {
		if (request.GetType() == "Bus") {
			if (catalogue.FindBus(request.GetName()) == nullptr) {
				builder.StartDict()
					.Key("request_id").Value(request.id)
					.Key("error_message").Value("not found")
					.EndDict();
			}
			else {
				transport_catalogue::catalogue::TransportCatalogue::BusInfo bus_info = catalogue.GetBusInfo(request.GetName());

				builder.StartDict()
					.Key("curvature").Value(bus_info.curvature)
					.Key("request_id").Value(request.id)
					.Key("route_length").Value(bus_info.route_length)
					.Key("stop_count").Value(bus_info.stops_amount)
					.Key("unique_stop_count").Value(bus_info.unique_stops)
					.EndDict();
			}
		} else if (request.GetType() == "Stop") {
			if (catalogue.FindStop(request.GetName()) == nullptr) {
				builder.StartDict()
					.Key("request_id").Value(request.id)
					.Key("error_message").Value("not found")
					.EndDict();
			} else {
				std::set<std::string_view> stop_info = catalogue.GetStopInfo(request.GetName());

				builder.StartDict().Key("buses").StartArray();

				for (const auto& bus : stop_info) {
					builder.Value(static_cast<std::string>(bus));
				}
				builder.EndArray().Key("request_id").Value(request.id).EndDict();
			}
		} else if (request.GetType() == "Map") {
			builder.StartDict().Key("map").Value(map_to_string).Key("request_id").Value(request.id).EndDict();
		} else {
			throw std::logic_error("request type not found");
		}
	}
	builder.EndArray();

	json::Print(json::Document{ builder.Build() }, std::cout);
	std::cout << std::endl;
}
