#ifndef DASH_H
#define DASH_H
#include "geo2d.h"

EW_ENTER

namespace geo2d {

class Dash {
	public:
		vec2d P1, P2;
    	std::pair<double, double> start;
    	std::pair<double, double> end;

		Dash() : P1(vec2d(MaxiPositive,MaxiPositive)), P2(vec2d(MaxiPositive,MaxiPositive)) {};
        Dash( const vec2d V1, const vec2d V2 ) : P1(V1), P2(V2) { update(); };

		~Dash(){};

		double getLength() { return _length; };
		double getSlope() { return _slope; };
		double getInterception() { return _interception; };
                bool   isParallelTo( Dash B ) {
                	double k = B.getSlope();
			if(std::abs(k) >= MaxiPositive && std::abs(_slope) >= MaxiPositive) return true;
                	double s = std::abs(_slope - k);
                	return (s <= MiniDistance) ? true : false;
                };
                vec2d  getCrossPoint( Dash B );
                double  getAngleDegreeWith( Dash B ) {return calculateAngle(P2-P1, B.P2-B.P1);};
                double  getAngleWith( Dash B ) {return getAngleDegreeWith( B )*M_PI/180.0;};
                double  getProjDistanceToPoint( vec2d O );
                double  getDistanceToPoint( vec2d O );
                bool  isPointOn( vec2d P );
                bool  isPointOnLeft( vec2d P );
                bool  isPointOnRight( vec2d P ) {return !(isPointOn(P)||isPointOnLeft(P));};
                bool  isValid( ) { return P1.length2() < MaxiPositive && P2.length2() < MaxiPositive
			           && (P2-P1).length2() < MaxiPositive && (P2-P1).length() > MiniDistance; };
                bool   isDashPerpendicular( Dash B );
                bool   isSameLength( Dash B ) { return std::abs(getLength()-B.getLength()) < MiniDistance; };
		bool   isDashIdentical( Dash& B ) { return isP2P(P1,B.P1) && isP2P(P2,B.P2); };
                bool   isDashCrossIn( Dash B );
                bool   isDashConnected( Dash B ) { vec2d C = P2 - B.P1; return C.length2() < MiniDistance; };
		RelationD2P getRelation( vec2d P ) {return isPointOn( P ) ? D2P_oneTouch : D2P_none;};
		RelationD2D getRelation( Dash B );
		Dash dashBoolCut( Dash B );
		Dash dashBoolCutLeft( Dash B );
		std::vector<Dash> dashSlicedByDash( Dash& B );
		Dash dashBoolAdd( Dash B );
		Dash dashBoolInt( Dash B );
	        bool operator==(const Dash& o) const { double d1 = (o.P1 - P1).length2(); double d2 = (o.P2 - P2).length2(); 
			                               return d1 <= MiniDistance && d2 <= MiniDistance; };
	        bool operator!=(const Dash& o) const { return !operator==(o); };
		void dashTranslate( vec2d& vec ) {P1 += vec; P2 += vec;update();};
		void dashRotDegree( double& ang );
		void dashZooming( double factorX, double factorY = MaxiDistance ) {
			P1[0] *= factorX; 
			P2[0] *= factorX; 
			if(factorY == MaxiDistance) {
				P1[1] *= factorX;
				P2[1] *= factorX;
			 } else {
				P1[1] *= factorY;
				P2[1] *= factorY;
			 };
			update();
		};
		void dashPrint(std::string myStr);


	private:
		bool _update = false;
		double _slope = MaxiPositive;
		double _interception = MaxiPositive;
		double _length = MaxiPositive;
		void update(){_update = false;length();slope();interception();_update = true;};
		double length() {
			if( _update ) return _length;
			vec2d dash = vec2d(P2[0]-P1[0],P2[1]-P1[1]);
			_length = dash.length();
			return _length;
		};
		double slope() {
			if( _update ) return _slope;
			double k = MaxiPositive;
			if( std::abs(P1[0]-P2[0]) > MiniDistance ) {
                k = (P1[1]-P2[1])/(P1[0]-P2[0]);
                if(std::abs(k) > MaxiPositive) k = MaxiPositive;                        
			}
			_slope = k;
			return _slope;
		};
		double interception() {
			if( _update ) return _interception;
			if( std::abs(_slope) < MaxiPositive ) _interception = P1[1] - P1[0]*_slope;
			return _interception;
		};
};

} // namesapce geo2d

EW_LEAVE

#endif

