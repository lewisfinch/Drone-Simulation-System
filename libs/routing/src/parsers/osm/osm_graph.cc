#include <stdexcept>
#include <limits>
#include "parsers/osm/osm_graph.h"

using std::string;
using std::vector;
using std::unordered_map;
using std::invalid_argument;
using std::out_of_range;
using std::numeric_limits;


namespace routing {

OSMNode::OSMNode(Point3 loc, string name) : loc_(loc), name_(name) { };

/*const vector< vector<float> > OSMGraph::GetPath(vector<float> src, vector<float> dest) const {
  const IGraphNode* start_node = entity_project::NearestNode(this, src);
  const IGraphNode* end_node = entity_project::NearestNode(this, dest);

  vector<string>* string_path = entity_project::DjikstraPathing::GetPath(this, start_node->GetName(), end_node->GetName());

  vector< vector<float> > position_path = entity_project::ConvertPathToFloatVectors(*this, *string_path);
  position_path.insert(position_path.begin(), src);
  position_path.push_back(dest);
  std::cout << "Called getpathpositions" << std::endl; 
  return position_path; 
}*/

OSMGraph::~OSMGraph() {
    for (auto kv: lookup_) {
        delete kv.second; // delete the associated node
    }
};

void OSMGraph::AddNode(OSMNode* node) {
    const string name = node->GetName();
    if(lookup_.find(name) != lookup_.end()) {
        // attempting to add duplicate Node
        throw invalid_argument(name);
    }
    lookup_.insert({name, node});
    nodes_.push_back(node);
};

void OSMGraph::AddEdge(const string name1, const string name2) {
    OSMNode* node1 = node_named(name1);
    OSMNode* node2 = node_named(name2);
    node1->AddNeighbour(node2);
};

OSMNode* OSMGraph::node_named(const string name) const {
    auto result = lookup_.find(name);
    if (result == lookup_.end()) {
        throw invalid_argument(name);
    }
    return result->second;
};

const OSMNode* OSMGraph::NodeNamed(const string name) const {
    return node_named(name);
};

bool OSMGraph::Contains(const string name) const {
    return !(lookup_.find(name) == lookup_.end());
};

};