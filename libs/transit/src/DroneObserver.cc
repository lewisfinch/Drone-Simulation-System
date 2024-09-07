#include "DroneObserver.h"

#include <iostream>
#include <list>
#include <string>
#include <vector>

DroneObserver::DroneObserver(IController& controller) : controller(controller) {
  // this->subject.Attach(this);
  // std::cout << "subscribe a new drone subject: " << subject.GetId() << ".\n";
  // //list_subject.push_back(subject);
  // this->ID = subject.GetId();
}
DroneObserver::~DroneObserver() {
  // this->subject.Detach(this);
  std::cout << "Delete the DroneObserver " << this->ID << ".\n";
}

void DroneObserver::update(std::string message) {
  // print message in the sidebar UI
  // std::cout<< message <<std::endl;
  JsonObject details;
  details["info"] = message;
  if (message.find("power") != std::string::npos) {
    controller.SendEventToView("power", details);
  } else {
    controller.SendEventToView("observe", details);
  }
}
