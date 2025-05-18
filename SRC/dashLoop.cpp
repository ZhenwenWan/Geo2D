
#include "dashLoop.h"
#include "geometric_structures.h"


EW_ENTER

namespace geo2d {

double dashLoop::getDistanceToPoint( vec2d O ) {
        double ss = MaxiPositive;
	if( !_isValid ) return ss;
	for(auto d : _dashTrains) ss = std::min(ss, d.getDistanceToPoint(O));
        return ss;
};
int dashLoop::getClosestTrainID( vec2d O ) {
        double ss = MaxiPositive;
	int m = _dashTrains.size();
	for(int n = 0; n < _dashTrains.size(); n++) {
		auto train = _dashTrains[n];
		double s = train.getDistanceToPoint(O);
		if(s<ss){ ss =s; m = n;};
	};
        return m;
};

std::vector<Dash> dashLoop::getDashes() { 
	if(_update) return _dashes;

	_dashes.clear();
	auto  trains = getDashTrains();
	for(auto train : trains) {
		auto  dashes = train.getDashes();
		for(auto dash : dashes) {
			bool Yes = true;
			for( auto d : _dashes ) if(d.isDashIdentical(dash)) Yes = false;
			if(Yes) _dashes.push_back(dash);
		};
	};

	return _dashes;
};

std::vector<vec2d> dashLoop::getVertexes() { 
	if(_update) return _vertexes;

	_vertexes.clear();
	auto  dashes = getDashes();
	for(auto dash : dashes) {
		if(!dash.isValid()) continue;
		bool Yes = true;
		auto  P1 = dash.P1;
		for(auto v : _vertexes) if(isP2P(v,P1)) Yes = false;
		if(Yes) _vertexes.push_back(P1);
		Yes = true;
		auto  P2 = dash.P2;
		for(auto v : _vertexes) if(isP2P(v,P2)) Yes = false;
		if(Yes) _vertexes.push_back(P2);
	};

	return _vertexes;
};

int dashLoop::index(vec2d& myVertex) { 
	auto  runs = getVertexes();
	int n = 0;
	for(auto run : runs) {
		if(isP2P(run,myVertex)) break;
		n++;
	};
	return n;
};

int dashLoop::index(Dash& myDash) { 
	auto  runs = getDashes();
	int n = 0;
	for(auto run : runs) {
		if(run.isDashIdentical(myDash)) break;
		n++;
	};
	return n;
};

int dashLoop::index(dashTrain& myTrain) { 
	auto  runs = getDashTrains();
	int n = 0;
	for(auto run : runs) {
		if(run.isTrainIdentical(myTrain)) break;
		n++;
	};
	return n;
};

bool dashLoop::isPointIn( vec2d O ) {
	if( !_isValid ) return false;
	int n = getClosestTrainID( O );
	if( n == _dashTrains.size() ) return false;
	auto train = _dashTrains[n]; 
	return train.isPointOnLeft(O);
};
bool dashLoop::isPointOn( vec2d O ) {
	if( !_isValid ) return false;
	int n = getClosestTrainID( O );
	if( n == _dashTrains.size() ) return false;
	auto train = _dashTrains[n]; 
	return train.isPointOn(O);
};
bool dashLoop::isPointOut( vec2d O ) {
	if( !_isValid ) return false;
	return !(isPointIn(O)||isPointOn(O));
};
bool dashLoop::isLoopCoveredByLoop( dashLoop& myLoop ) {
	auto loop = dashLoop(_dashTrains);
	if( !(loop.isValid() && myLoop.isValid()) ) return false;
	bool ret = true;
	for( auto v : loop.getVertexes() ) if(myLoop.isPointOut(v)) ret = false;  
	for( auto d : loop.getDashes() ) {
		auto dashes = myLoop.loopToolSliceDash(d);  
		for( auto dd : dashes ) 
			if(myLoop.isPointOut(dd.P1)||myLoop.isPointOut(dd.P2)
			||myLoop.isPointOut(0.5*(dd.P1+dd.P2))) ret = false;  
	};
	return ret;
};
bool  dashLoop::isLoopIdentical( dashLoop& loopB ) { 
	auto loopA = dashLoop(_dashTrains);
	if( !(loopA.isValid() && loopB.isValid()) ) return false;
	if( loopA.getDashTrains().size() != loopB.getDashTrains().size() ) return false;
	for(int n = 0; n < loopA.getDashTrains().size(); n++ ) {
		auto trainA = loopA.getDashTrains()[n];
		auto trainB = loopB.getDashTrains()[n];
		if( !trainA.isTrainIdentical(trainB) ) return false;
	};
	return true;
};
bool dashLoop::isLoopInsectLoop( dashLoop& myLoop ) {
	auto loop = dashLoop(_dashTrains);
	if( !(loop.isValid() && myLoop.isValid()) ) return false;

	for( auto dA : loop.getDashes() ) {
		for( auto dB : myLoop.getDashes() ) {
			if( dA.getRelation(dB) != D2D_none ) return true;
		};
	};

	bool ret = false;
	for( auto v : loop.getVertexes() ) if(!myLoop.isPointOut(v)) ret = true;  
	for( auto v : myLoop.getVertexes() ) if(!loop.isPointOut(v)) ret = true;  
	return ret;
};
std::vector<vec2d> dashLoop::getPointsIn( dashTrain& myTrain ) {
        std::vector<vec2d> verts;
	if(!(_isValid&&myTrain.isValid())) return verts;
	for(auto vert : myTrain.getVertexes()) {
		if(isPointIn(vert)) {
			double s = MaxiPositive;
			for(auto v : verts) s = std::min(s,(vert-v).length2());
			if(s > MiniDistance) verts.push_back(vert);
		};
	};
	return verts;
};
std::vector<vec2d> dashLoop::getPointsOn( dashTrain& myTrain ) {
        std::vector<vec2d> verts;
	if(!(_isValid&&myTrain.isValid())) return verts;
	for(auto vert : myTrain.getVertexes()) {
		if(isPointOn(vert)) {
			double s = MaxiPositive;
			for(auto v : verts) s = std::min(s,(vert-v).length2());
			if(s > MiniDistance) verts.push_back(vert);
		};
	};
	return verts;
};
std::vector<vec2d> dashLoop::getPointsOut( dashTrain& myTrain ) {
        std::vector<vec2d> verts;
	if(!(_isValid&&myTrain.isValid())) return verts;
	for(auto vert : myTrain.getVertexes()) {
		if(isPointOut(vert)) {
			double s = MaxiPositive;
			for(auto v : verts) s = std::min(s,(vert-v).length2());
			if(s > MiniDistance) verts.push_back(vert);
		};
	};
	return verts;
};
std::vector<vec2d> dashLoop::getPointsIn( dashLoop& myLoop ) {
        std::vector<vec2d> verts;
	if(!(_isValid&&myLoop.isValid())) return verts;
	std::vector<dashTrain> myTrains = myLoop.getDashTrains();
	for(int n = 0; n < myTrains.size(); n++) {
		std::vector<vec2d> myVerts = getPointsIn(myTrains[n]);
		for(auto vert : myVerts) {
			double s = MaxiPositive;
			for(auto v : verts) s = std::min(s,(vert-v).length2());
			if(s > MiniDistance) verts.push_back(vert);
		};
	};
	return verts;
};
std::vector<vec2d> dashLoop::getPointsOn( dashLoop& myLoop ) {
        std::vector<vec2d> verts;
	if(!(_isValid&&myLoop.isValid())) return verts;
	std::vector<dashTrain> myTrains = myLoop.getDashTrains();
	for(int n = 0; n < myTrains.size(); n++) {
		std::vector<vec2d> myVerts = getPointsOn(myTrains[n]);
		for(auto vert : myVerts) {
			double s = MaxiPositive;
			for(auto v : verts) s = std::min(s,(vert-v).length2());
			if(s > MiniDistance) verts.push_back(vert);
		};
	};
	return verts;
};
std::vector<vec2d> dashLoop::getPointsOut( dashLoop& myLoop ) {
        std::vector<vec2d> verts;
	if(!(_isValid&&myLoop.isValid())) return verts;
	std::vector<dashTrain> myTrains = myLoop.getDashTrains();
	for(int n = 0; n < myTrains.size(); n++) {
		std::vector<vec2d> myVerts = getPointsOut(myTrains[n]);
		for(auto vert : myVerts) {
			double s = MaxiPositive;
			for(auto v : verts) s = std::min(s,(vert-v).length2());
			if(s > MiniDistance) verts.push_back(vert);
		};
	};
	return verts;
};
std::vector<vec2d> dashLoop::getPointsIn( std::vector<vec2d>& myVerts ) {
        std::vector<vec2d> verts;
	for(auto vert : myVerts) {
		if(!isPointIn(vert)) continue;
		double s = MaxiPositive;
		for(auto v : verts) s = std::min(s,(vert-v).length2());
		if(s > MiniDistance) verts.push_back(vert);
	};
	return verts;
};
std::vector<vec2d> dashLoop::getPointsOn( std::vector<vec2d>& myVerts ) {
        std::vector<vec2d> verts;
	for(auto vert : myVerts) {
		if(!isPointOn(vert)) continue;
		double s = MaxiPositive;
		for(auto v : verts) s = std::min(s,(vert-v).length2());
		if(s > MiniDistance) verts.push_back(vert);
	};
	return verts;
};
std::vector<vec2d> dashLoop::getPointsOut( std::vector<vec2d>& myVerts ) {
        std::vector<vec2d> verts;
	for(auto vert : myVerts) {
		if(!isPointOut(vert)) continue;
		double s = MaxiPositive;
		for(auto v : verts) s = std::min(s,(vert-v).length2());
		if(s > MiniDistance) verts.push_back(vert);
	};
	return verts;
};
std::vector<Dash> dashLoop::loopToolSliceDash( Dash& objectDash ) {
	std::vector<Dash> retDashes, objDashes;
	if(!_isValid) {retDashes.push_back(objectDash); return retDashes;};
	for(int n = 0; n < _dashes.size(); n++) {
		auto dash = _dashes[n];
		objDashes.clear();
		if(n==0) {
			objDashes.push_back(objectDash);
		} else {
			for(auto ret : retDashes) objDashes.push_back(ret);
		};
		retDashes.clear();
		for(auto d : objDashes) {
			auto dashes = d.dashSlicedByDash(dash);
			for(auto ret : dashes) retDashes.push_back(ret);
		};
	};
	return retDashes;
};
void  dashLoop::loopToolSliceLoop( dashLoop& loopB, std::vector<dashTrain>& trainsIn, 
		std::vector<dashTrain>& trainsOut, std::vector<dashTrain>& trainsOn) {
	auto loopA = dashLoop(_dashTrains);
	if(!loopA.isValid()||!loopB.isValid()) {
		trainsIn.clear();
		trainsOut.clear();
		trainsOn.clear();
		return;
	};

	std::vector<int> types;
	for(auto train : loopB.getDashTrains()) {
		auto dashes = loopA.loopToolSliceTrain(train);

		types.clear();
		for(auto dash : dashes) {
			int i;
			vec2d mp = 0.5*(dash.P1 + dash.P2);
			if(loopA.isPointOut(dash.P1)||loopA.isPointOut(dash.P2)) {
				i = -1;
			} else if(loopA.isPointIn(dash.P1)||loopA.isPointIn(dash.P2)) {
				i = 1;
			} else if(loopA.isPointOut(mp)) {
				i = -1;
			} else if(loopA.isPointIn(mp)) {
				i = 1;
			} else i = 0;
			types.push_back(i);
		};
		int j1, j2;
		std::vector<Dash> dashesSub;
		for(int i = 0; i < types.size(); i++) {
			if(i==0) {
				dashesSub.clear();
				j1 = types[i];
			};
			j2 = types[i]; 
			if( j2!=j1 ) {
				auto trainSub = dashTrain(dashesSub);
				if(j1==1) trainsIn.push_back(trainSub); 
				if(j1==-1) trainsOut.push_back(trainSub); 
				if(j1==0) trainsOn.push_back(trainSub); 
				dashesSub.clear();
				j1 = types[i];
			};
			if( i==(types.size()-1) ) {
				dashesSub.push_back(dashes[i]);
				auto trainSub = dashTrain(dashesSub);
				if(j1==1) trainsIn.push_back(trainSub); 
				if(j1==-1) trainsOut.push_back(trainSub); 
				if(j1==0) trainsOn.push_back(trainSub); 
				dashesSub.clear();
				j1 = types[i];
			};
			dashesSub.push_back(dashes[i]);
		};
	};
};
std::vector<Dash> dashLoop::loopToolSliceTrain( dashTrain& objectTrain ) {
	if(!_isValid||!objectTrain.isValid()) return objectTrain.getDashes();
	dashTrain myTrain;
	for(int n = 0; n < _dashTrains.size(); n++) {
		auto train = _dashTrains[n];
		if( n== 0 ) {
			auto dDashes = objectTrain.trainSlicedByTrain(train);
			myTrain = dashTrain(dDashes);
		} else {
			auto dDashes = myTrain.trainSlicedByTrain(train);
			myTrain = dashTrain(dDashes);
		};
	};
	return myTrain.getDashes();
};
std::vector<Dash> dashLoop::loopToolSliceTrainOn( dashTrain& objectTrain ) {
        std::vector<Dash> myDashes = loopToolSliceTrain( objectTrain );
        std::vector<Dash> dashes;
	for(auto dash : myDashes) {
		if( !dash.isValid() ) continue;
		if( isPointOn(dash.P1) && isPointOn(dash.P2) && isPointOn(0.5*(dash.P1+dash.P2))) 
		dashes.push_back(dash);
	};
	return dashes;
};
std::vector<Dash> dashLoop::loopToolSliceTrainIn( dashTrain& objectTrain ) {
        std::vector<Dash> myDashes = loopToolSliceTrain( objectTrain );
        std::vector<Dash> dashes;
	for(auto dash : myDashes) {
		if( !dash.isValid() ) continue;
		if( isPointIn(dash.P1) || isPointIn(dash.P2) ) {
			dashes.push_back(dash);
			continue;
		};
		if( isPointOn(dash.P1) && isPointOn(dash.P2) ) {
			if(isPointIn(0.5*(dash.P1 + dash.P2))) {
				dashes.push_back(dash);
				continue;
			};
		};
	};
	return dashes;
};
std::vector<Dash> dashLoop::loopToolSliceTrainOut( dashTrain& objectTrain ) {
        std::vector<Dash> myDashes = loopToolSliceTrain( objectTrain );
        std::vector<Dash> dashes;
	for(auto dash : myDashes) {
		if( !dash.isValid() ) continue;
		if( isPointOut(dash.P1) || isPointOut(dash.P2) ) dashes.push_back(dash);
	};
	for(auto dash : myDashes) {
		if( isPointOn(dash.P1) && isPointOn(dash.P2) ) {
			if(isPointOut(0.5*(dash.P1 + dash.P2))) {
				dashes.push_back(dash);
				continue;
			};
		};
	};
	return dashes;
};
dashTrain dashLoop::trainCutByLoop( dashTrain& objectTrain ) {
	if(!_isValid||!objectTrain.isValid()) return dashTrain();
	vec2d startPoint = objectTrain.getDashes()[0].P1;
	if(isPointOn(startPoint)) return dashTrain();
	auto dDashes = loopToolSliceTrain( objectTrain );
	dashTrain myTrain = dashTrain( dDashes );
	if(!myTrain.isPointOn(startPoint)||!myTrain.isValid()) return dashTrain();
        std::vector<Dash> myDashes;
	for(auto dash : dDashes) {
		myDashes.push_back(dash);
		vec2d PointB = dash.P2;
		if( isPointOn(PointB) ) break;
	};

	return dashTrain(myDashes);
};
dashTrain dashLoop::trainCutByLoopLeft( dashTrain& objectTrain ) {
	if(!_isValid||!objectTrain.isValid()) return objectTrain;
	vec2d startPoint = objectTrain.getDashes()[0].P1;
	if(isPointOn(startPoint)) return objectTrain;
	auto dDashes = loopToolSliceTrain( objectTrain );
	dashTrain myTrain = dashTrain( dDashes );
	if(!myTrain.isPointOn(startPoint)||!myTrain.isValid()) return objectTrain;
        std::vector<Dash> myDashes;
	bool Yes = false;
	for(auto dash : dDashes) {
		vec2d PointA = dash.P1;
		if( isPointOn(PointA) ) Yes = true;
		if( Yes ) myDashes.push_back(dash);
	};
	return dashTrain(myDashes);
};

dashLoop dashLoop::BoolCut( dashLoop loopB ) {
	if(!(_isValid&&loopB.isValid())) return dashLoop();
    std::vector<dashTrain> trainsA, trainsB, trainsOut, trainsIn;
	auto loopA = dashLoop(_dashTrains);

    if(loopA.isLoopCoveredByLoop(loopB)) return dashLoop();
    if(!loopA.isLoopInsectLoop(loopB)) return loopA;

	trainsA = loopA.getDashTrains();
	trainsB = loopB.getDashTrains();

	for(auto trainA : trainsA) {
		auto aDashes = loopB.loopToolSliceTrainOut(trainA);
		while(aDashes.size() > 0) {
			auto train = dashTrain(aDashes);
			trainsOut.push_back(train);
			auto bDashes = train.getDashes();
			if(aDashes.size() > bDashes.size()) {
				auto cDashes = aDashes;
				aDashes.clear();
				for(int n = bDashes.size(); n < cDashes.size(); n++)
					aDashes.push_back(cDashes[n]);
			} else break;
		};
	};

	for(auto trainB : trainsB) {
		auto aDashes = loopA.loopToolSliceTrainIn(trainB);
		while(aDashes.size() > 0) {
			auto train = dashTrain(aDashes);
			trainsIn.push_back(train);
			auto bDashes = train.getDashes();
			if(aDashes.size() > bDashes.size()) {
				auto cDashes = aDashes;
				aDashes.clear();
				for(int n = bDashes.size(); n < cDashes.size(); n++)
					aDashes.push_back(cDashes[n]);
			} else break;
		};
	};

	for(int n = 0; n < trainsIn.size(); n++) {
		int m = trainsIn.size() - n -1;
		auto train = trainsIn[m];
		trainsOut.push_back(train.trainReverse());
	};
	return dashLoop(trainsOut);
};

dashLoop dashLoop::BoolCutLeft( dashLoop loopB ) {
	if(!(_isValid&&loopB.isValid())) return dashLoop();
        std::vector<dashTrain> trainsA, trainsB, trainsOut, trainsIn;
	auto loopA = dashLoop(_dashTrains);

    if(loopA.isLoopCoveredByLoop(loopB)) return loopA;
    if(!loopA.isLoopInsectLoop(loopB)) return dashLoop();

	trainsA = loopA.getDashTrains();
	trainsB = loopB.getDashTrains();

	for(auto trainA : trainsA) {
		auto aDashes = loopB.loopToolSliceTrainIn(trainA);
		while(aDashes.size() > 0) {
			auto train = dashTrain(aDashes);
			trainsIn.push_back(train);
			auto bDashes = train.getDashes();
			if(aDashes.size() > bDashes.size()) {
				auto cDashes = aDashes;
				aDashes.clear();
				for(int n = bDashes.size(); n < cDashes.size(); n++)
					aDashes.push_back(cDashes[n]);
			} else break;
		};
	};

	for(auto trainB : trainsB) {
		auto aDashes = loopA.loopToolSliceTrainIn(trainB);
		while(aDashes.size() > 0) {
			auto train = dashTrain(aDashes);
			trainsIn.push_back(train);
			auto bDashes = train.getDashes();
			if(aDashes.size() > bDashes.size()) {
				auto cDashes = aDashes;
				aDashes.clear();
				for(int n = bDashes.size(); n < cDashes.size(); n++)
					aDashes.push_back(cDashes[n]);
			} else break;
		};
	};

	return dashLoop(trainsIn);
};
dashLoop dashLoop::BoolInt( dashLoop loopB ) {
	if(!(_isValid&&loopB.isValid())) return dashLoop();
    std::vector<dashTrain> trainsA, trainsB, trainsOut, trainsIn;
	auto loopA = dashLoop(_dashTrains);

    if(loopA.isLoopCoveredByLoop(loopB)) return loopA;
    if(loopB.isLoopCoveredByLoop(loopA)) return loopB;

	trainsA = loopA.getDashTrains();
	trainsB = loopB.getDashTrains();

	for(auto trainA : trainsA) {
		auto aDashes = loopB.loopToolSliceTrainIn(trainA);
		while(aDashes.size() > 0) {
			auto train = dashTrain(aDashes);
			trainsIn.push_back(train);
			auto bDashes = train.getDashes();
			if(aDashes.size() > bDashes.size()) {
				auto cDashes = aDashes;
				aDashes.clear();
				for(int n = bDashes.size(); n < cDashes.size(); n++)
					aDashes.push_back(cDashes[n]);
			} else break;
		};
	};

	for(auto trainB : trainsB) {
		auto aDashes = loopA.loopToolSliceTrainIn(trainB);
		while(aDashes.size() > 0) {
			auto train = dashTrain(aDashes);
			trainsIn.push_back(train);
			auto bDashes = train.getDashes();
			if(aDashes.size() > bDashes.size()) {
				auto cDashes = aDashes;
				aDashes.clear();
				for(int n = bDashes.size(); n < cDashes.size(); n++)
					aDashes.push_back(cDashes[n]);
			} else break;
		};
	};

	return dashLoop(trainsIn);
};
dashLoop dashLoop::BoolAdd( dashLoop loopB ) {
	if(!(_isValid&&loopB.isValid())) return dashLoop();
    std::vector<dashTrain> trainsA, trainsB, trainsOut, trainsIn;
	auto loopA = dashLoop(_dashTrains);

    if(loopA.isLoopCoveredByLoop(loopB)) return loopB;
    if(loopB.isLoopCoveredByLoop(loopA)) return loopA;

	trainsA = loopA.getDashTrains();
	trainsB = loopB.getDashTrains();


	for(auto trainA : trainsA) {
		auto aDashes = loopB.loopToolSliceTrainOut(trainA);
		while(aDashes.size() > 0) {
			auto train = dashTrain(aDashes);
			trainsOut.push_back(train);
			auto bDashes = train.getDashes();
			if(aDashes.size() > bDashes.size()) {
				auto cDashes = aDashes;
				aDashes.clear();
				for(int n = bDashes.size(); n < cDashes.size(); n++)
					aDashes.push_back(cDashes[n]);
			} else break;
		};
	};

	for(auto trainB : trainsB) {
		auto aDashes = loopA.loopToolSliceTrainOut(trainB);
		while(aDashes.size() > 0) {
			auto train = dashTrain(aDashes);
			trainsOut.push_back(train);
			auto bDashes = train.getDashes();
			if(aDashes.size() > bDashes.size()) {
				auto cDashes = aDashes;
				aDashes.clear();
				for(int n = bDashes.size(); n < cDashes.size(); n++)
					aDashes.push_back(cDashes[n]);
			} else break;
		};
	};

	return dashLoop(trainsOut);
};

std::vector<dashLoop> dashLoop::VBoolCut( dashLoop loopB ) {
	std::vector<dashLoop> loops;
	std::vector<dashTrain> trainsInA, trainsOutA, trainsOnA;
	std::vector<dashTrain> trainsInB, trainsOutB, trainsOnB;

	auto loopA = dashLoop(_dashTrains);
	if(!loopA.isValid()||!loopB.isValid()) return loops; 

	if(!loopA.isLoopInsectLoop(loopB)) {loops.push_back(loopA); return loops;}; 
	if(loopA.isLoopCoveredByLoop(loopB)) return loops; 

	loopA.loopToolSliceLoop(loopB, trainsInB, trainsOutB, trainsOnB);
	loopB.loopToolSliceLoop(loopA, trainsInA, trainsOutA, trainsOnA);

	for(int n = 0; n < trainsInB.size(); n++) {
		int m = trainsInB.size() - n -1;
		auto train = trainsInB[m];
		trainsOutA.push_back(train.trainReverse());
	};

    LoopCreator myLoopCreator;
    loops = myLoopCreator.createLoops(trainsOutA);
	return loops; 
};

void dashLoop::loopConstruct(std::vector<dashTrain>& myTrains) {
	std::vector<int> workQuene, trainsID;
	int totalTrain = myTrains.size();
	if(totalTrain == 0) return;
	for(int i = 0; i < totalTrain; i++) {
		auto train = myTrains[i];
		if(train.isValid()) {
			workQuene.push_back(i);
		};
	};

	int j, m;
	int validTrain = workQuene.size();
	while(validTrain > 0) {
		for(int i = 0; i < workQuene.size(); i++) {
			j = workQuene[i];
			if( j == totalTrain ) continue;
			if(  trainsID.size() == 0 ) {
				trainsID.push_back(j);
				workQuene[i] = totalTrain;
			};
			m = trainsID.back();
			if(myTrains[m].isTrainConnectedBy(myTrains[j])) {
				trainsID.push_back(j);
				workQuene[i] = totalTrain;
			};
		};
		auto ref = validTrain;
		validTrain = 0;
		for( auto i : workQuene ) if(i < totalTrain) validTrain++;
		if(ref == validTrain) break;
	};

	j = trainsID.front();
	m = trainsID.back();
	if( trainsID.size() > 0 && myTrains[m].isTrainConnectedBy(myTrains[j])) {
		_isValid = true;
	} else trainsID.clear();

	for(int n = 0; n < trainsID.size(); n++) {
		int m = trainsID[n];
	        _dashTrains.push_back(myTrains[m]);
	};
};

void dashLoop::update() {
	_update = false;
	getDashTrains();
	getDashes();
	getVertexes();
	_update = true;
};

void dashLoop::loopTranslate( vec2d& vec ) {
	for(auto& train : _dashTrains) {
		train.trainTranslate( vec);
	};
	update();
};

void dashLoop::loopRotDegree( double& factor ) {
	for(auto& train : _dashTrains) {
		train.trainRotDegree( factor );
	};
	update();
};

void dashLoop::loopZooming( double factorX, double factorY = MaxiDistance ) {
	if(factorY == MaxiDistance) {
		for(auto& train : _dashTrains) train.trainZooming( factorX, factorX );
	} else {
		for(auto& train : _dashTrains) train.trainZooming( factorX, factorY );
	};
	update();
};

void dashLoop::loopPrint(std::string myStr) {
	for(int n = 0; n < _dashTrains.size(); n++) {
		auto uStr = myStr + std::to_string(n) ;
		uStr += " Train " ;
		_dashTrains[n].trainPrint( uStr );
	};
};

} // namespace geo2d

EW_LEAVE




