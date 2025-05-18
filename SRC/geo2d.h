#ifndef GEO2D_H
#define GEO2D_H
#include "geo2d_math.h"
#include "ewa_base/util/polygon.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

EW_ENTER
namespace geo2d {
	const double MiniPositive = 1.0e-20; // <= MiniPositive means zero geometrically.
	const double MaxiPositive = 1.0e+20; // >= MaxiPositive means infinite geometrically.
	const double MiniDistance = 1.0e-10; // <= MiniDistance means two points are identical.
	const double MaxiDistance = 1.0e+10; // >= MaxiDistance means a nonsense distance.
	const double ZERO         = 0.0; 
	enum RelationD2P {
		D2P_none,
		D2P_oneTouch,
	};
	enum RelationD2D {
		D2D_none,
		D2D_oneTouch,
		D2D_multiTouch,
	};
	enum RelationT2D {
		T2D_none,
		T2D_oneTouch,
		T2D_multiTouch,
	};
	
	inline bool isP2P(const vec2d& P1, const vec2d& P2){vec2d PP = P1 - P2; return PP.length2() < MiniDistance;};
	inline double distanceP2P(const vec2d& P1, const vec2d& P2){vec2d PP = P1 - P2; return PP.length();};
	inline bool isPointValid(const vec2d P) { 
		return std::abs(P[0]) < MaxiDistance && std::abs(P[1]) < MaxiDistance; }; 
	inline void pointPrint( std::string myStr, vec2d P) { 
		std::cout << myStr << " : X " << P[0] << " Y " << P[1] << std::endl;
	};
	
	// Function to calculate the cross product of two vectors
	inline double crossProduct(const vec2d vector1, const vec2d vector2) {
	    return vector1[0] * vector2[1] - vector1[1] * vector2[0];
	}
	
	// Function to calculate the dot product of two vectors
	inline double dotProduct(const vec2d vector1, const vec2d vector2) {
	    double result = 0.0;
	    for (int i = 0; i < 2; ++i) {
	        result += vector1[i] * vector2[i];
	    }
	    return result;
	}
	
	// Function to calculate the magnitude of a vector
	inline double vectorMagnitude(const vec2d vector) {
	    double result = 0.0;
	    for (int i = 0; i < 2; ++i) {
	        result += vector[i] * vector[i];
	    }
	    return std::sqrt(result);
	}
	
	// Function to calculate the angle in decimal degrees between two three-dimensional vectors
	inline double calculateAngle(const vec2d vector2, vec2d vector1 = vec2d(1.0,0.0)) {
	    double dotProd = dotProduct(vector1, vector2);
	    double croProd = crossProduct(vector1, vector2);
	    double magV1 = vectorMagnitude(vector1);
	    double magV2 = vectorMagnitude(vector2);
            if(magV1 < MiniDistance || magV2 < MiniDistance) return 0.0;

	    // Calculate the angle in radians
	    double angleRad = std::acos(dotProd / (magV1 * magV2));
	
	    // Convert the angle from radians to degrees
	    double angleDeg = angleRad * (180.0 / M_PI);
	    return (croProd>=0.0) ? angleDeg : 360.0 - angleDeg;
	}

} // geo2d

EW_LEAVE


#endif

