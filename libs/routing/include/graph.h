#ifndef GRAPH_H_
#define GRAPH_H_

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "routing_strategy.h"
#include "distance_function.h"
#include "bounding_box.h"

namespace routing {

class IGraphNode;
class RoutingStrategy;

class IGraph {
public:
	virtual ~IGraph() {}
	virtual const IGraphNode* GetNode(const std::string& name) const = 0;
	virtual const std::vector<IGraphNode*>& GetNodes() const = 0;
	virtual BoundingBox GetBoundingBox() const = 0;
	virtual const IGraphNode* NearestNode(std::vector<float> point, const DistanceFunction& distance) const = 0;
	virtual const std::vector< std::vector<float> > GetPath(std::vector<float> src, std::vector<float> dest, const RoutingStrategy& strategy) const = 0;
};

class IGraphNode {
public:
	virtual ~IGraphNode() {}
	virtual const std::string& GetName() const = 0;
	virtual const std::vector<IGraphNode*>& GetNeighbors() const = 0;
	virtual const std::vector<float> GetPosition() const = 0;
};

class GraphBase : public IGraph {
public:
	virtual ~GraphBase() {}
	BoundingBox GetBoundingBox() const;
	const IGraphNode* NearestNode(std::vector<float> point, const DistanceFunction& distance) const;
	const std::vector< std::vector<float> > GetPath(std::vector<float> src, std::vector<float> dest, const RoutingStrategy& strategy) const;
};

}

#endif
