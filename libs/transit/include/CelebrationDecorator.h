#ifndef CELEBRATION_DECORATOR_H_
#define CELEBRATION_DECORATOR_H_

#include "IStrategy.h"

/**
 * @brief this class inhertis from the IStrategy class and is represents
 * a celebration decorator where the entity will celebrate according to it.
 */
class CelebrationDecorator : public IStrategy {
 protected:
  IStrategy* strategy;
  float time;

 public:
  /**
   * @brief Construct a new Celebration Decorator object
   *
   * @param strategy the strategy to decorate onto
   */
  CelebrationDecorator(IStrategy* strategy);


  /**
   * @brief Celebration Destructor
   */
  ~CelebrationDecorator();

  /**
   * @brief Move the entity with the behavior as described for 4 seconds.
   * 
   * @param entity Entity to move
   * @param dt Delta Time
   */
  virtual void Move(IEntity* entity, double dt) = 0;

  /**
   * @brief Check if the movement is completed by checking the time.
   *
   * @return True if complete, false if not complete
   */
  virtual bool IsCompleted();
};

#endif  // CELEBRATION_DECORATOR_H_
