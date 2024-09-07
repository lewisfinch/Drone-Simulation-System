#ifndef ROUTING_POINT3_H_
#define ROUTING_POINT3_H_

#include <cmath>
#include <vector>

namespace routing {

struct Point3 {
  float p[3];

  Point3(float x, float y, float z) {
    p[0] = x;
    p[1] = y;
    p[2] = z;
  }

  Point3(const std::vector<float>& arr) {
    // guess we just hope that it has legnth of at least 3
    p[0] = arr[0];
    p[1] = arr[1];
    p[2] = arr[2];
  }

  float operator[](int index) const { return p[index]; }

  bool operator==(const Point3& other) {
  return this->p[0] == other[0] && this->p[1] == other[1] && this->p[2] == other[2];
  }

  std::vector<float> toVec() const {
      std::vector<float> result(std::begin(p), std::end(p));
      return result;
  }


  float distanceBetween(const Point3 &other) const {
    float dx = this->p[0] - other[0];
    float dy = this->p[1] - other[1];
    float dz = this->p[2] - other[2];
    return sqrt(dx*dx + dy*dy + dz*dz);
  }
};
}

#endif  // ROUTING_POINT3_H_
