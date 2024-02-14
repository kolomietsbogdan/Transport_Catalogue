#include "json_reader.h"
#include "map_renderer.h"
#include "serialization.h"
#include "request_handler.h"
#include "transport_router.h"
#include "transport_catalogue.h"

using namespace std::literals;

void PrintUsage(std::ostream& stream = std::cerr) {
    stream << "Usage: transport_catalogue [make_base|process_requests]\n"sv;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        PrintUsage();
        return 1;
    }
    const std::string_view mode(argv[1]);
    if (mode == "make_base"sv) {
        Request input_json(json::GetWriteData(std::cin));
        transport_catalogue::TransportCatalogue catalogue;
        input_json.MakeCatalogue(catalogue);
        renderer::MapRenderer renderer(input_json.GetRenderSettings());
        transport_catalogue::TransportRouter router(input_json.GetRoutingSettings(), catalogue);
        std::ofstream fout(input_json.GetSerializationSettings().AsDict().at("file"s).AsString(), std::ios::binary);
        if (fout.is_open()) {
            Serialize(catalogue, renderer, router, fout);
        }
    }
    else if (mode == "process_requests"sv) {
        Request input_json(json::GetWriteData(std::cin));
        std::ifstream db_file(input_json.GetSerializationSettings().AsDict().at("file"s).AsString(), std::ios::binary);
        if (db_file) {
            auto [catalogue, renderer, router, graph, stop_ids] = Deserialize(db_file);
            router.SetGraph(std::move(graph), std::move(stop_ids));
            RequestHandler requestHandler(catalogue, router, renderer);
            requestHandler.Handle(input_json.GetStatRequest(), std::cout);
        }
    }
    else {
        PrintUsage();
        return 1;
    }
}
