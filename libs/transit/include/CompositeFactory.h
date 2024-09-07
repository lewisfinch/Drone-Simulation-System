#ifndef COMPOSITE_FACTORY_H_
#define COMPOSITE_FACTORY_H_

#include "IEntityFactory.h"

/**
 *@brief Factory method for composite class. Inherits from IEntityFactory.
 **/
class CompositeFactory : public IEntityFactory {
 public:
  /**
   * @brief Creates entity using the given JSON object, if possible.
   * @param entity - JsonObject to be used to create the new entity.
   * @return Entity that was created if it was created successfully, or a
   *nullpointer if creation failed.
   **/
  IEntity* CreateEntity(JsonObject& entity);

  /**
   * @brief Adds given factory
   * @param factoryEntity - Factory to be added.
   **/
  void AddFactory(IEntityFactory* factoryEntity);

  /**
   * @brief Destructor for IEntityFactory class.
   **/
  virtual ~CompositeFactory();

 private:
  std::vector<IEntityFactory*> componentFactories;
};

#endif
