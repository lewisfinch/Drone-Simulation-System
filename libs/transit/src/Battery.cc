#define _USE_MATH_DEFINES
#include "Battery.h"

#include <cmath>
#include <limits>

Battery::Battery(JsonObject& obj) : details(obj) {
  JsonArray pos(obj["position"]);
  position = {pos[0], pos[1], pos[2]};
}

Battery::~Battery() {
  // Delete dynamically allocated variables
  delete graph;
}

void Battery::Rotate(double angle) {
  Vector3 dirTmp = direction;
  direction.x = dirTmp.x * std::cos(angle) - dirTmp.z * std::sin(angle);
  direction.z = dirTmp.x * std::sin(angle) + dirTmp.z * std::cos(angle);
}

void Battery::Update(double dt, std::vector<IEntity*> scheduler) {}
