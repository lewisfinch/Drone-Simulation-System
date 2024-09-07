#ifndef ROBOTOBSERVER_H_
#define ROBOTOBSERVER_H_

#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "IEntity.h"
#include "ISubject.h"
#include "IController.h"
#include "IObserver.h"

/**
 * @class RobotObserver
 * @brief Observer for Robot
 */
class RobotObserver : public IObserver {
 public:
  /**
  * @brief RobotObserver Constructor
  * @param controller IController to send events
  */
  RobotObserver(IController& controller);
  /**
   * @brief Destructor for IObserver class.
   **/
  virtual ~RobotObserver();
  /**
   * @brief Update the event and set message
   * @param message - String to represent the event
   **/
  void update(std::string message);

 private:
  IController& controller;
  std::string message;
  // IEntity &subject;
  std::string name;
  int ID;
  std::list<IEntity *> list_subject;  // robotobserver subscribe all robots
};

#endif
