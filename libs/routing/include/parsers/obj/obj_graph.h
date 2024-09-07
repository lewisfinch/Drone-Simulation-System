#ifndef OBJ_GRAPH_PARSER_H_
#define OBJ_GRAPH_PARSER_H_

#include "graph.h"
#include "impl/simple_graph.h"
#include <map>
#include <vector>

namespace routing {

class ObjGraph : public SimpleGraph {
public:
	ObjGraph(const std::string& file);
};

}

#endif