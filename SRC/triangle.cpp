
#include "triangle.h"


EW_ENTER

namespace geo2d {

std::vector<dashLoop>  triangle::aLoops(vec2d p1,vec2d p2,vec2d p3) {
	A = p1;
	B = p2;
	C = p3;
	std::vector<dashLoop> loops;
	std::vector<Dash> dashesA;
	dashesA.push_back(Dash(A,B));
	dashesA.push_back(Dash(B,C));
	dashTrain trainA = dashTrain(dashesA);
	std::vector<Dash> dashesB;
	dashesB.push_back(Dash(C,A));
	dashTrain trainB = dashTrain(dashesB);
	std::vector<dashTrain> trains;
	trains.push_back(trainA);
	trains.push_back(trainB);
	dashLoop loop(trains);
	loops.push_back(loop);
	return loops;
};

} // namespace geo2d

EW_LEAVE




