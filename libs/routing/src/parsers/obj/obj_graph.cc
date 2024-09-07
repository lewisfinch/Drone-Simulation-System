#include "parsers/obj/obj_graph.h"
#include <fstream>

namespace routing {

ObjGraph::ObjGraph(const std::string& file) {
    std::ifstream objFile;
    objFile.open(file);
    
    if (objFile.is_open()) {

        int numNodes = 0;
        std::string in;

        while (objFile >> in) {
            if (in == "v") {
                std::vector<float> pos;
                float x, y, z;
                objFile >> x >> y >> z;
                pos.push_back(x);
                pos.push_back(z);
                pos.push_back(-y);

                numNodes++;
                AddNode(new SimpleGraphNode(std::to_string(numNodes), pos));
            }

            if (in == "f") {
                std::string a, b, c;
                objFile >> a >> b >> c;
                AddEdge(a, b);
                AddEdge(b, a);
                AddEdge(b, c);
                AddEdge(c, b);
                AddEdge(c, a);
                AddEdge(a, c);
            }
        }
        

        objFile.close();
    }
}

}
