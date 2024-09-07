#ifndef DRONE_H_
#define DRONE_H_

#include <vector>

#include "IEntity.h"
#include "IStrategy.h"
#include "math/vector3.h"

// Represents a drone in a physical system.
// Drones move using euler integration based on a specified
// velocity and direction.
/**
 * @class Drone
 * @brief Represents a drone in a physical system. Drones move using euler
 * integration based on a specified velocity and direction.
 */
class Drone : public IEntity {
 public:
  /**
   * @brief Drones are created with a name
   * @param obj JSON object containing the drone's information
   */
  Drone(JsonObject& obj);

  /**
   * @brief Destructor
   */
  ~Drone();

  /**
   * @brief Gets the speed of the drone
   * @return The speed of the drone
   */
  float GetSpeed() const { return speed; }

  /**
   * @brief Gets the position of the drone
   * @return The position of the drone
   */
  Vector3 GetPosition() const { return position; }

  /**
   * @brief Gets the direction of the drone
   * @return The direction of the drone
   */
  Vector3 GetDirection() const { return direction; }

  /**
   * @brief Gets the destination of the drone
   * @return The destination of the drone
   */
  Vector3 GetDestination() const { return destination; }

  /**
   * @brief Gets the color of the drone
   * @return The color of the drone
   */
  std::string GetColor() const { return color; }

  /**
   * @brief Gets the details information of the drone
   * @return The details information of the drone
   */
  JsonObject GetDetails() const { return details; }

  /**
   * @brief Gets the availability of the drone
   * @return The availability of the drone
   */
  bool GetAvailability() const { return available; }

  /**
   * @brief Gets the nearest entity in the scheduler
   * @param scheduler Vector containing all the entities in the system
   */
  void GetNearestEntity(std::vector<IEntity*> scheduler);

  /**
   * @brief Updates the drone's position
   * @param dt Delta time
   * @param scheduler Vector containing all the entities in the system
   */
  void Update(double dt, std::vector<IEntity*> scheduler);

  /**
   * @brief Sets the position of the drone
   * @param pos_ The new position of the drone
   */
  void SetPosition(Vector3 pos_) { position = pos_; }

  /**
   * @brief Sets the direction of the drone
   * @param dir_ The new direction of the drone
   */
  void SetDirection(Vector3 dir_) { direction = dir_; }

  /**
   * @brief Sets the destination of the drone
   * @param des_ The new destination of the drone
   */
  void SetDestination(Vector3 des_) { destination = des_; }

  /**
   * @brief Sets the color of the drone
   * @param col_ The new color of the drone
   */
  void SetColor(std::string col_) { color = col_; }

  /**
   * @brief Rotates the drone
   * @param angle The angle by which the drone should be rotated
   */
  void Rotate(double angle);

  /**
   * @brief Makes the drone jump
   * @param height The height at which the drone should jump
   */
  void Jump(double height);

  /**
   * @brief Removing the copy constructor and assignment operator
   * so that drones cannot be copied.
   */
  Drone(const Drone& drone) = delete;
  Drone& operator=(const Drone& drone) = delete;

  /**
   * @brief Sets the NearestEntity of the drone
   * @param ne The new NearestEntity of the drone
   */
  void SetNearestEntity(IEntity* ne) { nearestEntity = ne; }

  /**
   * @brief Check if the drone has a NearestEntity now
   * @return if the drone has a NearestEntity
   */
  bool CheckNearestEntity();

  /**
   * @brief Sets the Availability of the drone
   * @param avail New availability of the drone
   */
  void SetAvailability(bool avail) { available = avail; }

  /**
   * @brief Sets the PickedUp of the drone
   * @param pickup if the drone picked up a robot
   */
  void SetPickedUp(bool pickup) { pickedUp = pickup; }

  /**
   * @brief Sets the strategy to fly to robot
   * @param strat The strategy used for flying to robot
   */
  void SetToRobot(IStrategy* strat) { toRobot = strat; }

  /**
   * @brief Sets the strategy to fly to robot's destination
   * @param strat The strategy used for flying to robot's destination
   */
  void SetToFinal(IStrategy* strat) { toFinalDestination = strat; }

  /**
   * @brief Sets the strategy to fly to charging station
   * @param strat The strategy used for flying to charging station
   */
  void SetToStation(IStrategy* strat) { toStation = strat; }

  /**
   * @brief Check if the drone already have ToRobot strategy
   * @return if ToRobot strategy is set
   */
  bool GetToRobot();

  /**
   * @brief Check if the drone already have ToFinalDestination strategy
   * @return if ToFinalDestination strategy is set
   */
  bool GetToFinal();

  /**
   * @brief Check if the drone already have ToStation strategy
   * @return if ToStation strategy is set
   */
  bool GetToStation();

  /**
   * @brief Check if the drone is moving
   * @return if the drone is moving
   */
  bool CheckMoving();

  /**
   * @brief Set whether the drone is charging
   * @param c if the drone is charging
   */
  void SetCharging(bool c) { charging = c; }

  /**
   * @brief Get if the drone is charging
   * @return if the drone is charging
   */
  bool GetCharging() { return charging; }

  /**
   * @brief Get the NearestEntity of the drone
   * @return the NearestEntity of the drone
   */
  IEntity* GetNearEntity() { return nearestEntity; }


 private:
  JsonObject details;
  Vector3 position;
  Vector3 direction;
  std::string color = "None";  // None means default color
  float jumpHeight = 0;
  bool goUp = true;  // jump helper
  Vector3 destination;
  float speed;
  bool available;
  bool pickedUp;
  IEntity* nearestEntity = nullptr;
  IStrategy* toRobot = nullptr;
  IStrategy* toFinalDestination = nullptr;
  IStrategy* toStation = nullptr;
  bool charging = false;
};

#endif
