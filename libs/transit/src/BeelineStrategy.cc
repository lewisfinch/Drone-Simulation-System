#include "BeelineStrategy.h"

BeelineStrategy::BeelineStrategy(Vector3 position, Vector3 destination)
  : position(position), destination(destination) {}

void BeelineStrategy::Move(IEntity* entity, double dt) {
  if (IsCompleted())
    return;

  Vector3 dir = (destination - position).Unit();

  position = position + dir * entity->GetSpeed() * dt;
  entity->SetPosition(position);
  entity->SetDirection(dir);
}

bool BeelineStrategy::IsCompleted() {
  return position.Distance(destination) < 4.0;
}
