#ifndef ASTAR_PATHING_H_
#define ASTAR_PATHING_H_

#include "routing_strategy.h"
#include "graph.h"
#include <string>

namespace routing {

class AStar : public RoutingStrategy {
public:
	AStar() : cost(new EuclideanDistance()), heuristic(new EuclideanDistance()) {}
	AStar(DistanceFunction* cost, DistanceFunction* heuristic) : cost(cost), heuristic(heuristic) {}
	virtual ~AStar();

	std::vector<std::string> GetPath(const IGraph* graph, const std::string& from, const std::string& to) const;

	static const RoutingStrategy& Default() {
		static AStar astar;
		return astar;
	}
	
private:
	DistanceFunction* cost;
	DistanceFunction* heuristic;
};

}

#endif
