#include "SpinDecorator.h"

SpinDecorator::~SpinDecorator() {
  // Delete dynamically allocated variables
  delete strategy;
}

void SpinDecorator::Move(IEntity* entity, double dt) {
  if (strategy->IsCompleted() && !IsCompleted()) {
    entity->Rotate(dt * 10);
    time += dt;
  } else {
    strategy->Move(entity, dt);
  }
}
