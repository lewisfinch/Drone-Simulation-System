#ifndef ITERATION2_SOLN_SRC_XML_TOOLS_OSM_PARSER_H_
#define ITERATION2_SOLN_SRC_XML_TOOLS_OSM_PARSER_H_

#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "util/xml/pugixml.h"
#include "parsers/osm/osm_graph.h"

using std::string;
using std::unordered_map;
using std::unordered_set;
using std::set;

namespace routing {

class OsmParser {
public:
  static OSMGraph* LoadGraphFromFile(string filename, bool debug);
private:
  static OSMGraph* read_nodes(pugi::xml_document* doc, bool debug = false);
  static void read_adjacencies_to(OSMGraph* graph, pugi::xml_document* doc, bool debug=false);
  static unordered_map<string, set<string>> get_adjacency_list_from_file(pugi::xml_document* doc, bool debug = false);
  static OSMGraph* without_lonely_nodes(OSMGraph* graph);

  static float normalize(float val, float max, float min);
  static float asRadians(float degrees);
  static float getLon(float lat, float lon, float centerLat, float centerLon);
};

};  // namespace entity_project

#endif  // ITERATION2_SOLN_SRC_XML_TOOLS_OSM_PARSER_H_
