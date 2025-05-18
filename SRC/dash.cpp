#include "dash.h"

EW_ENTER

namespace geo2d {

vec2d  Dash::getCrossPoint( Dash B ) {
        vec2d ret = vec2d(MaxiPositive, MaxiPositive);
        if( isParallelTo( B ) ) return ret;
	double k1 = _slope;
	double k2 = B.getSlope();
	double b1 = _interception;
	double b2 = B.getInterception();
	if( (std::abs(k1) < MaxiPositive) && (std::abs(k2) < MaxiPositive) ) {
        	ret[0] = -(b2 - b1)/(k2 - k1);  
        	ret[1] = k1*ret[0] + b1;
	} else if( (std::abs(k1) < MaxiPositive) && !(std::abs(k2) < MaxiPositive) ) {
        	ret[0] = 0.5*(B.P1[0]+B.P2[0]);  
        	ret[1] = k1*ret[0] + b1;
	} else if( !(std::abs(k1) < MaxiPositive) && (std::abs(k2) < MaxiPositive) ) {
        	ret[0] = 0.5*(P1[0]+P2[0]);  
        	ret[1] = k2*ret[0] + b2;
	}
	return isPointOn(ret)&&B.isPointOn(ret) ? ret : vec2d(MaxiPositive, MaxiPositive);
};
double  Dash::getProjDistanceToPoint( vec2d O ) {
        if( P1.length2() >= MaxiPositive ) return MaxiPositive;
        if( P2.length2() >= MaxiPositive ) return MaxiPositive;
        if(  O.length2() >= MaxiPositive ) return MaxiPositive;
        vec2d AO = O - P1;
        vec2d AB = P2- P1;
        if( AO.length2() >= MaxiPositive ) return MaxiPositive;
        if( AB.length2() >= MaxiPositive ) return MaxiPositive;
        double dis   = sqrt(AO.length2() - pow(dot(AO, AB),2.0)/AB.length2());
        return dis;
};
double  Dash::getDistanceToPoint( vec2d O ) {
	if( ! isValid() ) return MaxiPositive; 
    	double cos = (P2[0] - P1[0])/_length;
    	double sin = (P2[1] - P1[1])/_length;
        double projLength = cos*(O[0] - P1[0]) + sin*(O[1] - P1[1]);
	projLength = std::max(0.0, std::min(projLength, _length));
        vec2d closestPoint = vec2d(cos*projLength+P1[0],sin*projLength+P1[1]);
        return ( O - closestPoint ).length();
};
bool  Dash::isPointOn( vec2d P ) {
	double s1 = (P - P1).length();
	double s2 = (P - P2).length();
	double ss = (P2- P1).length();
	return ( s1 + s2 - ss > MiniDistance ) ? false : true;
};
bool  Dash::isPointOnLeft( vec2d P ) {
        vec2d AP = P - P1;
        vec2d AB = P2- P1;
        double c = crossProduct(AB, AP);
	return c > MiniDistance;
};
bool   Dash::isDashPerpendicular( Dash B ) {
	double angle = getAngleDegreeWith( B );
	return (std::abs(angle-90.0) <= MiniDistance || std::abs(angle-270.0) <= MiniDistance) ? 
		true : false;
};
bool   Dash::isDashCrossIn( Dash B ) {
	if( isParallelTo( B ) ) return false;
	vec2d C = getCrossPoint(B);
	if( !isPointValid(C) ) return false;
	if( !isPointOn(C) ) return false;
	return distanceP2P(C,P1) > MiniDistance && distanceP2P(C,P2) > MiniDistance;
};
RelationD2D Dash::getRelation( Dash B ) {
	bool crossIn = isDashCrossIn( B );
	int touch = 0;
	if( getRelation( B.P1 ) == D2P_oneTouch ) touch++;
	if( getRelation( B.P2 ) == D2P_oneTouch ) touch++;
	if( B.getRelation( P1 ) == D2P_oneTouch ) touch++;
	if( B.getRelation( P2 ) == D2P_oneTouch ) touch++;
	if( touch == 0 && !crossIn ) return D2D_none;
	if( crossIn ) return D2D_oneTouch;
	return D2D_multiTouch;
};
Dash Dash::dashBoolCut( Dash B ) {
	if( getRelation( B ) == D2D_none ) return *this;
	if( getRelation( B ) == D2D_oneTouch ) {
		vec2d P = getCrossPoint( B );
		return isPointValid(P) ? Dash(P1, P) : Dash();
	};
	return isPointOn(B.P1) ? Dash(P1, B.P1) : Dash();
};
Dash Dash::dashBoolCutLeft( Dash B ) {
	if( getRelation( B ) == D2D_none ) return Dash();
	if( getRelation( B ) == D2D_oneTouch ) {
		vec2d P = getCrossPoint( B );
		return isPointValid(P) ? Dash(P, P2) : *this;
	};
	return isPointOn(B.P1) ? Dash(B.P1, P2) : *this;
};
std::vector<Dash> Dash::dashSlicedByDash( Dash& B ) {
	auto A = Dash(P1, P2);
	std::vector<Dash> retDashes;

	// case 1: invalid
	if(!(A.isValid() && B.isValid())) {retDashes.push_back(A); return retDashes;};

	// case 2: parallel to
	if(A.isParallelTo(B)) {
		if(A.isPointOn(B.P1) && A.isPointOn(B.P2)) {
			double r1 = distanceP2P(A.P1,B.P1);
			double r2 = distanceP2P(A.P1,B.P2);
			if(r1<r2) {
				retDashes.push_back(Dash(A.P1,B.P1));
				retDashes.push_back(B);
				retDashes.push_back(Dash(B.P2,A.P2));
			} else {
				retDashes.push_back(Dash(A.P1,B.P2));
				retDashes.push_back(Dash(B.P2,B.P1));
				retDashes.push_back(Dash(B.P1,A.P2));
			};
		} else if(A.isPointOn(B.P2)) {
			retDashes.push_back(Dash(A.P1,B.P2));
			retDashes.push_back(Dash(B.P2,A.P2));
		} else if(A.isPointOn(B.P2)) {
			retDashes.push_back(Dash(A.P1,B.P2));
			retDashes.push_back(Dash(B.P2,A.P2));
		};
		if( retDashes.size() == 0 ) retDashes.push_back(A);
	    return retDashes;
	};

	// case 3: crossing
	vec2d PP = A.getCrossPoint( B );
	if( isPointValid(PP) ) {
		retDashes.push_back(Dash(A.P1,  PP));
		retDashes.push_back(Dash(PP  ,A.P2));
	    return retDashes;
	};

	if( retDashes.size() == 0 ) retDashes.push_back(A);
	return retDashes;
};
Dash Dash::dashBoolAdd( Dash B ) {
	if( getRelation( B ) == D2D_none ) return *this;
	bool parallel = isParallelTo( B );
	if( ! parallel ) return *this;
	double angle = getAngleWith( B );
	double ref = std::cos( angle );
	if( std::abs(ref-1.0) >= MiniDistance ) return *this;
	if( isPointOn(B.P1) && isPointOn(B.P2) ) return *this;
	if( B.isPointOn(P1) && B.isPointOn(P2) ) return B;
	if( isPointOn(B.P1) ) {
		return Dash(P1, B.P2);
	} else if( isPointOn(B.P2) ) {
		return Dash(B.P1, P2);
	} else return Dash();
};
Dash Dash::dashBoolInt( Dash B ) {
	if( getRelation( B ) == D2D_none ) return *this;
	bool parallel = isParallelTo( B );
	if( ! parallel ) return *this;
	double angle = getAngleWith( B );
	double ref = std::cos( angle );
	if( std::abs(ref-1.0) >= MiniDistance ) return *this;
	if( isPointOn(B.P1) && isPointOn(B.P2) ) return B;
	if( B.isPointOn(P1) && B.isPointOn(P2) ) return *this;
	if( isPointOn(B.P1) ) {
		return Dash(B.P1, P2);
	} else if( isPointOn(B.P2) ) {
		return Dash(P1, B.P2);
	} else return Dash();
};
void Dash::dashPrint(std::string myStr) {
	auto uStr = myStr + " Starting point ";
	pointPrint(uStr,P1); 
	uStr = myStr + "   Ending point ";
	pointPrint(uStr,P2);
};
void Dash::dashRotDegree( double& ang ) {
	auto mag1 = vectorMagnitude(P1); 
	auto ang1 = calculateAngle(P1); 
	auto x1 = std::cos((ang1 + ang)*M_PI/180.0);
	auto y1 = std::sin((ang1 + ang)*M_PI/180.0);
	P1 = mag1*vec2d(x1,y1);
	auto mag2 = vectorMagnitude(P2); 
	auto ang2 = calculateAngle(P2); 
	auto x2 = std::cos((ang2 + ang)*M_PI/180.0);
	auto y2 = std::sin((ang2 + ang)*M_PI/180.0);
	P2 = mag2*vec2d(x2,y2);
	update();
};

}  // namespace geo2d

EW_LEAVE


