
#include "polygon.h"


EW_ENTER

namespace geo2d {

std::vector<dashLoop>  polygon::aLoops(int n, std::vector<vec2d> vertexes) {
	Num = n;

	std::vector<dashLoop> loops;
	
	if(Num<3) return loops;
	
	std::vector<Dash> dashesA;
	for(int i = 0; i < (Num + 1)/2; i++) {
		vec2d A = vertexes[i];
		vec2d B = vertexes[i+1];
	    dashesA.push_back(Dash(A,B));
	};
	dashTrain trainA = dashTrain(dashesA);
	
	std::vector<Dash> dashesB;
	for(int i = (Num + 1)/2; i < Num; i++) {
		vec2d A = vertexes[i];
		vec2d B = (i==Num-1) ? vertexes[0] : vertexes[i+1];
	    dashesB.push_back(Dash(A,B));
	};
	dashTrain trainB = dashTrain(dashesB);

	std::vector<dashTrain> trains;
	trains.push_back(trainA);
	trains.push_back(trainB);
	dashLoop loop(trains);
	loops.push_back(loop);
	return loops;
};

std::vector<dashLoop>  polygon::bLoops(int n, double radius) {
	std::vector<vec2d> vertexes;
	for(int i = 0; i < n; i++) {
		double x = radius*std::cos(2.0*M_PI*i/n);
		double y = radius*std::sin(2.0*M_PI*i/n);
		vec2d A = vec2d(x,y);
		vertexes.push_back(A);
	};
        return aLoops(n,vertexes);
};

} // namespace geo2d

EW_LEAVE




