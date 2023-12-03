#include "map_renderer.h"
#include "request_handler.h"
#include "transport_catalogue.h"


int main() {
	using namespace transport_catalogue;

	catalogue::TransportCatalogue catalogue;
    RequestHandler requestHandler;
    RenderMap renderer;

	DataRequests query = GetWriteData(std::cin);
	MakeCatalogue(catalogue, query.base_request_);
    std::string transport_map = renderer.MapRenderer(catalogue, query.parameter_);
	requestHandler.Handle(catalogue, query.stat_request_, transport_map);
	return 0;
}
