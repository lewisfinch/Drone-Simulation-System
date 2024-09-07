#ifndef DRONESUBJECT_H_
#define DRONESUBJECT_H_

#include <iostream>
#include <list>
#include <string>

/**
 * @class DroneSubject
 * @brief DroneSubject owns some important state and notifies observer when
 * updated environment
 */
class DroneSubject : public ISubject {
 public:
  ~DroneSubject();

  /**
   * The subscription management methods.
   */
  /**
   * @brief Virtual add new observer/user to subject .
   * @param observer a observer variable
   */
  void Attach(IObserver *observer) override;
  /**
   * @brief Virtual delete new observer/user to subject .
   * @param observer a observer variable
   */
  void Detach(IObserver *observer) override;
  /**
   * @brief Virtual notifies the observer in the subject
   */
  void Notify() override;
  /**
   * @brief initialize the message to be "Empty" at the beginning
   * @param message a string "Empty"
   */
  void CreateMessage(std::string message = "Empty");
  /**
   * @brief calculate the number of observer at the current time
   */
  void HowManyObserver();

 private:
  std::list<IObserver *> list;
  std::string message;
}
#endif
