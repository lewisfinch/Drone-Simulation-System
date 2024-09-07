#ifndef Battery_H_
#define Battery_H_

#include "IEntity.h"
#include "IStrategy.h"

/**
 * @class Battery
 * @brief The station for drone to charge on map
 */
class Battery : public IEntity {
 public:
  /**
   * @brief Battery are created with a name
   * @param obj JSON object containing the battery's information
   */
  Battery(JsonObject& obj);

  /**
   * @brief Destroy the Battery object
   */
  ~Battery();
  /**
   * @brief Gets the speed of the battery
   * @return The speed of the battery
   */
  float GetSpeed() const { return speed; }

  /**
   * @brief Gets the position of the battery
   * @return The position of the battery
   */
  Vector3 GetPosition() const { return position; }

  /**
   * @brief Gets the direction of the battery
   * @return The direction of the battery
   */
  Vector3 GetDirection() const { return direction; }

  /**
   * @brief Gets the destination of the battery
   * @return The destination of the battery
   */
  Vector3 GetDestination() const { return destination; }

  /**
   * @brief Gets the details information of the battery
   * @return The details information of the battery
   */
  JsonObject GetDetails() const { return details; }

  /**
   * @brief Updates the battery's position
   * @param dt Delta time
   * @param scheduler Vector containing all the entities in the system
   */
  void Update(double dt, std::vector<IEntity*> scheduler);

  /**
   * @brief Sets the position of the battery
   * @param pos_ The new position of the battery
   */
  void SetPosition(Vector3 pos_) { position = pos_; }

  /**
   * @brief Sets the direction of the battery
   * @param dir_ The new direction of the battery
   */
  void SetDirection(Vector3 dir_) { direction = dir_; }

  /**
   * @brief Sets the destination of the battery
   * @param des_ The new destination of the battery
   */
  void SetDestination(Vector3 des_) { destination = des_; }

  /**
   * @brief Rotates the battery
   * @param angle The angle by which the battery should be rotated
   */
  void Rotate(double angle);

 private:
  JsonObject details;
  Vector3 position;
  Vector3 direction;
  Vector3 destination;
  float speed;
};

#endif
