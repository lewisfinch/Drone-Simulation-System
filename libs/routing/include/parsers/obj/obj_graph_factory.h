#ifndef OBJ_GRAPH_FACTORY_H_
#define OBJ_GRAPH_FACTORY_H_

#include "graph_factory.h"
#include "parsers/obj/obj_graph.h"

namespace routing {

class ObjGraphFactory : public IGraphFactory {
public:
	virtual ~ObjGraphFactory() {}
	virtual IGraph* Create(const std::string& file) const {
		if (file.substr(file.size()-4) != ".obj") {
			return NULL;
		}

		return new ObjGraph(file);
	}
};

}

#endif
