#ifndef UFOBSERVER_H_
#define UFOBSERVER_H_

#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "IEntity.h"
#include "ISubject.h"
#include "IObserver.h"
#include "IController.h"

/**
 * @class UFObserver
 * @brief Observer for UFO
 */
class UFObserver : public IObserver {
 public:
    /**
 * @brief UFObserver Constructor
 * @param controller IController to send events
 */
  UFObserver(IController& controller);

    /**
   * @brief UFObserver Destructor
   */
  virtual ~UFObserver();

    /**
   * @brief Update the event
   * @param message a string message to be sent
   */
  void update(std::string message);

 private:
  IController& controller;
  std::string message;
  std::string name;
  int ID;
};

#endif
