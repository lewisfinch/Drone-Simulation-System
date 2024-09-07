#include "routing/astar.h"
#include "routing/depth_first_search.h"

#include <stdexcept>
#include <unordered_set>
#include <queue>
#include <tuple>
#include <iostream>
#include <functional>
#include <vector>

using namespace std;

namespace routing {

AStar::~AStar() {
    delete cost;
    delete heuristic;
}

template <class T>
class FStack {
    public:
        FStack() {};
        FStack(T elem) : elem(elem), stack(NULL) { };
        FStack(T elem, const FStack* stack) : elem(elem), stack(stack) { };

        FStack* Push(T elem) const {
            return new FStack<T>(elem, this);
        };
        T Top() const {
            return elem;
        };
        vector<T>* ToList() const {
            vector<T>* result;
            if (stack) {
                result = stack->ToList();
            } else {
                result = new vector<T>();
            }
            result->push_back(elem);
            return result;
        }

        T elem;
        const FStack* stack;
};


struct CandidatePath {
    CandidatePath(FStack<string>* path, float distance = 0, float estimate = 0)
        : path(path), distance(distance), estimate(estimate) { };
    FStack<string>* path;
    float distance;
    float estimate;
};

bool compareCandidatePaths(const CandidatePath* path1, const CandidatePath* path2) {
    return (path1->distance + path1->estimate) > (path2->distance + path2->estimate);
};

vector<string> AStar::GetPath(const IGraph* graph, const std::string& from, const std::string& to) const {

    const IGraphNode* start_node = graph->GetNode(from);
    // only here for debugging
    if(!start_node) {
        throw invalid_argument("'from' node not found in graph: " + from);
    }

    const IGraphNode* terminal_node = graph->GetNode(to);
    if(!terminal_node) {
        throw invalid_argument("'to' node not found in graph: " + to);
    }

    unordered_set<string> visited; // don't check nodes we've already visited
    priority_queue<CandidatePath*, vector<CandidatePath*>,
        function<bool(const CandidatePath*, const CandidatePath*)>>
        possible_paths(compareCandidatePaths);


    // visited.insert(from);
    possible_paths.push(
        new CandidatePath(
            new FStack<string>(from),
            0
        ));

    while (!possible_paths.empty()) {
        CandidatePath* candidate = possible_paths.top();
        possible_paths.pop();

        // TODO
        /*
        if (candidate->path->ToList()->size() < 10) {
            std::cerr << "Current Candidate: ";
            for(string str : *(candidate->path->ToList())) {
                std::cerr << " => " << str;
            }
            std::cerr << std::endl;
            std::cerr << "Length: " << candidate->distance << std::endl;
            std::cerr << "Estimate: " << candidate->estimate << std::endl;
        } else {
            while(!possible_paths.empty()) {
                std::cerr << "=============" << "REMAINING PATHS" << "==============" << std::endl;
                CandidatePath* candidate = possible_paths.top();
                possible_paths.pop();

                std::cerr << "Other Candidate: ";
                for(string str : *(candidate->path->ToList())) {
                    std::cerr << " => " << str;
                }
                std::cerr << std::endl;
                std::cerr << "Length: " << candidate->distance << std::endl;
                std::cerr << "Estimate: " << candidate->estimate << std::endl;
            }
            throw invalid_argument("END ME NOW");
        }
        */

        const string path_end = candidate->path->Top();
        // std::cerr << "at: " << path_end << std::endl;
        if (visited.find(path_end) == visited.end()) {
            visited.insert(path_end);

            if(path_end == to) {
                // we found our result
                return *candidate->path->ToList();
            } // implicit else


            const IGraphNode* path_end_node = graph->GetNode(path_end);
            if (!path_end_node) {
                std::cerr << "Encountered node not in graph, named: ";
                std::cerr << path_end << std::endl;
                std::cerr << "Ignoring and continuing" << std::endl;
                continue;
            }

            const vector<IGraphNode*>& next_steps = path_end_node->GetNeighbors();

            for(IGraphNode* next : next_steps) {
                const string next_name = next->GetName();

                possible_paths.push(
                    new CandidatePath(
                        candidate->path->Push(next->GetName()),
                        candidate->distance + cost->Calculate(path_end_node->GetPosition(), next->GetPosition()),
                        heuristic->Calculate(next->GetPosition(), terminal_node->GetPosition())
                    ));
            }
        }
    }
}

std::vector<std::string> DepthFirstSearch::GetPath(const IGraph* graph, const std::string& from, const std::string& to) const {
    unordered_set<string> visited; // don't check nodes we've already visited
    queue<CandidatePath*> possible_paths; // queue of all paths we're considering in BFS

    const IGraphNode* start_node = graph->GetNode(from);
    // only here for debugging
    if(!start_node) {
        throw invalid_argument("'from' node not found in graph: " + from);
    }

    const IGraphNode* terminal_node = graph->GetNode(to);
    if(!terminal_node) {
        throw invalid_argument("'to' node not found in graph: " + to);
    }

    visited.insert(from);
    possible_paths.push(
        new CandidatePath(
            new FStack<string>(from),
            0
        ));

    while(!possible_paths.empty()) {
        auto* path = possible_paths.front();
        possible_paths.pop();

        const string path_end = path->path->Top();
        // was checked to be in the graph when we added it
        // so no need to check

        const IGraphNode* path_end_node = graph->GetNode(path_end);
        if (!path_end_node) {
            std::cerr << "Encountered node not in graph, named: ";
            std::cerr << path_end << std::endl;
            std::cerr << "Ignoring and continuing" << std::endl;
            continue;
        }

        const vector<IGraphNode*> next_steps = path_end_node->GetNeighbors();
        for(IGraphNode* next : next_steps) {
            const string next_name = next->GetName();
            if(next_name == to) {
                // we found our goal
                vector<string> result = *path->path->ToList();
                result.push_back(next_name);
                // NOTE: This currently makes a large memory leak
                // B/C None of the candidate paths, nor their FStack's are freed
                // Not sure how to free the FStacks.
                return result;
            } // implicit else


            if(visited.find(next_name) == visited.end()) {
                // we haven't been to this node yet
                visited.insert(next_name);
                possible_paths.push(
                    new CandidatePath(
                        path->path->Push(next_name)));
            }
        }
    }
}

}