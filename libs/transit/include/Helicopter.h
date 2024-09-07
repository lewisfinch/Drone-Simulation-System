#ifndef Helicopter_H_
#define Helicopter_H_

#include "IEntity.h"
#include "IStrategy.h"

/**
 * @class Helicopter
 * @brief A helicopter fly randomly on map
 */
class Helicopter : public IEntity {
 public:
  /**
   * @brief Helicopter are created with a name
   * @param obj JSON object containing the Helicopter's information
   */
  Helicopter(JsonObject& obj);

  /**
   * @brief Destroy the Helicopter object
   */
  ~Helicopter();
  /**
   * @brief Gets the speed of the Helicopter
   * @return The speed of the Helicopter
   */
  float GetSpeed() const { return speed; }

  /**
   * @brief Gets the position of the Helicopter
   * @return The position of the Helicopter
   */
  Vector3 GetPosition() const { return position; }

  /**
   * @brief Gets the direction of the Helicopter
   * @return The direction of the Helicopter
   */
  Vector3 GetDirection() const { return direction; }

  /**
   * @brief Gets the destination of the Helicopter
   * @return The destination of the Helicopter
   */
  Vector3 GetDestination() const { return destination; }

  /**
   * @brief Gets the details information of the Helicopter
   * @return The details information of the Helicopter
   */
  JsonObject GetDetails() const { return details; }

  /**
   * @brief Updates the Helicopter's position
   * @param dt Delta time
   * @param scheduler Vector containing all the entities in the system
   */
  void Update(double dt, std::vector<IEntity*> scheduler);

  /**
   * @brief Sets the position of the Helicopter
   * @param pos_ The new position of the Helicopter
   */
  void SetPosition(Vector3 pos_) { position = pos_; }

  /**
   * @brief Sets the direction of the Helicopter
   * @param dir_ The new direction of the Helicopter
   */
  void SetDirection(Vector3 dir_) { direction = dir_; }

  /**
   * @brief Sets the destination of the Helicopter
   * @param des_ The new destination of the Helicopter
   */
  void SetDestination(Vector3 des_) { destination = des_; }

  /**
   * @brief Create a new destination for helicopter
   */
  void CreateNewDestination();

  /**
   * @brief Rotates the Helicopter
   * @param angle The angle by which the Helicopter should be rotated
   */
  void Rotate(double angle);

 private:
  JsonObject details;
  Vector3 position;
  Vector3 direction;
  Vector3 destination;
  float speed;
  IStrategy* toDestination = nullptr;
};

#endif
