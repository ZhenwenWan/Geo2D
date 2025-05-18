#ifndef DASHTRAIN_H
#define DASHTRAIN_H
#include "dash.h"

EW_ENTER

namespace geo2d {

class dashTrain {
	public:
    	std::vector<Dash> dashes;
		dashTrain(){};
		dashTrain( std::vector<Dash>& dashes);
		std::vector<vec2d> getVertexes( );
		std::vector<Dash> getDashes( ) { return _dashes; };
		~dashTrain(){};
		bool addVertex(vec2d V);
		bool addDash(Dash dash);
		bool isClosed();
        bool isValid( );
        bool isPointOn( vec2d V );
        bool isPointOnLeft( vec2d V );
        bool isTrainConnectedBy( Dash& dash );
        bool isTrainConnectedBy( dashTrain& train );
        bool isTrainCrossedIn( dashTrain& trainB );
        bool isTrainIdentical( dashTrain& trainB );
        double getDistanceToPoint( vec2d O );
        int    getClosestDashID( vec2d O );
        vec2d  getCrossPoint( Dash dashB );
		void   getCrossPoints( Dash& dashB, std::vector<vec2d>& verts );
		void   getCrossPoints( dashTrain& trainB, std::vector<vec2d>& verts );
		std::vector<Dash> trainSlicedByDash( Dash& dash );
        std::vector<Dash> trainSlicedByTrain( dashTrain& toolTrain );
		dashTrain BoolAdd( Dash& dash );
		dashTrain BoolAdd( dashTrain& train );
		dashTrain BoolCut( Dash dashB );
		dashTrain BoolCutLeft( Dash dashB );
		dashTrain BoolCut( dashTrain trainB );
		dashTrain BoolCutLeft( dashTrain trainB );
		dashTrain trainReverse( );
        void trainSelfDivided(std::vector<dashTrain>& myTrains);
        std::vector<dashTrain> trainSelfDivided( );
		void trainTranslate(vec2d& vec );
		void trainRotDegree(double& factor );
		void trainZooming(double factorX, double factorY );
		void trainPrint(std::string myStr);

	private:
		std::vector<vec2d> _vertexes;
		std::vector<Dash> _dashes;
		bool _update = false;
		bool _isClosed = false;
		void update();

};

}  // namespace geo2d

EW_LEAVE

#endif




