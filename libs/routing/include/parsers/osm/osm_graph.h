#ifndef OSM_GRAPH_H_
#define OSM_GRAPH_H_

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include "graph.h"
#include "parsers/osm/point3.h"


using std::string;
using std::vector;
using std::unordered_map;

namespace routing {

class OSMNode: public IGraphNode {
    public:
        OSMNode(Point3 loc, string name);
        Point3 GetLoc() const { return loc_; };
        const string& GetName() const override { return name_; };
        void AddNeighbour(OSMNode* other) { neighbours_.push_back(other); };
        const std::vector<IGraphNode*>& GetNeighbors() const override
            {   return neighbours_;
            };
        const std::vector<float> GetPosition() const override {
            return loc_.toVec();
        }
    private:
        string name_;
        Point3 loc_;
        vector<IGraphNode*> neighbours_; 
};

class OSMGraph : public GraphBase {
    public:
        ~OSMGraph();
        void AddNode(OSMNode* node);
        const OSMNode* NodeNamed(const string name) const;
        void AddEdge(const string name1, const string name2);
        bool Contains(const string name) const;

        const IGraphNode* GetNode(const std::string& name) const override 
            { return Contains(name) ? NodeNamed(name) : NULL; }
        const std::vector<IGraphNode*>& GetNodes() const override
            { return nodes_; }

    private:
        vector<IGraphNode*> nodes_;
        unordered_map<string, OSMNode*> lookup_;
        OSMNode* node_named(const string name) const;

};
};

#endif // OSM_GRAPH_H_