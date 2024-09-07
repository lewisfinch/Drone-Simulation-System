#include "RobotFactory.h"

IEntity* RobotFactory::CreateEntity(JsonObject& entity) {
  std::string type = entity["type"];
  if (type.compare("robot") == 0) {
    std::cout << "Robot Created" << std::endl;
    return new Robot(entity);
  }
  return nullptr;
}
