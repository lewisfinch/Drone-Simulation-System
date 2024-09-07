#include "BatteryFactory.h"

IEntity* BatteryFactory::CreateEntity(JsonObject& entity) {
  std::string type = entity["type"];
  if (type.compare("battery") == 0) {
    std::cout << "Battery Created" << std::endl;
    return new Battery(entity);
  }
  return nullptr;
}
