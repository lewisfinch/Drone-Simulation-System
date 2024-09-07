#ifndef JUMP_DECORATOR_H_
#define JUMP_DECORATOR_H_

#include "CelebrationDecorator.h"

/**
 * @brief this class inhertis from the IStrategy class and is represents
 * a celebration decorator where the entity will celebrate according to it.
 */
class JumpDecorator : public CelebrationDecorator {
 public:
  /**
   * @brief Construct a new Jump Decorator object
   *
   * @param strategy the strategy to decorate onto
   */
  JumpDecorator(IStrategy* strategy) : CelebrationDecorator(strategy) {}

  /**
   * @brief JumpDecorator Destructor
   */
  ~JumpDecorator();

  /**
   * @brief Move the entity with the jump behavior for 4 seconds.
   * 
   * @param entity Entity to move
   * @param dt Delta Time
   */
  virtual void Move(IEntity* entity, double dt);
};

#endif  // JUMP_DECORATOR_H_
