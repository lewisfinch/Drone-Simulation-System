#include "graph.h"
#include <limits>

namespace routing {

BoundingBox GraphBase::GetBoundingBox() const {
    BoundingBox bb;

    const std::vector<IGraphNode*>& nodes = GetNodes();
    
    for (int i = 0; i < nodes.size(); i++) {
        std::vector<float> pos = nodes[i]->GetPosition();
        if (i == 0) {
            bb.min = pos;
            bb.max = pos;
        }
        else {
            for (int j = 0; j < pos.size(); j++) {
                if (bb.min[j] > pos[j]) {
                    bb.min[j] = pos[j];
                }
                if (bb.max[j] < pos[j]) {
                    bb.max[j] = pos[j];
                }
            }
        }
    }

    return bb;
}

const IGraphNode* GraphBase::NearestNode(std::vector<float> point, const DistanceFunction& distanceFunction) const {
    const std::vector<IGraphNode*> nodes = GetNodes();
    float minDistance = std::numeric_limits<float>::infinity();
    const IGraphNode* closestNode = NULL;
    for (auto* node: nodes) {
        auto temp_3d = node->GetPosition();
        float distance = distanceFunction.Calculate(temp_3d, point);
        if (distance < minDistance) {
            closestNode = node;
            minDistance = distance;
        }
    }

    return closestNode;
}

const std::vector< std::vector<float> > GraphBase::GetPath(std::vector<float> src, std::vector<float> dest, const RoutingStrategy& pathing) const {
    using namespace std;
    const IGraphNode* start_node = NearestNode(src, EuclideanDistance());
    const IGraphNode* end_node = NearestNode(dest, EuclideanDistance());
    vector<string> string_path = pathing.GetPath(this, start_node->GetName(), end_node->GetName());

    vector< vector<float> > position_path;
    position_path.push_back(start_node->GetPosition());
    for (int i = 0; i < string_path.size(); i++) {
        position_path.push_back(this->GetNode(string_path[i])->GetPosition());
    }
    position_path.push_back(end_node->GetPosition());

    return position_path; 
}

}

