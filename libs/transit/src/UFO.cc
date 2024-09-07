#define _USE_MATH_DEFINES

#include "UFO.h"

#include <cmath>
#include <limits>

#include "BeelineStrategy.h"
#include "SpinDecorator.h"

UFO::UFO(JsonObject &obj) : details(obj) {
  JsonArray pos(obj["position"]);
  position = {pos[0], pos[1], pos[2]};
  JsonArray dir(obj["direction"]);
  direction = {dir[0], dir[1], dir[2]};

  speed = obj["speed"];
  available = true;
}

UFO::~UFO() {
  // Delete dynamically allocated variables
  delete graph;
  delete target;
  delete toTarget;
}

void UFO::GetTarget(std::vector<IEntity *> scheduler) {
    target = scheduler[0];
    JsonObject targetDetails = target->GetDetails();
    std::string targetName = targetDetails["name"];

    if (target) {
        destination = target->GetPosition();
        Vector3 finalDestination = target->GetDestination();
        toTarget = new BeelineStrategy(position, destination);
        available = false;
        std::string targetMsg = "<<UFO>> Targeting => " + targetName + ".\n";
        Notify(targetMsg);
    }
}

void UFO::Update(double dt, std::vector<IEntity *> scheduler) {
    if (available) GetTarget(scheduler);

    if (toTarget) {
        toTarget->Move(this, dt);

        if (target) {
            delete toTarget;
            destination = target->GetPosition();
            Vector3 finalDestination = target->GetDestination();
            toTarget = new BeelineStrategy(position, destination);
        }

        if (toTarget->IsCompleted()) {
            delete toTarget;
            toTarget = nullptr;
            available = true;
            target->SetMark(true);

            JsonObject targetDetails = target->GetDetails();
            std::string targetName = targetDetails["name"];
            std::string targetMsg = "<<UFO>> Destroyed " + targetName + ".\n";
            Notify(targetMsg);
        }
    }
}

void UFO::Rotate(double angle) {
  Vector3 dirTmp = direction;
  direction.x = dirTmp.x * std::cos(angle) - dirTmp.z * std::sin(angle);
  direction.z = dirTmp.x * std::sin(angle) + dirTmp.z * std::cos(angle);
}
