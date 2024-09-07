#include "CelebrationDecorator.h"

CelebrationDecorator::CelebrationDecorator(IStrategy* strategy) {
  this->strategy = strategy;
  time = 0;
}

CelebrationDecorator::~CelebrationDecorator() {
  // Delete dynamically allocated variables
  delete strategy;
}

bool CelebrationDecorator::IsCompleted() {
  if (time >= 4.0) {
    return true;
  } else {
    return false;
  }
}
