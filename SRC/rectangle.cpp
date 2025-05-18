
#include "rectangle.h"


EW_ENTER

namespace geo2d {

std::vector<dashLoop>  rectangle::aLoops(vec2d pnt1,vec2d pnt2) {
	LLeft  = pnt1;
	URight = pnt2;
	std::vector<dashLoop> loops;
	LRight[0] = URight[0];
	LRight[1] = LLeft[1];
	ULeft[0]  = LLeft[0];
	ULeft[1]  = URight[1];
	width     = URight[0] - LLeft[0];
	height    = URight[1] - LLeft[1];
	center    = 0.5 * (LLeft + URight);
	std::vector<Dash> dashesA;
	dashesA.push_back(Dash(LLeft,LRight));
	dashesA.push_back(Dash(LRight,URight));
	dashTrain trainA = dashTrain(dashesA);
	std::vector<Dash> dashesB;
	dashesB.push_back(Dash(URight,ULeft));
	dashesB.push_back(Dash(ULeft,LLeft));
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




