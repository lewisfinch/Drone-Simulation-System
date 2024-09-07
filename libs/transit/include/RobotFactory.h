#ifndef ROBOT_FACTORY_H_
#define ROBOT_FACTORY_H_

#include <vector>

#include "IEntityFactory.h"
#include "Robot.h"

/**
 *@brief Drone Factory to produce Drones class.
 **/
class RobotFactory : public IEntityFactory {
 public:
  /**
   * @brief Destructor for IEntityFactory class.
   **/
  virtual ~RobotFactory() {}

  /**
   * @brief Creates entity using the given JSON object, if possible.
   * @param entity - JsonObject to be used to create the new entity.
   * @return Entity that was created if it was created successfully, or a
   *nullpointer if creation failed.
   **/
  IEntity* CreateEntity(JsonObject& entity);
};

#endif
