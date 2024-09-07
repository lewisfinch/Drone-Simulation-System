#include <map>
#include <chrono>
#include "WebServer.h"
#include "SimulationModel.h"
#include "routing_api.h"

//--------------------  Controller ----------------------------

/// A Transit Service that communicates with a web page through web sockets.  It also acts as the controller
/// in the model view controller pattern.
class TransitService : public JsonSession, public IController {
public:
    TransitService(SimulationModel& model) : model(model), start(std::chrono::system_clock::now()), time(0.0) {
        routing::RoutingAPI api;
        routing::IGraph* graph = api.LoadFromFile("libs/routing/data/umn.osm");
        model.SetGraph(graph);
    }

    /// Handles specific commands from the web server
    void ReceiveCommand(const std::string& cmd, JsonObject& data, JsonObject& returnValue) {
        //std::cout << cmd << ": " << data << std::endl;
        if (cmd == "CreateEntity") {
            model.CreateEntity(data);
        }
        else if (cmd == "ScheduleTrip") {
            model.ScheduleTrip(data);
        }
        else if (cmd == "ping") {
            returnValue["response"] = data;
        }
        else if (cmd == "Update") {
            updateEntites.clear();

            std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
            std::chrono::duration<double> diff = end - start;
            double delta = diff.count() - time;
            time += delta;

            double simSpeed = data["simSpeed"];
            delta *= simSpeed;

            if (delta > 0.1) {
                for (float f = 0.0; f < delta; f+=0.01) {
                    model.Update(0.01);
                }
            }
            else {
                model.Update(delta);
            }

            for (std::map<int, const IEntity*>::iterator it = updateEntites.begin(); it != updateEntites.end(); it++) {
                // Send updated entities
                SendEntity("UpdateEntity", *it->second, false);
            }
        }
    }

    void SendEntity(const std::string& event, const IEntity& entity, bool includeDetails) {
        //JsonObject details = entity.GetDetails();
        JsonObject details;
        if (includeDetails) {
            details["details"] = entity.GetDetails();
        }
        details["id"] = entity.GetId();
        Vector3 pos_ = entity.GetPosition();
        Vector3 dir_ = entity.GetDirection();
        JsonArray pos = {pos_.x, pos_.y, pos_.z};
        JsonArray dir = {dir_.x, dir_.y, dir_.z};
        details["pos"] = pos;
        details["dir"] = dir;
        std::string col_ = entity.GetColor();
//        std::cout << col_ << std::endl;
        if(col_ != "None") details["color"] = col_;
        SendEventToView(event, details);
    }

    void AddEntity(const IEntity& entity) {
        SendEntity("AddEntity", entity, true);
    }

    void UpdateEntity(const IEntity& entity) {
        updateEntites[entity.GetId()] = &entity;
    }

    void RemoveEntity(int id) {
        JsonObject details;
        details["id"] = id;
        SendEventToView("RemoveEntity", details);
    }

    void AddPath(int id, const std::vector< std::vector<float> >& path) {
        JsonObject details;
        JsonArray array = details["path"];
        array.Resize(path.size());
        for (int i = 0; i < path.size(); i++) {
            JsonArray point = array[i];
            point.Resize(3);
            point[0] = path[i][0];
            point[1] = path[i][1];
            point[2] = path[i][2];
        }
        SendEventToView("AddPath", details);
    }

    void RemovePath(int id) {
        JsonObject details;
        details["id"] = id;
        SendEventToView("RemovePath", details);
    }

    /// Allows messages to be passed back to the view
    void SendEventToView(const std::string& event, const JsonObject& details) {
        JsonObject eventData;
        eventData["event"] = event;
        eventData["details"] = details;
        sendMessage(eventData.ToString());
    }

private:
    // Simulation Model
    SimulationModel& model;
    // Used for tracking time since last update
    std::chrono::time_point<std::chrono::system_clock> start;
    // The total time the server has been running.
    double time;
    // Current entities to update
    std::map<int, const IEntity*> updateEntites;
};


//--------------------  View / Web Server Code ----------------------------

/// The TransitWebServer holds the simulation and updates sessions.
class TransitWebServer : public WebServerBase, public IController {
public:
	TransitWebServer(int port = 1942, const std::string& webDir = ".") : WebServerBase(port, webDir), model(*this) {}
    void AddEntity(const IEntity& entity) {
        for (int i = 0; i < sessions.size(); i++) {
            static_cast<TransitService*>(sessions[i])->AddEntity(entity);
        }
    }

    void UpdateEntity(const IEntity& entity) {
        for (int i = 0; i < sessions.size(); i++) {
            static_cast<TransitService*>(sessions[i])->UpdateEntity(entity);
        }
    }

    void RemoveEntity(int id) {
        for (int i = 0; i < sessions.size(); i++) {
            static_cast<TransitService*>(sessions[i])->RemoveEntity(id);
        }
    }

    void AddPath(int id, const std::vector< std::vector<float> >& path) {
        for (int i = 0; i < sessions.size(); i++) {
            static_cast<TransitService*>(sessions[i])->AddPath(id, path);
        }
    }

    void RemovePath(int id) {
        for (int i = 0; i < sessions.size(); i++) {
            static_cast<TransitService*>(sessions[i])->RemovePath(id);
        }
    }

    void SendEventToView(const std::string& event, const JsonObject& details) {
        for (int i = 0; i < sessions.size(); i++) {
            static_cast<TransitService*>(sessions[i])->SendEventToView(event, details);
        }
    }

protected:
	Session* createSession() { return new TransitService(model); }
private:
    SimulationModel model;
};

/// The main program that handels starting the web sockets service.
int main(int argc, char**argv) {
    if (argc > 1) {
        int port = std::atoi(argv[1]);
        std::string webDir = std::string(argv[2]);
        TransitWebServer server(port, webDir);
        std::cout << "Schedule page: http://localhost:" << port << "/schedule.html" << std::endl;
        std::cout << "Index page: http://localhost:" << port << "/index.html" << std::endl;
        while (true) {
            server.service();
        }
    }
    else {
        std::cout << "Usage: ./build/bin/transit_service <port> apps/transit_service/web/" << std::endl;
    }

    return 0;
}

