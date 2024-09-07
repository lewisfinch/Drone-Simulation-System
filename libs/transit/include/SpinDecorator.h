#ifndef SPIN_DECORATOR_H_
#define SPIN_DECORATOR_H_

#include "CelebrationDecorator.h"

/**
 * @brief this class inhertis from the IStrategy class and is represents
 * a celebration decorator where the entity will celebrate according to it.
 */
class SpinDecorator : public CelebrationDecorator {
 public:
  /**
   * @brief Construct a new Spin Decorator object
   *
   * @param strategy the strategy to decorate onto
   */
  SpinDecorator(IStrategy* strategy) : CelebrationDecorator(strategy) {}

  /**
   * @brief SpinDecorator Destructor
   */
  ~SpinDecorator();

  /**
   * @brief Move the entity with the spin behavior for 4 seconds.
   * 
   * @param entity Entity to move
   * @param dt Delta Time
   */
  virtual void Move(IEntity* entity, double dt);
};

#endif  // SPIN_DECORATOR_H_
