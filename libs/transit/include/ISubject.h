#ifndef ISUBJECT_H_
#define ISUBJECT_H_

#include <iostream>
#include <string>
#include <vector>

#include "IObserver.h"

/**
 * @class ISubject
 * @brief ISubject owns some important state and notifies observer when updated
 * environment
 */
class ISubject {
 public:
  /**
   * @brief Virtual destructor for IEntity.
   */
  virtual ~ISubject() {}
  /**
   * @brief Virtual add new observer/user to subject .
   * @param observer Type IObserver variable
   */
  virtual void Attach(IObserver *observer) = 0;
  /**
   * @brief Virtual delete new observer/user to subject .
   * @param observer Type IObserver variable
   */
  virtual void Detach(IObserver *observer) = 0;
  /**
   * @brief Virtual notifies the observer in the subject
   * @param message Type string
   */
  virtual void Notify(std::string message) = 0;

 private:
  std::vector<IObserver *> list_observer;
  std::string message;
};
#endif
