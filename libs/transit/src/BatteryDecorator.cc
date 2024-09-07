#include "BatteryDecorator.h"

#include <cmath>
#include <limits>
#include <string>
#include <vector>

#include "AstarStrategy.h"
#include "BeelineStrategy.h"
#include "DfsStrategy.h"
#include "DijkstraStrategy.h"
#include "JumpDecorator.h"
#include "SpinDecorator.h"

BatteryDecorator::BatteryDecorator(Drone* new_drone) {
  drone = new_drone;
  power = 100.0;
  haspick = true;
  hasdeliv = false;
  warn = true;
}

void BatteryDecorator::Update(double dt, std::vector<IEntity*> scheduler) {
  if (drone->GetCharging()) {
    if (power < 100) {
      power += 0.05;
      intpower = static_cast<int>(power);
      message = "power: " + std::to_string(intpower);
      Notify(message);
      //      std::cout << power << std::endl;
    } else {
      drone->SetCharging(false);
      message = "...is charging ...\nBingo! Drone is fully charged!\n";
      Notify(message);
      warn = true;
    }
  } else {
    std::string typeTemp = "None";
    if (!scheduler.empty()) {
      JsonObject detailsTemp = scheduler[0]->GetDetails();
      typeTemp = static_cast<std::string>(detailsTemp["type"]);
    }
    if (typeTemp.compare("battery") == 0) {
      if (power <= 30 && !drone->GetToRobot() && !drone->GetToFinal() &&
          !drone->GetToStation()) {
        GetNearestStation(scheduler);
      }
    } else {
      if (power > 0.0) {
        if (power >=
            60.0) {  // check battery first to enable print warning info.
          drone->SetColor("green");
        } else if (power >= 35.0 && power < 60.0) {
          drone->SetColor("yellow");
        } else {
          if (warn) {
            message = "WARNING: Drone Power Low!\nWARNING: Drone Power Low!\n ";
            Notify(message);
            warn = false;
          }
          drone->SetColor("red");
        }

        if (drone->GetAvailability()) {  // loop once availablity--false
          GetNearestEntity(scheduler);
        }
        // std::string robotName = drone->GetNearEntity()->GetDetails()["name"];
        drone->Update(dt, scheduler);  // update print info!!!

        if (drone->GetToFinal() && !drone->GetToRobot() &&
            haspick) {  // Going to pick up robot
          IEntity* robot = drone->GetNearEntity();
          std::string robotName = drone->GetNearEntity()->GetDetails()["name"];
          message = robotName + " is picked up.\n";
          robot->Notify(message);

          message = "Drone is delivering " + robotName +
                    " to its destination\n" + "(Strategy: " + strat + ").\n";
          Notify(message);
          haspick = false;  // now picked up
          hasdeliv = true;  // assigned
        }

        if (drone->CheckMoving()) {
          //          std::cout << "Power:" << power << std::endl;
          power -= 0.001;
          intpower = static_cast<int>(power);
          message = "power: " + std::to_string(intpower);
          Notify(message);
        }
        if (!drone->CheckMoving() && !haspick && hasdeliv &&
            drone->GetAvailability()) {
          message = "Drone has delivered the user to the final destination.\n";
          Notify(message);
          haspick = true;
          hasdeliv = false;
        }
      }
    }
  }
}
void BatteryDecorator::GetNearestEntity(std::vector<IEntity*> scheduler) {
  float minDis = std::numeric_limits<float>::max();
  for (auto entity : scheduler) {
    if (entity->GetAvailability()) {
      Vector3 dronePos = drone->GetPosition();
      float disToEntity = dronePos.Distance(entity->GetPosition());
      if (disToEntity <= minDis) {
        minDis = disToEntity;
        drone->SetNearestEntity(entity);
      }
    }
  }
  if (drone->CheckNearestEntity()) {
    // set availability to the nearest entity
    drone->GetNearEntity()->SetAvailability(false);
    drone->SetAvailability(false);
    drone->SetPickedUp(false);

    drone->SetDestination(drone->GetNearEntity()->GetPosition());
    Vector3 finalDestination = drone->GetNearEntity()->GetDestination();

    drone->SetToRobot(
        new BeelineStrategy(drone->GetPosition(), drone->GetDestination()));
    std::string robotName = drone->GetNearEntity()->GetDetails()["name"];
    message = "Drone is going to pick up " + robotName +
              ".\n";  // is on the way to pick up user
    Notify(message);
    haspick = true;
    hasdeliv = false;
    strat = drone->GetNearEntity()->GetStrategyName();

    if (strat == "astar") {
      drone->SetToFinal(new JumpDecorator(
          new AstarStrategy(drone->GetDestination(), finalDestination, graph)));
      strat = "AStar";
    } else if (strat == "dfs") {
      drone->SetToFinal(new SpinDecorator(new JumpDecorator(
          new DfsStrategy(drone->GetDestination(), finalDestination, graph))));
      strat = "Dfs";
    } else if (strat == "dijkstra") {
      drone->SetToFinal(
          new JumpDecorator(new SpinDecorator(new DijkstraStrategy(
              drone->GetDestination(), finalDestination, graph))));
      strat = "Dijkstra";
    }
  }
}

void BatteryDecorator::GetNearestStation(std::vector<IEntity*> scheduler) {
  float minDis = std::numeric_limits<float>::max();
  for (auto entity : scheduler) {
    Vector3 dronePos = drone->GetPosition();
    float disToEntity = dronePos.Distance(entity->GetPosition());
    if (disToEntity <= minDis) {
      minDis = disToEntity;
      drone->SetNearestEntity(entity);
    }
  }
  if (drone->CheckNearestEntity()) {
    drone->SetAvailability(false);
    drone->SetDestination(drone->GetNearEntity()->GetPosition());
    drone->SetToStation(
        new BeelineStrategy(drone->GetPosition(), drone->GetDestination()));
    message = "Drone is going to charging station. \n";
    Notify(message);
  }
}
