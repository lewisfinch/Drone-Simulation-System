#include "math/vector3.h"

/**
 * @brief Default constructor.
 */
Vector3::Vector3() {
  x = 0.0;
  y = 0.0;
  z = 0.0;
}
Vector3::Vector3(float x_, float y_, float z_) {
  x = x_;
  y = y_;
  z = z_;
}

Vector3 Vector3::operator+(const Vector3& v) {
  return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3 Vector3::operator-(const Vector3& v) {
  return Vector3(this->x - v.x, this->y - v.y, this->z - v.z);
}

Vector3 Vector3::operator*(float m) {
  return Vector3(m * this->x, m * this->y, m * this->z);
}

Vector3 Vector3::operator/(float m) {
  return Vector3(this->x / m, this->y / m, this->z / m);
}

void Vector3::Print() {
  std::cout << "[" << this->x << ", " << this->y << ", " << this->z << "]"
            << std::endl;
}