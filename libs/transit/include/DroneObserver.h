#ifndef DRONEOBSERVER_H_
#define DRONEOBSERVER_H_

#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "IEntity.h"
#include "ISubject.h"
#include "IObserver.h"
#include "IController.h"

/**
 * @class DroneObserver
 * @brief Observer for Drone
 */
class DroneObserver : public IObserver {
 public:
  /**
   * @brief DroneObserver Constructor
   * @param controller IController to send events
   */
  DroneObserver(IController& controller);
   /**
   * @brief Destructor for IObserver class.
   **/
  virtual ~DroneObserver();
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
  // std::list<IEntity *> list_subject;  // droneobserver subscribe all drone
};

#endif
