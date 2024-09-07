#include "DroneFactory.h"

#include "BatteryDecorator.h"

IEntity* DroneFactory::CreateEntity(JsonObject& entity) {
  std::string type = entity["type"];
  if (type.compare("drone") == 0) {
    std::cout << "Drone Created" << std::endl;
    Drone* new_drone = new Drone(entity);
    BatteryDecorator* batteryDrone = new BatteryDecorator(new_drone);
    return batteryDrone;
    // eturn new_drone;
  }
  return nullptr;
}
