#ifndef DIJKSTRA_PATHING_H_
#define DIJKSTRA_PATHING_H_

#include "routing/astar.h"
#include <string>

namespace routing {

class Dijkstra : public AStar {
public:
    Dijkstra() : AStar(new EuclideanDistance(), new ZeroDistance()) {}
	virtual ~Dijkstra() {}

	static const RoutingStrategy& Instance() {
		static Dijkstra dikjstra;
		return dikjstra;
	}
};

}

#endif
