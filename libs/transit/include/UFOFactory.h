#ifndef UFO_FACTORY_H_
#define UFO_FACTORY_H_

#include <vector>

#include "IEntityFactory.h"
#include "UFO.h"

/**
 *@brief UFO Factory to produce UFO class.
 **/
class UFOFactory : public IEntityFactory {
 public:
  /**
   * @brief Destructor for UFOFactory class.
   **/
  virtual ~UFOFactory() {}

  /**
   * @brief Creates entity using the given JSON object, if possible.
   * @param entity - JsonObject to be used to create the new entity.
   * @return Entity that was created if it was created successfully, or a
   *nullpointer if creation failed.
   **/
  IEntity* CreateEntity(JsonObject& entity);
};

#endif
