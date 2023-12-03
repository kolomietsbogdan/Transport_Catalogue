#include "json_reader.h"

const std::string& Request::GetTypeRequest() const {
    return type_request_;
}

void Request::SetTypeRequest(std::string type_request) {
    type_request_ = std::move(type_request);
}

const std::string& Request::GetType() const {
    return type_;
}

void Request::SetType(std::string type) {
    type_ = std::move(type);
}

const std::string& Request::GetName() const {
    return name_;
}

void Request::SetName(std::string name) {
    name_ = std::move(name);
}

std::vector<BaseRequest> ReaderBaseRequests(const json::Node& data) {
    std::vector<BaseRequest> v_base_request;
    v_base_request.reserve(data.AsArray().size());
    for (const auto& arr : data.AsArray()) {
        BaseRequest base_request;
        base_request.SetTypeRequest("base_requests");

        for (const auto& [key_type, value] : arr.AsDict()) {
            if (key_type == "type") {
                base_request.SetType(value.AsString());
            } 
            else if (key_type == "name") {
                base_request.SetName(value.AsString());
            }
            else if (key_type == "latitude") {
                base_request.coordinates.lat = value.AsDouble();
            }
            else if (key_type == "longitude") {
                base_request.coordinates.lng = value.AsDouble();
            }
            else if (key_type == "road_distances") {
                for (const auto& [to_stop_name_, distances] : value.AsDict()) {
                    base_request.road_distances[to_stop_name_] = distances.AsInt();
                }
            }
            else if (key_type == "stops") {
                for (const auto& stop_name : value.AsArray()) {
                    base_request.stops.push_back(stop_name.AsString());
                }
            }
            else if (key_type == "is_roundtrip") {
                base_request.is_roundtrip = value.AsBool();
            } else {
                throw json::ParsingError("Parsing error");
            }
        }
        v_base_request.push_back(std::move(base_request));
    }
    return v_base_request;
}

std::vector<StatRequest> ReaderStatRequests(const json::Node& data) {
    std::vector<StatRequest> v_stat_request;
    v_stat_request.reserve(data.AsArray().size());
    for (const auto& arr : data.AsArray()) {
        StatRequest stat_request;
        stat_request.SetTypeRequest("stat_requests");

        for (const auto& [key_type, value] : arr.AsDict()) {
            if (key_type == "id") {
                stat_request.id = value.AsInt();
            }
            else if (key_type == "type") {
                stat_request.SetType(value.AsString());
            }
            else if (key_type == "name") {
                stat_request.SetName(value.AsString());
            } else {
                throw json::ParsingError("Parsing error");
            }
        }
        v_stat_request.push_back(std::move(stat_request));
    }
    return v_stat_request;
}

MapParameter ReaderRenderSettings(const json::Node& data) {
    MapParameter parameters;
    for (const auto& [key_type, value] : data.AsDict()) {
        if (key_type == "width") {
            parameters.width_ = value.AsDouble();
        }
        else if (key_type == "height") {
            parameters.height_ = value.AsDouble();
        }
        else if (key_type == "padding") {
            parameters.padding_ = value.AsDouble();
        }
        else if (key_type == "line_width") {
            parameters.line_width_ = value.AsDouble();
        }
        else if (key_type == "stop_radius") {
            parameters.stop_radius_ = value.AsDouble();
        }
        else if (key_type == "bus_label_font_size") {
            parameters.bus_label_font_size_ = value.AsInt();
        }
        else if (key_type == "bus_label_offset") {
            if (value.AsArray().size() != 2) {
                throw json::ParsingError("bus_label_offset has more than two values");
            }
            for (size_t i = 0; i < value.AsArray().size(); ++i) {
                i == 0 ? parameters.bus_label_offset_.x = value.AsArray()[i].AsDouble() :
                    parameters.bus_label_offset_.y = value.AsArray()[i].AsDouble();
            }
        }
        else if (key_type == "stop_label_font_size") {
            parameters.stop_label_font_size_ = value.AsInt();
        }
        else if (key_type == "stop_label_offset") {
            if (value.AsArray().size() != 2) {
                throw json::ParsingError("bus_label_offset has more than two values");
            }
            for (size_t i = 0; i < value.AsArray().size(); ++i) {
                i == 0 ? parameters.stop_label_offset_.x = value.AsArray()[i].AsDouble() :
                    parameters.stop_label_offset_.y = value.AsArray()[i].AsDouble();
            }
        }
        else if (key_type == "underlayer_color") {
            if (value.IsString()) {
                parameters.underlayer_color_ = value.AsString();
            }
            else if (value.IsArray()) {
                int count_numbers = static_cast<int>(value.AsArray().size());

                std::string name_color = count_numbers == 3 ? "rgb(" : "rgba(";
                for (int i = 0; i < count_numbers; ++i) {
                    if (i != 3) {
                        name_color += std::to_string(value.AsArray()[i].AsInt());
                    } else {
                        std::string str;
                        std::ostringstream oss;
                        oss << str << value.AsArray()[i].AsDouble();
                        str = oss.str();

                        name_color += str;
                    }
                    i != count_numbers - 1 ? name_color.push_back(',') : name_color.push_back(')');
                }
                parameters.underlayer_color_ = name_color;
            } else {
                throw json::ParsingError("Parsing error");
            }
        }
        else if (key_type == "underlayer_width") {
            parameters.underlayer_width_ = value.AsDouble();
        }
        else if (key_type == "color_palette") {
            for (const auto& color : value.AsArray()) {
                if (color.IsString()) {
                    parameters.color_palette_.push_back(color.AsString());
                }
                else if (color.IsArray()) {
                    int count_numbers = static_cast<int>(color.AsArray().size());

                    std::string name_color = count_numbers == 3 ? "rgb(" : "rgba(";
                    for (int i = 0; i < count_numbers; ++i) {
                        if (i != 3) {
                            name_color += std::to_string(color.AsArray()[i].AsInt());
                        } else {
                            std::string str;
                            std::ostringstream oss;
                            oss << str << color.AsArray()[i].AsDouble();
                            str = oss.str();

                            name_color += str;
                        }
                        i != count_numbers - 1 ? name_color.push_back(',') : name_color.push_back(')');
                    }
                    parameters.color_palette_.push_back(name_color);
                } else {
                    throw json::ParsingError("Parsing error");
                }
            }
        } else {
            throw json::ParsingError("Parsing error");
        }
    }
    return parameters;
}


