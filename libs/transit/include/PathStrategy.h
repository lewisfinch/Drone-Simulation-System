#ifndef PATH_STRATEGY_H_
#define PATH_STRATEGY_H_

#include "IStrategy.h"

/**
 * @brief this class inhertis from the IStrategy class and is represents
 * a movement strategy where the entity simply moves along the given path
 */
class PathStrategy : public IStrategy {
 protected:
  std::vector<std::vector<float>> path;
  int index;

 public:
  /**
   * @brief Construct a new PathStrategy Strategy object
   *
   * @param path the path to follow
   */
  PathStrategy(std::vector<std::vector<float>> path = {});

  /**
   * @brief Move toward next position in the path
   *
   * @param entity Entity to move
   * @param dt Delta Time
   */
  virtual void Move(IEntity* entity, double dt);

  /**
   * @brief Check if the trip is completed by seeing if index 
   *        has reached the end of the path
   *
   * @return True if complete, false if not complete
   */
  virtual bool IsCompleted();
};

#endif  // PATH_STRATEGY_H_
