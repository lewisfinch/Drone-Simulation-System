#ifndef BATTERY_DECORATOR_H_
#define BATTERY_DECORATOR_H_

#include "Drone.h"
#include "IEntity.h"

/**
 * @class BatteryDecorator
 * @brief The battery decorator wrapped on drone
 */
class BatteryDecorator : public IEntity {
 public:
  /**
   * @brief Construct a new Battery Decorator object
   * @param new_drone Drone the decorator is wrapped on
   */
  BatteryDecorator(Drone* new_drone);

  /**
   * @brief BatteryDecorator Destructor
   */
  ~BatteryDecorator() {}

  /**
   * @brief Gets the ID of the entity.
   * @return The ID of the entity.
   */
  virtual int GetId() const { return drone->GetId(); }

  /**
   * @brief Gets the speed of the drone
   * @return The speed of the drone
   */
  float GetSpeed() const { return drone->GetSpeed(); }

  /**
   * @brief Gets the position of the drone
   * @return The position of the drone
   */
  Vector3 GetPosition() const { return drone->GetPosition(); }

  /**
   * @brief Gets the direction of the drone
   * @return The direction of the drone
   */
  Vector3 GetDirection() const { return drone->GetDirection(); }

  /**
   * @brief Gets the destination of the drone
   * @return The destination of the drone
   */
  Vector3 GetDestination() const { return drone->GetDestination(); }

  /**
   * @brief Gets the details information of the drone
   * @return The details information of the drone
   */
  JsonObject GetDetails() const { return drone->GetDetails(); }

  /**
   * @brief Gets the availability of the drone
   * @return The availability of the drone
   */
  bool GetAvailability() const { return drone->GetAvailability(); }

  /**
   * @brief Updates the drone's position
   * @param dt Delta time
   * @param scheduler Vector containing all the entities in the system
   */
  void Update(double dt, std::vector<IEntity*> scheduler);

  /**
   * @brief Set the power of the drone
   * @param p New power of the drone
   */
  void setPower(float p) { power = p; }

  /**
   * @brief Gets the power of the drone
   * @return The power of the drone
   */
  float getPower() { return power; }

  /**
   * @brief Sets the graph object used by the entity in the simulation.
   * @param graph The IGraph object to be used.
   */
  void SetGraph(const IGraph* graph) { drone->SetGraph(graph); }

  /**
   * @brief Gets the nearest entity in the scheduler
   * @param scheduler Vector containing all the entities in the system
   */
  void GetNearestEntity(std::vector<IEntity*> scheduler);

  /**
   * @brief Gets the nearest charging station in the system
   * @param scheduler Vector containing all the charging stations in the system
   */
  void GetNearestStation(std::vector<IEntity*> scheduler);

  /**
   * @brief Set color of the drone
   * @param col_ New color of the drone
   */
  void SetColor(std::string col_) { drone->SetColor(col_); }

  /**
   * @brief Gets the color of the drone
   * @return The color of the drone
   */
  std::string GetColor() const { return drone->GetColor(); }

 protected:
  float power;
  int intpower;
  Drone* drone;
  bool haspick;   // true,can pick up; false,already picked up
  bool hasdeliv;  // true,already finish;false,processing/unassigned
  bool warn;      // true,enable alerting; false, alreay warned
  std::string strat;
};

#endif  // BATTERY_DECORATOR_H_
