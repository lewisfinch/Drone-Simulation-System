#include "SimulationModel.h"

#include <vector>

#include "BatteryDecorator.h"
#include "BatteryFactory.h"
#include "DroneFactory.h"
#include "HelicopterFactory.h"
#include "HumanFactory.h"
#include "RobotFactory.h"
#include "UFOFactory.h"

SimulationModel::SimulationModel(IController &controller)
    : controller(controller) {
  compFactory = new CompositeFactory();
  // droneObserver = new DroneObserver(controller);
  // robotObserver = new RobotObserver(controller);
  AddFactory(new DroneFactory());
  AddFactory(new RobotFactory());
  AddFactory(new HumanFactory());
  AddFactory(new HelicopterFactory());
  AddFactory(new BatteryFactory());
  AddFactory(new UFOFactory());
}

SimulationModel::~SimulationModel() {
  // Delete dynamically allocated variables
  for (int i = 0; i < entities.size(); i++) {
    delete entities[i];
  }
  for (int i = 0; i < scheduler.size(); i++) {
    delete scheduler[i];
  }
  delete graph;
  delete compFactory;
}

void SimulationModel::CreateEntity(JsonObject &entity) {
  std::string type = entity["type"];
  std::string name = entity["name"];
  JsonArray position = entity["position"];
  std::cout << name << ": " << position << std::endl;

  IEntity *myNewEntity = compFactory->CreateEntity(entity);
  myNewEntity->SetGraph(graph);

  // Call AddEntity to add it to the view
  controller.AddEntity(*myNewEntity);
  if (type.compare("UFO") == 0) {
    UFObserver *alien = new UFObserver(controller);
    myNewEntity->Attach(alien);
    std::string msg = "The Purge is coming!\n";
    myNewEntity->Notify(msg);

    ufo = myNewEntity;
    Purge();
  } else if (type.compare("battery") == 0) {
    stations.push_back(myNewEntity);
    entities.push_back(myNewEntity);
  } else {
    entities.push_back(myNewEntity);
  }

  if (type.compare("drone") == 0) {
    DroneObserver *droneObserver = new DroneObserver(controller);
    // Drone* myNewEntity =dynamic_cast<Drone*>(myNewEntity);
    myNewEntity->Attach(droneObserver);
    std::string msg = "Drone battery power: 100\n";
    myNewEntity->Notify(msg);
    // myNewEntity->GetColor();
  }
  if (type.compare("robot") == 0) {
    RobotObserver *robotObserver = new RobotObserver(controller);
    myNewEntity->Attach(robotObserver);
    std::string msg =
        "Robot " + name +
        " is scheduled.\n";  // be scheduled but arrange by nearest distance
    myNewEntity->Notify(msg);
  }
}

/// Schedules a trip for an object in the scene
void SimulationModel::ScheduleTrip(JsonObject &details) {
  std::string name = details["name"];
  JsonArray start = details["start"];
  JsonArray end = details["end"];
  std::cout << name << ": " << start << " --> " << end << std::endl;

  for (auto entity : entities) {  // Add the entity to the scheduler
    JsonObject detailsTemp = entity->GetDetails();
    std::string nameTemp = detailsTemp["name"];
    std::string typeTemp = detailsTemp["type"];

    if (name.compare(nameTemp) == 0 && typeTemp.compare("robot") == 0 &&
        entity->GetAvailability()) {
      std::string strategyName = details["search"];
      entity->SetDestination(Vector3(end[0], end[1], end[2]));
      entity->SetStrategyName(strategyName);
      scheduler.push_back(entity);
      break;
    }
  }
  controller.SendEventToView("TripScheduled", details);
}

void SimulationModel::Purge() { purge = true; }

/// Updates the simulation
void SimulationModel::Update(double dt) {
  for (int i = 0; i < entities.size(); i++) {
    if (purge == true) {  // Check if ufo is available and ufo toTarget is
                          // freed,
      ufo->Update(dt, entities);
      controller.UpdateEntity(*ufo);
      if (entities[0]->GetMark()) {
        int targetId = entities[0]->GetId();
        //          controller.SendEventToView("Removing entity: ",
        //          entities[0]->GetDetails());
        controller.RemoveEntity(targetId);
        entities.erase(entities.begin());
      }
    } else {
      // std::cout<< "the type is "<<std::endl;
      // std::cout<< entities[i]->GetDetails()["type"] <<std::endl;
      entities[i]->Update(dt, scheduler);
      entities[i]->Update(dt, stations);
      controller.UpdateEntity(*entities[i]);
    }
  }
}

void SimulationModel::AddFactory(IEntityFactory *factory) {
  compFactory->AddFactory(factory);
}
