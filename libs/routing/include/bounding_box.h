#ifndef BOUNDING_BOX_H_
#define BOUNDING_BOX_H_

#include <vector>
#include <iostream>

namespace routing {

struct BoundingBox {
	std::vector<float> min;
	std::vector<float> max;
	std::vector<float> Normalize(std::vector<float> point) const;
	friend std::ostream& operator<<(std::ostream& os, const BoundingBox& bb);
};
std::ostream& operator<<(std::ostream& os, const BoundingBox& bb);
}

#endif
