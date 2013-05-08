#include "mathvector.h"

#include <iostream>

namespace mw {

	MathVector MathVector::projectOn(const MathVector& v) const {
		return *(this)*v/(v*v)*v;
	}

	MathVector operator*(double scale, const MathVector& v) {
		return v * scale;
	}

	std::ostream& operator<<(std::ostream& output, const MathVector& v) {
		output << '(' << v.x_ << ", " << v.y_ << ", " << v.z_ << ')';
		return output;
	}

} // Namespace mw
