#include "RobotObserver.h"

#include <iostream>
#include <list>
#include <string>
#include <vector>

RobotObserver::RobotObserver(IController& controller) : controller(controller) {
  // this->subject.Attach(this);
  // std::cout << "subscribe a new robot subject: " << subject.GetId() << ".\n";
  // //list_subject.push_back(subject);
  // this->ID = subject.GetId();
}
RobotObserver::~RobotObserver() {
  std::cout << "Delete the RobotObserver " << this->ID << ".\n";
}

void RobotObserver::update(std::string message) {
  // print message in the sidebar UI
  // std::cout<< message <<std::endl;
  JsonObject details;
  details["info"] = message;
  controller.SendEventToView("observe", details);
}
