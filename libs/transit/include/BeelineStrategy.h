#ifndef BEELINE_H_
#define BEELINE_H_

#include "IEntity.h"
#include "IStrategy.h"

/**
 * @brief this class inhertis from the IStrategy class and is responsible for
 * generating the beeline that the drone will take.
 */
class BeelineStrategy : public IStrategy {
 public:
  /**
   * @brief Construct a new Beeline Strategy object
   *
   * @param position Starting position
   * @param destination End destination
   */
  BeelineStrategy(Vector3 position, Vector3 destination);

  /**
   * @brief Move toward next position
   *
   * @param entity Entity to move
   * @param dt Delta Time
   */
  void Move(IEntity* entity, double dt);

  /**
   * @brief Check if the trip is completed
   *
   * @return True if complete, false if not complete
   */
  bool IsCompleted();

 private:
  Vector3 position;
  Vector3 destination;
};
#endif  // BEELINE_H_

// quicker way to just inherit off of path strategy:
// don't even need a .cc file

// #include "PathStrategy.h"

// class BeelineStrategy : public PathStrategy {
//   public:
//     BeelineStrategy(Vector3 pos, Vector3 des)
//      : PathStrategy({{pos[0], pos[1], pos[2]}, {des[0], des[1], des[2]}}) {}
// };
