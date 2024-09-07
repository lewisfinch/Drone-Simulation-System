#ifndef OSM_GRAPH_FACTORY_H_
#define OSM_GRAPH_FACTORY_H_

#include "graph_factory.h"

namespace routing {

class OSMGraphFactory : public IGraphFactory {
public:
	virtual ~OSMGraphFactory() {}
	virtual IGraph* Create(const std::string& file) const;
};

}

#endif
