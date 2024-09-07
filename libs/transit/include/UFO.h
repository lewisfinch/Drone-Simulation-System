#ifndef UFO_H_
#define UFO_H_

#include <vector>

#include "IEntity.h"
#include "IStrategy.h"
#include "math/vector3.h"

// Represents a UFO in a physical system.
// UFOs move using euler integration based on a specified
// velocity and direction.
/**
 * @class UFO
 * @brief Represents a UFO in a physical system. UFOs move using euler
 * integration based on a specified velocity and direction.
 */
class UFO : public IEntity {
 public:
  /**
   * @brief UFOs are created with a name
   * @param obj JSON object containing the UFO's information
   */
  UFO(JsonObject& obj);

  /**
   * @brief Destructor
   */
  ~UFO();

  /**
   * @brief Gets the speed of the UFO
   * @return The speed of the UFO
   */
  float GetSpeed() const { return speed; }

  /**
   * @brief Gets the position of the UFO
   * @return The position of the UFO
   */
  Vector3 GetPosition() const { return position; }

  /**
   * @brief Gets the direction of the UFO
   * @return The direction of the UFO
   */
  Vector3 GetDirection() const { return direction; }

  /**
   * @brief Gets the destination of the UFO
   * @return The destination of the UFO
   */
  Vector3 GetDestination() const { return destination; }

  /**
   * @brief Gets the details information of the UFO
   * @return The details information of the UFO
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
  void GetTarget(std::vector<IEntity*> scheduler);

  /**
   * @brief Updates the UFO's position
   * @param dt Delta time
   * @param scheduler Vector containing all the entities in the system
   */
  void Update(double dt, std::vector<IEntity*> scheduler);

  /**
   * @brief Sets the position of the UFO
   * @param pos_ The new position of the UFO
   */
  void SetPosition(Vector3 pos_) { position = pos_; }

  /**
   * @brief Sets the direction of the UFO
   * @param dir_ The new direction of the UFO
   */
  void SetDirection(Vector3 dir_) { direction = dir_; }

  /**
   * @brief Sets the destination of the UFO
   * @param des_ The new destination of the UFO
   */
  void SetDestination(Vector3 des_) { destination = des_; }

  /**
   * @brief Rotates the UFO
   * @param angle The angle by which the UFO should be rotated
   */
  void Rotate(double angle);

  /**
   * @brief Removing the copy constructor and assignment operator
   * so that UFOs cannot be copied.
   */
  UFO(const UFO& UFO) = delete;
  UFO& operator=(const UFO& UFO) = delete;

 private:
  JsonObject details;
  Vector3 position;
  Vector3 direction;
  Vector3 destination;
  float speed;
  IEntity* target = nullptr;
  IStrategy* toTarget = nullptr;
  bool available;
};

#endif
