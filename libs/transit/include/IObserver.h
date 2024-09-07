#ifndef OBSERVER_H_
#define OBSERVER_H_

#include <iostream>
#include <list>
#include <string>
#include <vector>

/**
 * @class IObserver
 * @brief IObserver to print message to observer
 */
class IObserver {
 public:
  /**
   * @brief Virtual destructor for IEntity.
   */
  virtual ~IObserver() {}
  /**
   * @brief Virtual print updated message .
   */
  virtual void update(std::string message) = 0;
};
#endif
