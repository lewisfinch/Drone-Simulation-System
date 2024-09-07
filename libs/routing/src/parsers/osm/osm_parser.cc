#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "parsers/osm/osm_parser.h"
#include "util/xml/pugixml.h"
#include <limits.h>

using std::logic_error;
using std::invalid_argument;

using std::set;
using std::string;

namespace routing {

template <class T, class V>
unordered_map<V, int>* count_value_occurrences(const unordered_map<T, V>* to_count) {
    unordered_map<V, int>* result = new unordered_map<V, int>();

    for (auto kv : *to_count) {
        auto existing_info = result->find(kv.second);
        if (existing_info == result->end()) {
            result->insert({kv.second, 1});
        } else {
            existing_info->second += 1;
        }
    }

    return result;
};

template <class T>
T argmax(const unordered_map<T, int>* mapping) {
    // this iterates through the list twice
    // because T max_found; isn't always valid
    // if T doesn't define a default constructor
    int max = INT_MIN;

    for (auto kv: *mapping) {
        if(kv.second > max) {
            max = kv.second;
        }
    }

    for (auto kv: *mapping) {
        if(kv.second == max) {
            return kv.first;
        }
    }
};

template <class T, class V>
unordered_map<T, bool>* where_equal(const unordered_map<T, int>* mapping, V equal_to){
    unordered_map<T, bool>* result = new unordered_map<T, bool>();

    for (auto kv: *mapping) {
        result->insert({
            kv.first,
            kv.second == equal_to});
    }

    return result;
}

class GraphUtils {
    public :  
        static unordered_map<string, int>* ConnectedComponents(const IGraph* graph);
        static OSMGraph* Filter(const IGraph* original, const unordered_map<string, bool>* filter);
        static OSMGraph* FilterToLargestConnectedComponent(const IGraph* graph);
    private:
        static void dfs_visit(const IGraphNode* n, unordered_map<string, int>* record, int index);
};

unordered_map<string, int>* GraphUtils::ConnectedComponents(const IGraph* graph) {
    unordered_map<string, int>* visited = new unordered_map<string, int>();
    int index = 0;

    for (auto* node : graph->GetNodes()) {
        if(visited->find(node->GetName()) == visited->end()) {
            // we haven't seen this node before
            GraphUtils::dfs_visit(node, visited, index);
            index += 1;
        }
    }

    return visited;
}

OSMGraph* GraphUtils::Filter(const IGraph* original, const unordered_map<string, bool>* filter) {
    // NOTE: does not preserve the type of the graph that was input, always gives back an OSMGraph
    OSMGraph* filtered_graph = new OSMGraph();

    // copy over all of the nodes for which filter(node->GetName()) is true
    for (auto kv : *filter) {
        const bool include = kv.second;
        if (include) {
            const string name = kv.first;
            const IGraphNode* existing_node = original->GetNode(name);
            if(existing_node) {
                vector<float> loc = existing_node->GetPosition();
                filtered_graph->AddNode(
                    new OSMNode(
                        Point3(loc.at(0), loc.at(1), loc.at(2)),
                        existing_node->GetName()));
            } else {
                throw logic_error(name);
            }
        }
    }

    for (IGraphNode* node : original->GetNodes()) {
        auto from_filter = filter->find(node->GetName());
        if (from_filter == filter->end()) {
            throw invalid_argument(node->GetName());
        } // implicit else
        if (from_filter->second) {
            // from node is in the filter            
            for (IGraphNode* other : node->GetNeighbors()) {
                auto to_filter = filter->find(other->GetName());
                if (to_filter == filter->end()) {
                    throw invalid_argument(node->GetName());
                } // implicit else
                if (to_filter->second) {
                    // both nodes in the filter
                    filtered_graph->AddEdge(node->GetName(), other->GetName());
                }
            }
        }
    }

    return filtered_graph;
};

OSMGraph* GraphUtils::FilterToLargestConnectedComponent(const IGraph* original) {
    unordered_map<string, int>* cc_mapping = GraphUtils::ConnectedComponents(original);
    unordered_map<int, int>* cc_sizes = count_value_occurrences(cc_mapping);
    int largest_subgraph_idx = argmax(cc_sizes);
    unordered_map<string, bool>* in_largest_cc = where_equal(cc_mapping, largest_subgraph_idx);
    OSMGraph* result = GraphUtils::Filter(original, in_largest_cc);

    delete cc_mapping;
    delete cc_sizes;
    delete in_largest_cc;
    
    return result;
}

void GraphUtils::dfs_visit(const IGraphNode* n, unordered_map<string, int>* record, int index) {
    if (record->find(n->GetName()) == record->end()) {
        record->insert({n->GetName(), index});

        for (auto* neighbour : (n->GetNeighbors())) {
            GraphUtils::dfs_visit(neighbour, record, index);
        }
    }
}

OSMGraph* OsmParser::LoadGraphFromFile(string filename, bool debug) {
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(filename.c_str());
  // sanity check, make sure the document loaded, and print something (anything) from the document
  #ifdef DEBUG
    std::cerr << "Loading graph using updated code" << std::endl;
  #endif

  OSMGraph* geazy = read_nodes(&doc, debug);

  read_adjacencies_to(geazy, &doc, debug);
  OSMGraph* connected = GraphUtils::FilterToLargestConnectedComponent(geazy);
  delete geazy;
  return connected;
};

OSMGraph* OsmParser::without_lonely_nodes(OSMGraph* geazy) {
  // this literally creates a new graph that's a copy except for 
  // the nodes with degree 0
  // believe it or not, this is actually faster & safer than deleting
  // all of the nodes from the existing graph*
  OSMGraph* newGraph = new OSMGraph();

  for (IGraphNode* node : geazy->GetNodes()) {
    if(node->GetNeighbors().size() > 0) {
      OSMNode* newNode = new OSMNode(node->GetPosition(), node->GetName());
      newGraph->AddNode(newNode);
    }
  }

  for (IGraphNode* node : geazy->GetNodes()) {
    const string name = node->GetName();
    for(IGraphNode* other : node->GetNeighbors()) {
      const string other_name = other->GetName();
      newGraph->AddEdge(name, other_name);
    }
  }

  return newGraph;
}

OSMGraph* OsmParser::read_nodes(pugi::xml_document* doc, bool debug) {
    pugi::xml_node parent_of_nodes = doc->first_child();
    pugi::xml_node way_node;

    OSMGraph* graph = new OSMGraph();

    pugi::xml_node bounds = parent_of_nodes.child("bounds");
    float minlat = std::stod(bounds.attribute("minlat").value());
    float minlon = std::stod(bounds.attribute("minlon").value());
    float maxlat = std::stod(bounds.attribute("maxlat").value());
    float maxlon = std::stod(bounds.attribute("maxlon").value());
    float centerLat = minlat + (maxlat-minlat)/2.0;
    float centerLon = minlon + (maxlon-minlon)/2.0;
    //std::cout << minlat << " " << minlon << " " << maxlat << " " << maxlon << std::endl;)

    std::string id;
    std::string lat;
    std::string lon;

    for(way_node = parent_of_nodes.child("node"); way_node != nullptr; way_node = way_node.next_sibling("node")) {

      if(!way_node.attribute("id")) {
        std::cerr << "Improperly formed node missing id. Continuing." << std::endl;
        continue;
      }
      if(!way_node.attribute("lat")) {
        std::cerr << "Improperly formed node missing lat. ID: " << way_node.attribute("ref");
        std::cerr << ". Continuing" << std::endl;
        continue;
      }
      if(!way_node.attribute("lon")) {
        std::cerr << "Improperly formed node missing lon. ID: " << way_node.attribute("ref");
        std::cerr << ". Continuing" << std::endl;
        continue;
      }
      id = way_node.attribute("id").value();
      lat = way_node.attribute("lat").value();
      lon = way_node.attribute("lon").value();

      if (graph->Contains(id)) {
        std::cerr << "Attempted to add duplicate node. ID: " << way_node.attribute("ref");
        std::cerr << ". Continuing" << std::endl;
      }

      float latitude = std::stod(lat);
      float longitude = std::stod(lon);
      float latN = OsmParser::normalize(latitude, minlat, maxlat);
      float lonN = OsmParser::normalize(longitude, minlon, maxlon);

      longitude = OsmParser::getLon(latitude,longitude, centerLat, centerLon);
      latitude = -(latitude-centerLat)* 40008000.0 / 360.0;
      float height = 264.0f;

      graph->AddNode(
        new OSMNode(
          Point3(longitude, height, latitude),
          id));
          
    }

    return graph;
};

float OsmParser::normalize(float val, float max, float min) {
  return (val - min) / (max - min);
};

float OsmParser::getLon(float lat, float lon, float centerLat, float centerLon) {
  float deltaLatitude = lat - centerLat;
  float deltaLongitude = lon - centerLon;
  float latitudeCircumference = 40075160 * std::cos(OsmParser::asRadians(centerLat));
  float resultX = deltaLongitude * latitudeCircumference / 360.0f;
  return resultX;
}

float OsmParser::asRadians(float degrees) {
  return degrees * 3.14159f / 180.0f;
}

void OsmParser::read_adjacencies_to(OSMGraph* graph, pugi::xml_document* doc, bool debug) {
  unordered_map<string, set<string>> adjacencies = get_adjacency_list_from_file(doc, debug);

  for(auto it : adjacencies) {
    string from = it.first;
    set<string> others = it.second;
    if(!graph->Contains(from)) {
      std::cerr << "Node ID: " << from << " not found. Continuing." << std::endl;
      continue;
    } // implicit else

    for(string to : others) {
      if(!graph->Contains(to)) {
        std::cerr << "Node ID: " << to << " not found. Continuing." << std::endl;
        continue;
      } // implicit else

      graph->AddEdge(from, to);
    }
  }
};

std::unordered_map<std::string, std::set<std::string>>
    OsmParser::get_adjacency_list_from_file(pugi::xml_document* doc, bool debug) {

  // create a parent of all (interesting) nodes, then get the first node the the "way" name
  pugi::xml_node parent_of_nodes = doc->first_child();
  pugi::xml_node way_node = parent_of_nodes.child("way");
  // create a hashmap the takes a string id and returns the xml_node
  //  corresponding to the "way" node with the same id
  std::unordered_map<std::string, pugi::xml_node> highway_way_nodes =
    std::unordered_map<std::string, pugi::xml_node>();
  // while the current way node is not equal to null, if it has a highway tag, add it to the hashmap
  //  of ids to way nodes
  while (way_node != NULL) {  
    for(pugi::xml_node tag_node = way_node.child("tag"); tag_node != nullptr; tag_node = tag_node.next_sibling("tag")) {
      if (tag_node != NULL &&
          strcmp(tag_node.attribute("k").value(), "highway") == 0) {
        highway_way_nodes.insert({way_node.attribute("id").value(), way_node});
        break;
      }
    }
    way_node = way_node.next_sibling();
  }
  // get all the string ids of nodes that are a part of highway ways, we don't care about the order
  //  and don't want repeats, so we use a unordered set
  std::unordered_set<std::string> highway_node_ids = std::unordered_set<std::string>();
  // for every highway way node, any child named "nd" is a node that is part of the way,
  //  so add it to the highway node ids set
  for (auto & pair : highway_way_nodes) {
    pugi::xml_node way_node_child = pair.second.child("nd");
    while (way_node_child != NULL) {
      // if the node's name is not nd, we don't care about it, so skip. otherwise add to set
      if (strcmp(way_node_child.name(), "nd") == 0) {
        highway_node_ids.insert(way_node_child.attribute("ref").value());
      }
      way_node_child = way_node_child.next_sibling();
    }
  }
  // we want an adjacency list that maps from a node's id to a list of nodes it is connected to
  std::unordered_map<std::string, std::set<std::string>> adjacency_list =
  std::unordered_map<std::string, std::set<std::string>>();
  // initialize it
  for (auto highway_node_id : highway_node_ids) {
    adjacency_list.insert({highway_node_id, std::set<std::string>()});
  }
  int num_edges = 0;
  // for every way node, adjacent "nd" nodes are connected, iterate through and connect
  //  adjacent nodes in the adjacency list
  for (auto way_node_key_value : highway_way_nodes) {
    pugi::xml_node first = way_node_key_value.second.child("nd");
    pugi::xml_node second = way_node_key_value.second.child("nd").next_sibling();
    while (second != NULL) {
      // if either first or second nodes aren't "nd" nodes, skip
      //  otherwise assign each to the other's adjacency list
      if (strcmp(first.name(), "nd") == 0 && strcmp(second.name(), "nd") == 0) {
        adjacency_list[first.attribute("ref").value()].insert(second.attribute("ref").value());
        adjacency_list[second.attribute("ref").value()].insert(first.attribute("ref").value());
        num_edges++;
      }
      first = second;
      second = second.next_sibling();
    }
  }
  return adjacency_list;
};

}