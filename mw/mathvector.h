#ifndef MW_MATHVECTOR_H
#define MW_MATHVECTOR_H

#include <cmath>
#include <iostream>

namespace mw {
	
static const double PI = 3.1415926535897932384626;

class MathVector {
	public:
		double x_,y_,z_;
		
		inline MathVector() {
            x_ = 0;
            y_ = 0;
            z_ = 0;
		}
		
		inline MathVector(double x, double y, double z) {
            x_ = x;
            y_ = y;
            z_ = z;		
		}
		
		inline MathVector(double x, double y) {
            x_ = x;
            y_ = y;
            z_ = 0;
		}
		
		inline MathVector operator*(double scale) const {
            return MathVector(x_*scale, y_*scale, z_*scale);
		}
		
		inline MathVector operator/(double scale) const {
            return MathVector(x_/scale, y_/scale, z_/scale);
		}
		
		inline MathVector operator+(const MathVector& other) const {
            return MathVector(x_+other.x_, y_+other.y_, z_+other.z_);
		}
		
		inline MathVector operator-(const MathVector& other) const {
            return MathVector(x_-other.x_, y_-other.y_, z_-other.z_);
		}
		
		inline MathVector operator-() const {
            return MathVector(-x_, -y_, -z_);
		}
		
		inline const MathVector& operator*=(double scale) {
            x_ *= scale;
            y_ *= scale;
            z_ *= scale;
            return *this;
		}
		
		inline const MathVector& operator/=(double scale) {
            x_ /= scale;
            y_ /= scale;
            z_ /= scale;
            return *this;
		}
		
		inline const MathVector& operator+=(const MathVector& v) {
            x_ += v.x_;
            y_ += v.y_;
            z_ += v.z_;
            return *this;
		}
		
		inline const MathVector& operator-=(const MathVector& v) {
            x_ -= v.x_;
            y_ -= v.y_;
            z_ -= v.z_;
            return *this;
		}		
		
		inline double operator*(const MathVector& v) const {
            return x_*v.x_ + y_*v.y_ + z_*v.z_;
		}
		
		inline double operator[](unsigned int nbr) const {
            switch (nbr) {
            case 0:
                return x_;
            case 1:
                return y_;
            case 2:
                return z_;
            default:
                //throw SdlGameException("Mathvector out of boundarys, operator[]");
				return 0;
            }
		}
		
		inline double& operator[](unsigned int nbr) {
            switch (nbr) {
            case 0:
                return x_;
            case 1:
                return y_;
            case 2:
                return z_;
            default:
				return x_;
                ///throw SdlGameException("Mathvector out of boundarys, operator[]");
            }
		}

		inline double magnitude() const {
            return sqrt(x_*x_+y_*y_+z_*z_);
		}
		
		inline double magnitudeSquared() const {
            return x_*x_+y_*y_+z_*z_;
		}
		
		inline MathVector normalize() const {
            double m = magnitude();
            return MathVector(x_/m,y_/m,z_/m);
		}		
		
		inline MathVector cross(const MathVector& v) const {
            return MathVector(y_*v.z_ - z_*v.y_,
                             -x_*v.z_ + z_*v.x_,
                              x_*v.y_ - y_*v.x_);
		}

		inline MathVector rotate(double angle) const {
			MathVector tmp(x_ * std::cos(angle) - y_ * std::sin(angle), y_ * std::cos(angle) + x_ * std::sin(angle));
			return tmp;
		}
		
		MathVector projectOn(const MathVector& v) const;
};

MathVector operator*(double scale, const MathVector& v);
std::ostream& operator<<(std::ostream& output, const MathVector& v);

} // namespace mw

#endif // MW_MATHVECTOR_H
