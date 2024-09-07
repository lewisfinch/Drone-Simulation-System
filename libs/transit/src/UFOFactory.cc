#include "UFOFactory.h"

IEntity* UFOFactory::CreateEntity(JsonObject& entity) {
  std::string type = entity["type"];
  if (type.compare("UFO") == 0) {
    std::cout << "UFO Created" << std::endl;
    return new UFO(entity);
  }
  return nullptr;
}
