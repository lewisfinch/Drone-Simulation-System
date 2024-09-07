#include "parsers/osm/osm_graph_factory.h"
#include "parsers/osm/osm_parser.h"

#include <stdexcept>

namespace routing {

IGraph* OSMGraphFactory::Create(const std::string& file) const {
	if (file.substr(file.size()-4) != ".osm") {
		return NULL;
	}

	return OsmParser::LoadGraphFromFile(file, false);
}

}