#include "UFObserver.h"

#include <iostream>
#include <list>
#include <string>
#include <vector>

UFObserver::UFObserver(IController &controller) : controller(controller) {
  // this->subject.Attach(this);
  // std::cout << "subscribe a new drone subject: " << subject.GetId() << ".\n";
  // //list_subject.push_back(subject);
  // this->ID = subject.GetId();
}

UFObserver::~UFObserver() {
  std::cout << "Delete the UFObserver " << this->ID << ".\n";
}

void UFObserver::update(std::string message) {
  JsonObject details;
  details["info"] = message;
  controller.SendEventToView("observe", details);
}
