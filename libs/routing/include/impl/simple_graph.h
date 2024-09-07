#ifndef SIMPLE_GRAPH_H_
#define SIMPLE_GRAPH_H_

#include "graph.h"
#include <map>
#include <string>
#include <vector>

namespace routing {

class SimpleGraphNode : public IGraphNode {
public:
    SimpleGraphNode(const std::string& name, const std::vector<float>& position) : name(name), position(position) {}
	virtual ~SimpleGraphNode() {}
	const std::string& GetName() const { return name; }
	const std::vector<IGraphNode*>& GetNeighbors() const { return neighbors; }
	const std::vector<float> GetPosition() const { return position; }
    void AddNeighbor(IGraphNode* neighbor) { neighbors.push_back(neighbor); }

private:
    std::string name;
    std::vector<IGraphNode*> neighbors;
    std::vector<float> position;
};

class SimpleGraph : public GraphBase {
public:
	virtual ~SimpleGraph() {
        for (int i = 0; i < nodes.size(); i++) {
            delete nodes[i];
        }
    }
	const IGraphNode* GetNode(const std::string& name) const { return nodeMap.find(name)->second; }
	const std::vector<IGraphNode*>& GetNodes() const { return nodes; }
    void AddNode(SimpleGraphNode* node) { 
        nodes.push_back(node);
        nodeMap[node->GetName()] = node; 
    }
    void AddEdge(const std::string& a, const std::string& b) {
        nodeMap[a]->AddNeighbor(nodeMap[b]);
    }

private:
    std::map<std::string, SimpleGraphNode*> nodeMap;
    std::vector<IGraphNode*> nodes;
};


}

#endif