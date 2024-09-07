#include "routing_api.h"
#include "parsers/osm/osm_graph_factory.h"
#include "parsers/obj/obj_graph_factory.h"

namespace routing {

RoutingAPI::RoutingAPI() {
    factories.push_back(new OSMGraphFactory());
    factories.push_back(new ObjGraphFactory());
}

RoutingAPI::~RoutingAPI() {
    for (int i = 0; i < factories.size(); i++) {
        delete factories[i];
    }
}

IGraph* RoutingAPI::LoadFromFile(const std::string& file) const {
    for (int i = 0; i < factories.size(); i++) {
        IGraph* graph = factories[i]->Create(file);
        if (graph) {
            return graph;
        }
    }

    return NULL;
}

void RoutingAPI::AddFactory(const IGraphFactory* factory) {
    factories.push_back(factory);
}

}