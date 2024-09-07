#ifndef I_ENTITY_FACTORY_H_
#define I_ENTITY_FACTORY_H_

#include "IEntity.h"
#include "util/json.h"

/**
 *@brief Composite Factory Interface
 **/
class IEntityFactory {
 public:
  /**
   * @brief Destructor for IEntityFactory class.
   **/
  virtual ~IEntityFactory() {}

  /**
   * @brief Creates entity using the given JSON object, if possible.
   * @param entity - JsonObject to be used to create the new entity.
   * @return Entity that was created if it was created successfully, or a
   *nullpointer if creation failed.
   **/
  virtual IEntity* CreateEntity(JsonObject& entity) = 0;
};

#endif