DataRequests GetWriteData(std::istream& input) {
    DataRequests queue_request;

    json::Document doc = json::Load(input);
    json::Node node = doc.GetRoute();

    for (const auto& [type_request, data] : node.AsDict()) {
        if (type_request == "base_requests") {
            std::vector<BaseRequest> v_base_request = ReaderBaseRequests(data);
            queue_request.base_request_.reserve(v_base_request.size());
            for (auto&& element : v_base_request) {
                queue_request.base_request_.push_back(std::move(element));
            }
        } else if (type_request == "stat_requests") {
            std::vector<StatRequest> v_base_request = ReaderStatRequests(data);
            queue_request.stat_request_.reserve(v_base_request.size());
            for (auto&& element : v_base_request) {
                queue_request.stat_request_.push_back(std::move(element));
            }
        } else if (type_request == "render_settings") {
            queue_request.parameter_ = std::move(ReaderRenderSettings(data));
        } else {
            throw json::ParsingError("Parsing error");
        }
    }
    return queue_request;
}

transport_catalogue::catalogue::TransportCatalogue& MakeCatalogue(transport_catalogue::catalogue::TransportCatalogue& catalogue, std::vector<BaseRequest> requests) {
    for (const auto& base_requests : requests) {
        if (base_requests.GetType() == "Stop") {
            catalogue.AddStop(base_requests.GetName(), base_requests.coordinates);
        }
    }
    for (const auto& base_requests : requests) {
        if (base_requests.GetType() == "Bus") {
            std::vector<transport_catalogue::domain::Stop*> stops = MakeStops(catalogue, base_requests.stops, base_requests.is_roundtrip);

            catalogue.AddBus(base_requests.GetName(), stops, base_requests.is_roundtrip);

            for (auto& stop : stops) {
                catalogue.FindStop(stop->stop_name_)->buses.insert(catalogue.FindBus(base_requests.GetName())->bus_name_);
            }
        }
    }
    for (const auto& base_requests : requests) {
        if (base_requests.GetType() == "Stop") {
            for (const auto& [stop_name, distance] : base_requests.road_distances) {
                catalogue.AddDistanceBetweenStops(base_requests.GetName(), stop_name, distance);
            }
        }
    }
    return catalogue;
}

std::vector<transport_catalogue::domain::Stop*> MakeStops(transport_catalogue::catalogue::TransportCatalogue& catalogue, const std::vector<std::string>& stops, bool is_roundtrip) {
    std::vector<transport_catalogue::domain::Stop*> stops_;

    for (size_t i = 0; i < static_cast<size_t>(stops.size()); ++i) {
        stops_.push_back(catalogue.FindStop(stops[i]));
    }

    if (is_roundtrip == false) {
        const size_t stops_count = stops.size();
        for (size_t i = stops_count - 1; i != 0;) {
            --i;
            stops_.push_back(stops_[i]);
        }
    }
    return stops_;
}
