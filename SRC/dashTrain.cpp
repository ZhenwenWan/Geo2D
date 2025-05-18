#include "dashTrain.h"

EW_ENTER

namespace geo2d {

dashTrain::dashTrain( std::vector<Dash>& dashes) {
	for(int n = 0; n < dashes.size(); n++) {
		Dash dash  = dashes[n];
		if(!dash.isValid()) continue;
		addDash(dash);
	};
	update();
};
void dashTrain::update( ) {
	_update = false;
	getVertexes();
	_update = true;
};
std::vector<vec2d> dashTrain::getVertexes( ) {
	if(_update) return _vertexes;

	_vertexes.clear();
	for(int n = 0; n < _dashes.size(); n++) {
		Dash dash  = _dashes[n];
		if(!dash.isValid()) continue;
		addVertex(dash.P1);
		addVertex(dash.P2);
	};
	return _vertexes;
};
bool dashTrain::addVertex(vec2d V)
{
	for(const auto vec : _vertexes) 
	{
	        if( isP2P(vec,V) ) return false;
        };
	_vertexes.push_back(V);
	return true;
};
bool dashTrain::addDash(Dash dash)
{
	if(!dash.isValid()) return false;
	if(_dashes.size()==0) {_dashes.push_back(dash); return true;};
	for(auto d : _dashes) if(d.isDashIdentical(dash)) return false;
	if(_dashes.back().isDashConnected(dash)) {
        	_dashes.push_back(dash);
	        return true;
	} else  return false;
};

bool dashTrain::isClosed()
{
	_isClosed = false;
	for(int n = 0; n < _dashes.size(); n++) 
	{
		Dash d1 = _dashes[n];
	        for(int m = n+1; m < _dashes.size(); m++) 
	        {
	        	Dash d2 = _dashes[m];
			if( d1.isDashCrossIn(d2) ) _isClosed = true;
                };
        };
	if(_dashes.size() > 0) {
		auto headPoint = _dashes.front().P1;
		auto tailPoint = _dashes.back().P2;
		if(distanceP2P(headPoint,tailPoint) < MiniDistance) _isClosed = true;
	};
	return _isClosed;
};
bool  dashTrain::isValid( ) { 
	if(_dashes.size()==0) return false;
	for(auto dash : _dashes) {
		if(!dash.isValid()) return false;
	}; 
	for(int n=0; n < _dashes.size()-1; n++) {
		auto dashA = _dashes[n];
		auto dashB = _dashes[n+1];
		if(!dashA.isDashConnected(dashB)) return false;
	}; 
	if(isClosed()) return false;
	return true;
};
bool  dashTrain::isPointOn( vec2d V ) { for(auto dash : _dashes) {if(dash.isPointOn(V)) return true;}; return false; };
bool  dashTrain::isPointOnLeft( vec2d V ) { int i = getClosestDashID(V); auto dash = _dashes[i]; return dash.isPointOnLeft(V); };
bool  dashTrain::isTrainConnectedBy( Dash& dash ) { return _dashes.back().isDashConnected(dash); };
bool  dashTrain::isTrainConnectedBy( dashTrain& train ) { 
	auto myDashes = train.getDashes();
	return _dashes.back().isDashConnected(myDashes.front());
	};
bool  dashTrain::isTrainCrossedIn( dashTrain& trainB ) { 
        std::vector<vec2d>  crossPs;
        getCrossPoints( trainB, crossPs );
	vec2d head = _dashes.front().P1;
	vec2d end  = _dashes.back().P2;
	for(auto Pnt : crossPs ) {
		double s = std::min((head-Pnt).length2(),(end-Pnt).length2());
		if( s < MaxiPositive && s > MiniDistance ) return true;
	};
	return false;
};

bool  dashTrain::isTrainIdentical( dashTrain& trainB ) { 
	auto trainA = dashTrain(_dashes);
	if( !(trainA.isValid() && trainB.isValid()) ) return false;
	if( trainA.getDashes().size() != trainB.getDashes().size() ) return false;
	for(int n = 0; n < trainA.getDashes().size(); n++ ) {
		auto dashA = trainA.getDashes()[n];
		auto dashB = trainB.getDashes()[n];
		if( dashA != dashB ) return false;
	};
	return true;
};

double dashTrain::getDistanceToPoint( vec2d O ) {
        double ss = MaxiPositive;
	for(auto dash:_dashes) ss = std::min(ss, dash.getDistanceToPoint(O));
        return ss;
};
int dashTrain::getClosestDashID( vec2d O ) {
        double ss = MaxiPositive;
	int m =0;
	for(int n=0; n<_dashes.size(); n++) {
		double s = _dashes[n].getDistanceToPoint(O);
		if(s<ss){ ss =s; m = n;};
	};
        return m;
};
vec2d dashTrain::getCrossPoint( Dash dashB ) {
        vec2d ret = vec2d(MaxiPositive, MaxiPositive);
        if( !(isValid() && dashB.isValid()) ) return ret;
	for(auto dash:_dashes) { 
		ret = dash.getCrossPoint(dashB);
		if(ret.length() < MaxiDistance) return ret;
	};
        return ret;
};
void dashTrain::getCrossPoints( Dash& dashB, std::vector<vec2d>&  verts ) {
        if( !(isValid() && dashB.isValid()) ) return;
        vec2d vert = vec2d(MaxiPositive, MaxiPositive);
	for(auto dash:_dashes) { 
		vert = dash.getCrossPoint(dashB);
		if(vert.length() < MaxiDistance) verts.push_back(vert);
	};
        return;
};
void dashTrain::getCrossPoints( dashTrain& trainB, std::vector<vec2d>&  verts ) {
        if( !(isValid() && trainB.isValid()) ) return;
        vec2d vert = vec2d(MaxiPositive, MaxiPositive);
	auto myDashes = trainB.getDashes();
	for(auto dashB : myDashes) { 
		for(auto dash:_dashes) { 
			vert = dash.getCrossPoint(dashB);
			if(vert.length() < MaxiDistance) verts.push_back(vert);
		};
	};
        return;
};
std::vector<Dash> dashTrain::trainSlicedByDash( Dash& toolDash ) {
	std::vector<Dash> myDashes;
	for(auto dash : _dashes) {
		std::vector<Dash> ResDashes = dash.dashSlicedByDash(toolDash);
		for(auto d : ResDashes) {if(d.isValid()) myDashes.push_back(d);};
	};
	return myDashes;
};
std::vector<Dash> dashTrain::trainSlicedByTrain( dashTrain& toolTrain ) {
	dashTrain objectTrain = dashTrain(_dashes);
	for(auto dash : toolTrain.getDashes()) {
		auto myDashes = objectTrain.trainSlicedByDash(dash);
		objectTrain = dashTrain(myDashes);
	};
	return objectTrain.getDashes();
};
dashTrain dashTrain::BoolAdd( Dash& dash ) {
	auto myDashes = getDashes();
	if(myDashes.back().isDashConnected(dash)) myDashes.push_back(dash);
	return dashTrain(myDashes);
};
dashTrain dashTrain::BoolAdd( dashTrain& train ) {
	auto myDashes =	train.getDashes();
	for(const auto dash : myDashes) if(myDashes.back().isDashConnected(dash)) 
		myDashes.push_back(dash);
	return dashTrain(myDashes);
};
dashTrain dashTrain::BoolCut( Dash dashB ) {
	std::vector<Dash> myDashes;
	for( auto dash : _dashes ) {
		auto dashC = dash.dashBoolCut( dashB );
	  	if( dashC == dash ) {
			myDashes.push_back(dash);
		} else {
			if(dashC.isValid()) myDashes.push_back(dashC);
			break;
			auto dashD = dash.dashBoolCutLeft( dashB );
			if(dashD.isValid()) myDashes.push_back(dashD);
		};
	};
	return dashTrain(myDashes);
};
dashTrain dashTrain::BoolCutLeft( Dash dashB ) {
	std::vector<Dash> myDashes;
	int m = 0;
	for( auto dash : _dashes ) {
		auto dashC = dash.dashBoolCut( dashB );
	  	if( dashC == dash ) {
			continue;
		} else {
			break;
		};
		m++;
	};
	for( int n = m; n < _dashes.size(); n++ ) {
	  	if( n == m ) {
			auto dashD = _dashes[n].dashBoolCutLeft( dashB );
			if(dashD.isValid()) myDashes.push_back(dashD);
		} else myDashes.push_back(_dashes[n]);
	};
	return dashTrain(myDashes);
};
dashTrain dashTrain::BoolCut( dashTrain trainB ) {
	dashTrain trainA = dashTrain();
	auto myDashes = trainB.getDashes();
	for(auto dashB : myDashes) {
		trainA = BoolCut( dashB );
		auto versA = trainA.getVertexes();
		auto versB = trainB.getVertexes();
	  	if( trainA.isValid() && versA != versB ) return trainA;
	};
	return trainA;
};
dashTrain dashTrain::BoolCutLeft( dashTrain trainB ) {
	dashTrain trainA = dashTrain();
	auto myDashes = trainB.getDashes();
	for(auto dashB : myDashes) {
		trainA = BoolCutLeft( dashB );
		auto versA = trainA.getVertexes();
		auto versB = trainB.getVertexes();
	  	if( trainA.isValid() && versA != versB ) return trainA;
	};
	return trainA;
};
dashTrain dashTrain::trainReverse( ) {
	if(!isValid()) return dashTrain();
	std::vector<Dash> dashes;
	int m = _dashes.size();
	for( int n = 0; n < m; n++ ) {
		auto dash = _dashes[m-n-1];
		dashes.push_back(Dash(dash.P2,dash.P1));
	};
	return dashTrain(dashes);
};
void dashTrain::trainSelfDivided( std::vector<dashTrain>& myTrains ) {
        std::vector<Dash> myDashes;
	int m = 0;
	while( m < _dashes.size() ) {
		myDashes.clear();
		auto dash = _dashes[m];
		if( dash.isValid() ) {
			myDashes.push_back(dash);
			int n = m + 1;
			while( n < _dashes.size() ) {
				if(!isP2P(_dashes[n].P1,_dashes[n-1].P2)) break;
				myDashes.push_back(_dashes[n]);
				n++;
			};
			if(myDashes.size()>0) myTrains.push_back(dashTrain(myDashes));
			m = n;
		} else m++;
	};
};
std::vector<dashTrain> dashTrain::trainSelfDivided( ) {
	std::vector<dashTrain> myTrains;
        std::vector<Dash> myDashes;
	int m = 0;
	while( m < _dashes.size() ) {
		myDashes.clear();
		auto dash = _dashes[m];
		if( dash.isValid() ) {
			myDashes.push_back(dash);
			int n = m + 1;
			while( n < _dashes.size() ) {
				if(!isP2P(_dashes[n].P1,_dashes[n-1].P2)) break;
				myDashes.push_back(_dashes[n]);
				n++;
			};
			if(myDashes.size()>0) myTrains.push_back(dashTrain(myDashes));
			m = n;
		} else m++;
	};
	return myTrains;
};
void dashTrain::trainPrint(std::string myStr) {
	for(int n = 0; n < _dashes.size(); n++) {
		auto uStr = myStr + std::to_string(n);
		uStr += " Dash ";
		_dashes[n].dashPrint(uStr);
	};
};
void dashTrain::trainTranslate(vec2d& vec) {
	for(auto& dash:_dashes) dash.dashTranslate(vec);
	update();
};
void dashTrain::trainZooming(double factorX, double factorY = MaxiDistance) {
	if(factorY == MaxiDistance) {
		for(auto& dash:_dashes) dash.dashZooming(factorX);
	} else {
		for(auto& dash:_dashes) dash.dashZooming(factorX, factorY);
	};
	update();
};
void dashTrain::trainRotDegree(double& factor) {
	for(auto& dash:_dashes) dash.dashRotDegree(factor);
	update();
};

} // namespace geo2d

EW_LEAVE




