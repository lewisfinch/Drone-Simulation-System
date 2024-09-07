#ifndef DISTANCE_FUNCTION_H_
#define DISTANCE_FUNCTION_H_

namespace routing {

class DistanceFunction {
public:
	virtual ~DistanceFunction() {}
	virtual float Calculate(const std::vector<float>& a, const std::vector<float>& b) const = 0;
};

class EuclideanDistance : public DistanceFunction {
public:
	virtual ~EuclideanDistance() {}
	virtual float Calculate(const std::vector<float>& a, const std::vector<float>& b) const {
		float sum = 0.0;
		for (int i = 0; i < a.size() && i < b.size(); i++) {
			float dist = (b[i]-a[i]);
			sum += dist*dist;
		}
		return std::sqrt(sum);
	}
};

class ZeroDistance : public DistanceFunction {
public:
	virtual ~ZeroDistance() {}
	virtual float Calculate(const std::vector<float>& a, const std::vector<float>& b) const {
		return 0;
	}
};

}

#endif
