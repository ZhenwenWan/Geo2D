#ifndef BASE2D_H
#define BASE2D_H
#include "dashLoop.h"

EW_ENTER

namespace geo2d {

class base2d {
	public:
		base2d(){};
		base2d(std::vector<dashLoop> myLoops) { base2dConstruct(myLoops); update();};
		~base2d(){};
		int index( dashLoop&  myLoop);
		int index( dashTrain& myTrain);
		int index( Dash&      myDash);
		int index( vec2d&     myVertex);
		std::vector<vec2d>            getVertexes();
		std::vector<Dash>             getDashes();
		std::vector<dashTrain>        getDashTrains();
		std::vector<dashLoop>         getDashLoops(){return _dashLoops;};
		std::vector<int>              getOuterLoopsID() { return _outerLoopsID; };
		std::vector<std::vector<int>> getInnerLoopsID() { return _innerLoopsID; };
                bool  isValid( ) { return _isValid; }; 
                bool  isOuterLoop( dashLoop& myLoop, std::vector<dashLoop>& Loops ); 
		base2d BoolCut( base2d& base2dB );
		base2d BoolInt( base2d& base2dB );
		base2d BoolAdd( base2d& base2dB );
		base2d VBoolCut( base2d& base2dB );
		void Print( std::string myStr );
		void Export2GMSH_Geo(const std::string& fileName, const std::string& myStr,
				double scaling = 1.0, int entry = 1);
 		void Translate(vec2d vec );
 		void RotDegree(double factor );
		void Zooming(double factorX, double factorY = MaxiDistance) {
			if(factorY == MaxiDistance) {
				for(auto& loop:_dashLoops) loop.loopZooming(factorX, factorX);
			} else {
				for(auto& loop:_dashLoops) loop.loopZooming(factorX, factorY);
			};
			update();
		};

	private:
		std::vector<vec2d>            _vertexes;
		std::vector<Dash>             _dashes;
		std::vector<dashTrain>        _dashTrains;
		std::vector<dashLoop>         _dashLoops;
		std::vector<std::vector<int>> _innerLoopsID; 
		std::vector<int>              _outerLoopsID; 
		bool _isValid                 =  false;
		bool _update                  =  false;
		void base2dConstruct(std::vector<dashLoop>& myLoops);
		void update();

};

} // namespace geo2d

EW_LEAVE



#endif

