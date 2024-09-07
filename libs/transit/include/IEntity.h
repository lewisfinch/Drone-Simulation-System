#ifndef ENTITY_H_
#define ENTITY_H_

#include <vector>

#include "IObserver.h"
#include "ISubject.h"
#include "graph.h"
#include "math/vector3.h"
#include "util/json.h"

using namespace routing;

/**
 * @class IEntity
 * @brief Represents an entity in a physical system.
 *
 * An IEntity object has a unique ID, a position, a direction, a destination,
 * and details. It also has a speed, which determines how fast the entity moves
 * in the physical system. Subclasses of IEntity can override the `Update`
 * function to implement their own movement behavior.
 */
class IEntity : public ISubject {
 public:
  /**
   * @brief Constructor that assigns a unique ID to the entity.
   */
  IEntity() {
    static int currentId = 0;
    id = currentId;
    currentId++;
  }

  /**
   * @brief Virtual destructor for IEntity.
   */
  virtual ~IEntity() {
    delete graph;
    std::cout << "Delete the subject.\n";
  }

  /**
   * @brief Virtual add new observer/user to subject .
   */
  void Attach(IObserver* observer) { list_observer.push_back(observer); }
  /**
   * @brief Virtual delete new observer/user to subject .
   */
  void Detach(IObserver* observer) {
    for (auto it = list_observer.begin(); it != list_observer.end(); ++it) {
      if (*it == observer) {
        list_observer.erase(it);
      }
    }
  }
  /**
   * @brief Virtual notifies the observer subscribe the subject
   * @param message Type string to set the new message that should shown on screen
   */
  void Notify(std::string message) override {
    for (auto observer : list_observer) {
      observer->update(message);
    }
  }
/**
   * @brief assign the message to initialized message
   * @param message Type string to set the message be "Empty"
   */
  void createMessage(std::string message = "Empty") {
    this->message = message;
    Notify(message);
  }
  /**
   * @brief A function that print the number of observers in the list
   */
  void HowManyObserver() {
    std::cout << "There are " << list_observer.size()
              << "observers in the list.\n";
  }
  /**
   * @brief Gets the ID of the entity.
   * @return The ID of the entity.
   */
  virtual int GetId() const { return id; }

  /**
   * @brief Gets the position of the entity.
   * @return The position of the entity.
   */
  virtual Vector3 GetPosition() const = 0;

  /**
   * @brief Gets the direction of the entity.
   * @return The direction of the entity.
   */
  virtual Vector3 GetDirection() const = 0;

  /**
   * @brief Gets the destination of the entity.
   * @return The destination of the entity.
   */
  virtual Vector3 GetDestination() const = 0;

  /**
   * @brief Gets the details of the entity.
   * @return The details of the entity.
   */
  virtual JsonObject GetDetails() const = 0;

  /**
   * @brief Gets the color of the drone
   * @return The color of the drone
   */
  virtual std::string GetColor() const {return "None"; }

  /**
   * @brief Gets the speed of the entity.
   * @return The speed of the entity.
   */
  virtual float GetSpeed() const = 0;

  /**
   * @brief Gets the availability of the entity.
   * @return The availability of the entity.
   */
  virtual bool GetAvailability() const {}

  /**
   * @brief Get the Strategy Name
   *
   * @return Streategy name
   */
  virtual std::string GetStrategyName() const {}

  /**
   * @brief Set the Strategy Name
   *
   * @param strategyName_ Strategy name
   */
  virtual void SetStrategyName(std::string strategyName_) {}

  /**
   * @brief Sets the availability of the entity.
   * @param choice The desired availability of the entity.
   */
  virtual void SetAvailability(bool choice) {}

  /**
   * @brief Updates the entity's position in the physical system.
   * @param dt The time step of the update.
   * @param scheduler The list of all entities in the system.
   */
  virtual void Update(double dt, std::vector<IEntity*> scheduler) {}

  /**
   * @brief Sets the graph object used by the entity in the simulation.
   * @param graph The IGraph object to be used.
   */
  void SetGraph(const IGraph* graph) { this->graph = graph; }

  /**
   * @brief Sets the position of the entity.
   * @param pos_ The desired position of the entity.
   */
  virtual void SetPosition(Vector3 pos_) {}

  /**
   *@brief Set the direction of the entity.
   *@param dir_ The new direction of the entity.
   */
  virtual void SetDirection(Vector3 dir_) {}

  /**
   *@brief Set the destination of the entity.
   *@param des_ The new destination of the entity.
   */
  virtual void SetDestination(Vector3 des_) {}

  /**
   * @brief Sets the color of the drone
   * @param col_ The new color of the drone
   */
  virtual void SetColor(std::string col_) {}

  /**
   * @brief Rotate the entity.
   * @param angle The angle to rotate the entity by.
   */
  virtual void Rotate(double angle) {}

  /**
   * @brief Make the entity jump.
   * @param height The height to make the entity jump.
   */
  virtual void Jump(double height) {}
  /**
   * @brief Get a random float number within min and max
   * @param Min Type float number
   * @param Max Type float number
   * @return Return a float value
   */
  virtual float Random(float Min, float Max) {
    static unsigned int seed = time(NULL);
    unsigned int state = seed;
    seed = rand_r(&state);
    return ((static_cast<float>(seed) / static_cast<float>(RAND_MAX))
     * (Max - Min)) + Min;
  }

  /**
   * @brief Get if the entity is marked by UFO
   * @return Return true if marked
   */
  bool GetMark() const { return marked; }

  /**
   * @brief Mark the entity
   * @param mark set marked to true if marked by UFO
   */
  void SetMark(bool mark) { marked = mark; }

 protected:
  int id;
  const IGraph* graph;
  std::string message;
  std::list<IObserver*> list_observer;
  bool marked = false;
};

#endif
