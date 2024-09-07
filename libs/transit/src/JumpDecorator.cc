#include "JumpDecorator.h"

JumpDecorator::~JumpDecorator() {
  // Delete dynamically allocated variables
  delete strategy;
}

void JumpDecorator::Move(IEntity* entity, double dt) {
  if (strategy->IsCompleted() && !IsCompleted()) {
    entity->Jump(dt * 10);
    time += dt;
  } else {
    strategy->Move(entity, dt);
  }
}
