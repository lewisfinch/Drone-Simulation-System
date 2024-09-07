#ifndef DFS_PATHING_H_
#define DFS_PATHING_H_

#include "routing_strategy.h"
#include <string>

namespace routing {

class DepthFirstSearch : public RoutingStrategy {
public:
	virtual ~DepthFirstSearch() {}

	std::vector<std::string> GetPath(const IGraph* graph, const std::string& from, const std::string& to) const;

	static const RoutingStrategy& Default() {
		static DepthFirstSearch dfs;
		return dfs;
	}
};

}

#endif
