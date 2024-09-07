#ifndef ROUTING_API_H_
#define ROUTING_API_H_

#include <string>
#include <vector>
#include "graph_factory.h"

namespace routing {

class RoutingAPI {
public:
    RoutingAPI();
	virtual ~RoutingAPI();
    virtual IGraph* LoadFromFile(const std::string& file) const;
    virtual void AddFactory(const IGraphFactory* factory);

private:
    std::vector<const IGraphFactory*> factories;
};

}

#endif
