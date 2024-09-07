#include <iostream>
#include "routing_api.h"
#include "image.h"
#include "routing/astar.h"
#include "routing/depth_first_search.h"
#include "routing/dijkstra.h"

void drawPath(Image& image, const routing::BoundingBox& bb, std::vector< std::vector<float> >& path, Color color) {
    std::vector<float> lastPos;
    for (int i = 0; i < path.size(); i++) {
        std::vector<float> pos = bb.Normalize(path[i]);
        if (i > 0) {
            int startX = lastPos[0]*image.GetWidth();
            int startY = lastPos[2]*image.GetHeight();
            int endX = pos[0]*image.GetWidth();
            int endY = pos[2]*image.GetHeight();
            image.DrawLine(startX, startY, endX, endY, color);
        }
        lastPos = pos;
    }
}

int main(int argc, char**argv) {
    using namespace routing;

    if (argc < 3) {
        std::cout << "Usage: ./build/bin/graph_viewer /path/to/graph /path/to/output.png" << std::endl;
        return 0;
    }

    RoutingAPI api;
    const IGraph* graph = api.LoadFromFile(argv[1]);
    if (!graph) { 
        std::cout << "Unable to parse graph file." << std::endl;
        return 1;
    }

    BoundingBox bb = graph->GetBoundingBox();
    std::cout << "Bounding Box: " << bb << std::endl;

    float aspectRatio = (bb.max[2]-bb.min[2])/(bb.max[0] - bb.min[0]);

    int resolution = 1024;
    Image output(resolution,resolution*aspectRatio);
    output.Clear(Color(0,0,0,1));

    const std::vector<IGraphNode*>& nodes = graph->GetNodes();
    for (int i = 0; i < nodes.size(); i++) {
        std::vector<float> normalizedPoint = bb.Normalize(nodes[i]->GetPosition());
        const std::vector<IGraphNode*>& neighbors = nodes[i]->GetNeighbors();
        for (int j = 0; j < neighbors.size(); j++) {
            std::vector<float> neighborPos = bb.Normalize(neighbors[j]->GetPosition());
            int startX = normalizedPoint[0]*output.GetWidth();
            int startY = normalizedPoint[2]*output.GetHeight();
            int endX = neighborPos[0]*output.GetWidth();
            int endY = neighborPos[2]*output.GetHeight();
            output.DrawLine(startX, startY, endX, endY, Color(0.5,0.5,1,1));
        }
    }

    /*for (int i = 0; i < nodes.size(); i++) {
        std::vector<float> normalizedPoint = bb.Normalize(nodes[i]->GetPosition());
        output.SetPixel(normalizedPoint[0]*output.GetWidth(), normalizedPoint[2]*output.GetHeight(), Color(0,0,1,1));
    }*/

    std::vector<float> start = graph->NearestNode(bb.min, EuclideanDistance())->GetPosition();
    std::vector<float> end = graph->NearestNode(bb.max, EuclideanDistance())->GetPosition();
    
    std::vector< std::vector<float> > path = graph->GetPath(start, end, DepthFirstSearch::Default());
    drawPath(output, bb, path, Color(1,0,0,1));
    
    path = graph->GetPath(start, end, AStar::Default());
    drawPath(output, bb, path, Color(0,1,0,1));

    float newHeight = start[2] + (end[2]-start[2])*0.5;
    start[2] = newHeight;
    end[2] = newHeight;
    path = graph->GetPath(start, end, Dijkstra::Default());
    drawPath(output, bb, path, Color(1,0.5,0,1));

    output.SaveAs(argv[2]);

    delete graph;

    return 0;
}

