#include "HumanFactory.h"

IEntity* HumanFactory::CreateEntity(JsonObject& entity) {
  std::string type = entity["type"];
  if (type.compare("human") == 0) {
    std::cout << "Human Created" << std::endl;
    return new Human(entity);
  }
  return nullptr;
}
