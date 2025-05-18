#ifndef DASHLOOP_H
#define DASHLOOP_H
#include "dashTrain.h"

EW_ENTER

namespace geo2d {

class dashLoop {
	public:
    	std::vector<dashTrain> trains;
		dashLoop(){};
		dashLoop(std::vector<dashTrain>& myTrains) 
			{ loopConstruct(myTrains); update(); };
		~dashLoop(){};
		std::vector<vec2d>     getVertexes();
		std::vector<Dash>      getDashes();
		std::vector<dashTrain> getDashTrains() {return _dashTrains;};
		int index( dashTrain&  myTrain);
		int index( Dash&       myDash);
		int index( vec2d&      myVertex);
                bool  isValid( ) { return _isValid; }; 
                double getDistanceToPoint( vec2d O );
                int getClosestTrainID( vec2d O );
		bool isPointIn( vec2d O );
		bool isPointOn( vec2d O );
		bool isPointOut( vec2d O );
		bool isLoopCoveredByLoop( dashLoop& myLoop );
		bool isLoopInsectLoop( dashLoop& myLoop );
		bool isLoopIdentical( dashLoop& myLoop );
		std::vector<vec2d> getPointsIn( dashTrain& myTrain );
		std::vector<vec2d> getPointsOn( dashTrain& myTrain );
		std::vector<vec2d> getPointsOut( dashTrain& myTrain );
		std::vector<vec2d> getPointsIn( dashLoop& myLoop );
		std::vector<vec2d> getPointsOn( dashLoop& myLoop );
		std::vector<vec2d> getPointsOut( dashLoop& myLoop );
		std::vector<vec2d> getPointsIn( std::vector<vec2d>& myVerts );
		std::vector<vec2d> getPointsOn( std::vector<vec2d>& myVerts );
		std::vector<vec2d> getPointsOut( std::vector<vec2d>& myVerts );
		void  loopToolSliceLoop( dashLoop& loopB, std::vector<dashTrain>& trainsIn, 
				std::vector<dashTrain>& trainsOut, std::vector<dashTrain>& trainsOn );
		std::vector<Dash>  loopToolSliceDash( Dash& objectDash );
		std::vector<Dash>  loopToolSliceTrain( dashTrain& objectTrain );
		std::vector<Dash>  loopToolSliceTrainOn( dashTrain& objectTrain );
		std::vector<Dash>  loopToolSliceTrainIn( dashTrain& objectTrain );
		std::vector<Dash>  loopToolSliceTrainOut( dashTrain& objectTrain );
		dashTrain trainCutByLoop( dashTrain& objectTrain );
		dashTrain trainCutByLoopLeft( dashTrain& objectTrain );
		dashLoop BoolCut( dashLoop loopB );
		dashLoop BoolCutLeft( dashLoop loopB );
		dashLoop BoolInt( dashLoop loopB );
		dashLoop BoolAdd( dashLoop loopB );
		std::vector<dashLoop> VBoolCut( dashLoop loopB );
		void loopTranslate( vec2d& vec );
		void loopRotDegree( double& factor );
		void loopZooming( double factorX, double factorY );
		void loopPrint(std::string myStr);

	private:
		std::vector<vec2d>     _vertexes;
		std::vector<Dash>      _dashes;
		std::vector<dashTrain> _dashTrains;
		bool _isValid =  false;
		bool _update  =  false;
		void loopConstruct(std::vector<dashTrain>& myTrains);
		void update();

};

} // namespace geo2d

EW_LEAVE



#endif

