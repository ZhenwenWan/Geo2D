
#include "base2d.h"


EW_ENTER

namespace geo2d {

std::vector<dashTrain> base2d::getDashTrains() { 
	if(_update) return _dashTrains;
	
	_dashTrains.clear();
	auto  loops = getDashLoops();
	for(int n = 0; n < getOuterLoopsID().size(); n++) {
		int iOut = getOuterLoopsID()[n];
		auto outerLoop = loops[iOut];
		auto  trains = outerLoop.getDashTrains();
		for(auto train : trains) {
			bool Yes = true;
			for( auto t : _dashTrains ) if(t.isTrainIdentical(train)) Yes = false;
			if(Yes) _dashTrains.push_back(train);
		};
		for(int m = 0; m < getInnerLoopsID()[n].size(); m++) {
			int jInn = getInnerLoopsID()[n][m];
			auto innerLoop = loops[jInn];
			auto  trains = innerLoop.getDashTrains();
			for(auto train : trains) {
				bool Yes = true;
				for( auto t : _dashTrains ) if(t.isTrainIdentical(train)) Yes = false;
				if(Yes) _dashTrains.push_back(train);
			};
		};
	};

	return _dashTrains;
};

std::vector<Dash> base2d::getDashes() { 
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

std::vector<vec2d> base2d::getVertexes() { 
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

int base2d::index(vec2d& myVertex) { 
	auto  runs = getVertexes();
	int n = 0;
	for(auto run : runs) {
		if(isP2P(run,myVertex)) break;
		n++;
	};
	return n;
};

int base2d::index(Dash& myDash) { 
	auto  runs = getDashes();
	int n = 0;
	for(auto run : runs) {
		if(run.isDashIdentical(myDash)) break;
		n++;
	};
	return n;
};

int base2d::index(dashTrain& myTrain) { 
	auto  runs = getDashTrains();
	int n = 0;
	for(auto run : runs) {
		if(run.isTrainIdentical(myTrain)) break;
		n++;
	};
	return n;
};

int base2d::index(dashLoop& myLoop) { 
	auto  runs = getDashLoops();
	int n = 0;
	for(auto run : runs) {
		if(run.isLoopIdentical(myLoop)) break;
		n++;
	};
	return n;
};

bool base2d::isOuterLoop(dashLoop& myLoop, std::vector<dashLoop>& Loops) {
	if(!myLoop.isValid()) return false;
	for(int n = 0; n < Loops.size(); n++) {
		auto loop = Loops[n];
		if(!loop.isValid()) continue;
		if(myLoop.isLoopIdentical(loop)) continue;
		if(myLoop.isLoopCoveredByLoop(loop)) return false;
	};
	return true;
};

void base2d::base2dConstruct(std::vector<dashLoop>& myLoops) {

	for(int n = 0; n < myLoops.size(); n++) {
		auto loop = myLoops[n];
		if(loop.isValid() && isOuterLoop(loop, myLoops)) {
			bool Yes = true;
			for(int m : _outerLoopsID) {
				auto lop = myLoops[m];
				if(lop.isLoopIdentical(loop)) Yes = false;
			};
			if(Yes) {
				_outerLoopsID.push_back(n); 
				_dashLoops.push_back(loop);
			};
		};
	};
	int ms = _outerLoopsID.size();
	if(ms==0) return;

	for(int n = 0; n < ms; n++) {
		int i = _outerLoopsID[n];
		auto outerLoop = myLoops[i];
		std::vector<int> vecID;
		for(int j = 0; j < myLoops.size(); j++) {
			if(j==i) continue;
			auto innerLoop = myLoops[j];
			if(innerLoop.isValid() && innerLoop.isLoopCoveredByLoop(outerLoop)) {
				bool Yes = true;
				for(int k = 0; k < vecID.size(); k++) {
			   		auto innerLoopA = myLoops[k];
					if(innerLoopA.isLoopIdentical(innerLoop)) Yes = false;
				};
			        if( Yes ) {
					vecID.push_back(j);
					_dashLoops.push_back(innerLoop);
				};
			};
		};
		_innerLoopsID.push_back(vecID);
	};

	for(int n = 0; n < _outerLoopsID.size(); n++) {
		int i = _outerLoopsID[n];
		auto outerLoop = myLoops[i];
		_outerLoopsID[n] = index(outerLoop);
		for(int m = 0; m < _innerLoopsID[n].size(); m++) {
			int j = _innerLoopsID[n][m];
			auto innerLoop = myLoops[j];
			_innerLoopsID[n][m] = index(innerLoop);
		};
	};

	_isValid = true;	
};

base2d base2d::BoolCut( base2d& base2dB ) {
	base2d base2dA = base2d(_dashLoops);
	auto AOutersID = base2dA.getOuterLoopsID();
	auto BOutersID = base2dB.getOuterLoopsID();
	auto AInnersID = base2dA.getInnerLoopsID();
	auto BInnersID = base2dB.getInnerLoopsID();
	auto loopsA = base2dA.getDashLoops();
	auto loopsB = base2dB.getDashLoops();

	std::vector<dashLoop> loopsInsectOut;
	std::vector<dashLoop> loopsInsectInn;

	for(int n = 0; n < AOutersID.size(); n++) {
		int nID = AOutersID[n];
		auto loopOutA  = loopsA[nID];
		for(int m = 0; m < BOutersID.size(); m++) {
			int mID = BOutersID[m];
			auto loopOutB = loopsB[mID];
			if(!loopOutB.isLoopInsectLoop(loopOutA)) {
				if(loopOutA.isValid()) loopsInsectOut.push_back(loopOutA);
				for(int k = 0; k < AInnersID[n].size(); k++) {
					int nt = AInnersID[n][k];
				    if(loopsA[nt].isValid()) loopsInsectInn.push_back(loopsA[nt]);
				};
				continue;
			};
			auto loop = loopOutA.BoolCut(loopOutB);
			for(int k = 0; k < AInnersID[n].size(); k++) {
			    int nk = AInnersID[n][k];
				auto loopInnA = loopsA[nk];
				if(loopInnA.isLoopInsectLoop(loopOutB)) loop = loop.BoolCut(loopInnA);
			};
			if(loop.isValid()) loopsInsectOut.push_back(loop);
			for(int k = 0; k < AInnersID[n].size(); k++) {
			    int nk = AInnersID[n][k];
				auto loopInnA = loopsA[nk];
				if(loopInnA.isLoopCoveredByLoop(loop)) loopsInsectInn.push_back(loopInnA);
			};

			loop = loopOutA.BoolInt(loopOutB);
			if(!loop.isValid()) continue;
			for(int k = 0; k < BInnersID[m].size(); k++) {
			    int mk = BInnersID[m][k];
				auto sLoop = loop.BoolInt(loopsB[mk]);
				if(!sLoop.isValid()) continue;
				for(int j = 0; j < AInnersID[n].size(); j++) {
					int nj = AInnersID[n][j];
					auto tLoop = loopsA[nj];
					if(sLoop.isLoopInsectLoop(tLoop)) sLoop = sLoop.BoolCut(tLoop);
				};
				if(sLoop.isValid()) loopsInsectOut.push_back(sLoop);
			};
		};
	};

	for(auto loop : loopsInsectInn) loopsInsectOut.push_back(loop);

	return base2d(loopsInsectOut);
};

base2d base2d::VBoolCut( base2d& base2dB ) {
	base2d base2dA = base2d(_dashLoops);
	auto AOutersID = base2dA.getOuterLoopsID();
	auto BOutersID = base2dB.getOuterLoopsID();
	auto AInnersID = base2dA.getInnerLoopsID();
	auto BInnersID = base2dB.getInnerLoopsID();
	auto loopsA = base2dA.getDashLoops();
	auto loopsB = base2dB.getDashLoops();

	std::vector<dashLoop> loopsInsectOut;
	std::vector<dashLoop> loopsInsectInn;

	for(int n = 0; n < AOutersID.size(); n++) {
		int nID = AOutersID[n];
		auto loopOutA  = loopsA[nID];
		for(int m = 0; m < BOutersID.size(); m++) {
			int mID = BOutersID[m];
			auto loopOutB = loopsB[mID];
			if(!loopOutB.isLoopInsectLoop(loopOutA)) {
				if(loopOutA.isValid()) loopsInsectOut.push_back(loopOutA);
				for(int k = 0; k < AInnersID[n].size(); k++) {
					int nt = AInnersID[n][k];
				    if(loopsA[nt].isValid()) loopsInsectInn.push_back(loopsA[nt]);
				};
				continue;
			};
			if(loopOutB.isLoopCoveredByLoop(loopOutA)) {
				if(loopOutA.isValid()) loopsInsectOut.push_back(loopOutA);
				if(loopOutB.isValid()) {
					auto tLoop = loopOutB;
					for(int k = 0; k < AInnersID[n].size(); k++) {
						int nt = AInnersID[n][k];
			            if(loopOutB.isLoopInsectLoop(loopsA[nt])) tLoop = tLoop.BoolAdd(loopsA[nt]);
					};
					loopsInsectInn.push_back(tLoop);
					for(int k = 0; k < AInnersID[n].size(); k++) {
						int nt = AInnersID[n][k];
			            if(!loopOutB.isLoopInsectLoop(loopsA[nt])) loopsInsectInn.push_back(loopsA[nt]);
					};
				};
				continue;
			};
			if(loopOutA.isLoopCoveredByLoop(loopOutB)) {
				if(loopOutA.isValid()) {
					for(int k = 0; k < BInnersID[m].size(); k++) {
						int mk = BInnersID[m][k];
					    auto tLoop = loopsB[mk];
						if(loopOutA.isLoopInsectLoop(tLoop)) {
							auto sLoop = loopOutA.BoolInt(loopsB[mk]);
							for(int j = 0; j < AInnersID[n].size(); j++) {
								int nj = AInnersID[n][j];
					    		tLoop = loopsA[nj];
								if(sLoop.isLoopInsectLoop(tLoop)) sLoop = sLoop.BoolCut(tLoop);
							};
							if(sLoop.isValid()) loopsInsectOut.push_back(sLoop);
						};
					};
				};
				continue;
			};
			auto loops = loopOutA.VBoolCut(loopOutB);
			for(auto loop : loops) {
				if(!loop.isValid()) continue;
				for(int k = 0; k < AInnersID[n].size(); k++) {
			 	   int nk = AInnersID[n][k];
				   auto loopInnA = loopsA[nk];
				   if(loopInnA.isLoopInsectLoop(loopOutB)) loop = loop.BoolCut(loopInnA);
				};
				if(loop.isValid()) loopsInsectOut.push_back(loop);
				for(int k = 0; k < AInnersID[n].size(); k++) {
			 	   int nk = AInnersID[n][k];
				   auto loopInnA = loopsA[nk];
				   if(loopInnA.isLoopCoveredByLoop(loop)) loopsInsectInn.push_back(loopInnA);
				};
			};

			auto loop = loopOutA.BoolCutLeft(loopOutB);
			if(!loop.isValid()) continue;
			for(int k = 0; k < BInnersID[m].size(); k++) {
			    int mk = BInnersID[m][k];
				auto loopInnB = loopsB[mk];
				if(loopInnB.isLoopInsectLoop(loop)) loopsInsectOut.push_back(loopInnB);
			};
		};
	};

	for(auto loop : loopsInsectInn) loopsInsectOut.push_back(loop);
	return base2d(loopsInsectOut);
};

base2d base2d::BoolInt( base2d& base2dB ) {
	base2d base2dA = base2d(_dashLoops);
	auto AOutersID = base2dA.getOuterLoopsID();
	auto BOutersID = base2dB.getOuterLoopsID();
	auto AInnersID = base2dA.getInnerLoopsID();
	auto BInnersID = base2dB.getInnerLoopsID();
	auto loopsA = base2dA.getDashLoops();
	auto loopsB = base2dB.getDashLoops();

	std::vector<dashLoop> loopsInsectOut;
	std::vector<dashLoop> loopsInsectInn;

	for(int n = 0; n < AOutersID.size(); n++) {
		int nID = AOutersID[n];
		auto loopOutA  = loopsA[nID];
		for(int m = 0; m < BOutersID.size(); m++) {
			int mID = BOutersID[m];
			auto loopOutB = loopsB[mID];
			if(!loopOutB.isLoopInsectLoop(loopOutA)) continue;
			auto loop = loopOutA.BoolInt(loopOutB);
			if(!loop.isValid()) continue;
			for(int k = 0; k < AInnersID[n].size(); k++) {
			        int kID = AInnersID[n][k];
				auto loopInnA = loopsA[kID];
				if(!loopInnA.isLoopInsectLoop(loop)) continue;
				loop = loop.BoolCut(loopInnA);
			};
			for(int k = 0; k < BInnersID[m].size(); k++) {
			        int kID = BInnersID[m][k];
				auto loopInnB = loopsB[kID];
				if(!loopInnB.isLoopInsectLoop(loop)) continue;
				loop = loop.BoolCut(loopInnB);
			};
			if(loop.isValid()) loopsInsectOut.push_back(loop);
			for(int k = 0; k < AInnersID[n].size(); k++) {
			        int kID = AInnersID[n][k];
				auto loopInnA = loopsA[kID];
				if(!loopInnA.isValid()) continue;
				if(loopInnA.isLoopCoveredByLoop(loop)) 
					loopsInsectInn.push_back(loopInnA);
			};
			for(int k = 0; k < BInnersID[m].size(); k++) {
			        int kID = BInnersID[m][k];
				auto loopInnB = loopsB[kID];
				if(!loopInnB.isValid()) continue;
				if(loopInnB.isLoopCoveredByLoop(loop)) 
					loopsInsectInn.push_back(loopInnB);
			};
		};
	};

	for(auto loop : loopsInsectInn) loopsInsectOut.push_back(loop);

	return base2d(loopsInsectOut);
};

base2d base2d::BoolAdd( base2d& base2dB ) {
	base2d base2dA = base2d(_dashLoops);
	auto AOutersID = base2dA.getOuterLoopsID();
	auto BOutersID = base2dB.getOuterLoopsID();
	auto AInnersID = base2dA.getInnerLoopsID();
	auto BInnersID = base2dB.getInnerLoopsID();
	auto loopsA = base2dA.getDashLoops();
	auto loopsB = base2dB.getDashLoops();
	std::vector<dashLoop> loopsInsectOut;
	std::vector<dashLoop> loopsInsectInn;

	for(int n = 0; n < AOutersID.size(); n++) {
		int nID = AOutersID[n];
		auto loopOutA  = loopsA[nID];
		for(int m = 0; m < BOutersID.size(); m++) {
			int mID = BOutersID[m];
			auto loopOutB = loopsB[mID];
			if(!loopOutB.isLoopInsectLoop(loopOutA)) {
				if(loopOutA.isValid()) {
				    loopsInsectOut.push_back(loopOutA);
					for(int k = 0; k < AInnersID[n].size(); k++) {
						auto tLoop = loopsA[AInnersID[n][k]];
					    loopsInsectInn.push_back(tLoop);
					};
				};
				if(loopOutB.isValid()) {
				    loopsInsectOut.push_back(loopOutB);
					for(int k = 0; k < BInnersID[m].size(); k++) {
						auto tLoop = loopsB[BInnersID[m][k]];
					    loopsInsectInn.push_back(tLoop);
					};
				};
				continue;
			};
			auto loop = loopOutA.BoolAdd(loopOutB);
			if(loop.isValid()) loopsInsectOut.push_back(loop);
			for(int k = 0; k < AInnersID[n].size(); k++) {
			        int nk = AInnersID[n][k];
				auto loopInnA = loopsA[nk];
				if(!loopInnA.isValid()) continue;
				if(loopInnA.isLoopInsectLoop(loopOutB)) {
					for(int j = 0; j < BInnersID[m].size(); j++) {
			 	 	      int mj = BInnersID[m][j];
						auto loopInnB = loopsB[mj];
						if(!loopInnB.isValid()) continue;
						if(loopInnB.isLoopInsectLoop(loopInnA)) {
							loopInnA = loopInnA.BoolAdd(loopInnB); 
						};
					};
				} 
				loopsInsectInn.push_back(loopInnA);
			};
			for(int k = 0; k < BInnersID[m].size(); k++) {
			        int mk = AInnersID[m][k];
				auto loopInnB = loopsB[mk];
				if(!loopInnB.isValid()) continue;
				bool Yes = true;
				for(int j = 0; j < AInnersID[n].size(); j++) {
			  	      int nj = AInnersID[n][j];
					auto loopInnA = loopsA[nj];
					if(!loopInnA.isValid()) continue;
					if(loopInnA.isLoopInsectLoop(loopInnB)) Yes = false;
				};
				if( Yes ) loopsInsectInn.push_back(loopInnB);
			};
		};
	};
	for(auto loop : loopsInsectInn) loopsInsectOut.push_back(loop);

	return base2d(loopsInsectOut);
};

void base2d::update() {
	_update = false;
	getDashLoops();
	getDashTrains();
	getDashes();
	getVertexes();
	_update = true;
};

void base2d::Print(std::string myStr) {
	for(int n = 0; n < _dashLoops.size(); n++) {
		auto uStr = myStr + std::to_string(n) ;
		uStr += " Loop " ;
		_dashLoops[n].loopPrint( uStr );
	};
};

void base2d::Export2GMSH_Geo(const std::string& fileName, const std::string& myStr, 
		double scaling, int entry) {
	if(!isValid()) return;

    	std::ofstream file(fileName);  // Create an output file stream
    
    	// Redirect the standard output stream to the file stream
    	std::streambuf* originalStreamBuf = std::cout.rdbuf();
    	std::cout.rdbuf(file.rdbuf());
    
	auto uStr = "// GMSH geo format " + myStr ;
	uStr = "  ";
	std::cout << uStr << std::endl;
	uStr = "// total points " + std::to_string(_vertexes.size()) ;
	std::cout << uStr << std::endl;
	for(int n = 0; n < _vertexes.size(); n++) {
		auto vec = _vertexes[n];
		uStr = "Point(" + std::to_string(n+1) + ") = {" ;
		uStr += std::to_string(vec[0]) + ", " ;
		uStr += std::to_string(vec[1]) + ", " ;
		uStr += std::to_string(0.0) + ", " ;
		uStr += std::to_string(scaling) + "};";
		std::cout << uStr << std::endl;
	};

	uStr = "  ";
	std::cout << uStr << std::endl;
	uStr = "// total dashes " + std::to_string(_dashes.size()) ;
	std::cout << uStr << std::endl;
	for(int n = 0; n < _dashes.size(); n++) {
		auto dash = _dashes[n];
		uStr = "Line(" + std::to_string(n+1) + ") = {" ;
		uStr += std::to_string(index(dash.P1)+1) + ", " ;
		uStr += std::to_string(index(dash.P2)+1) + "}; " ;
		std::cout << uStr << std::endl;
	};

	uStr = "  ";
	std::cout << uStr << std::endl;
	uStr = "// total Line Loops " + std::to_string(_dashLoops.size()) ;
	std::cout << uStr << std::endl;
	for(int n = 0; n < _dashLoops.size(); n++) {
		auto loop = _dashLoops[n];
		uStr = "Line Loop(" + std::to_string(n+1) + ") = {" ;
		auto dashes = loop.getDashes();
		for(int m = 0; m < dashes.size() -1; m++) {
			auto dash = dashes[m];
			uStr += std::to_string(index(dash)+1) + ", " ;
		};
		uStr += std::to_string(index(dashes.back())+1) + "}; " ;
		std::cout << uStr << std::endl;
	};

	uStr = "  ";
	std::cout << uStr << std::endl;
	uStr = "// Plane Surface "; 
	std::cout << uStr << std::endl;
	uStr = ") = {" ;
	for(int n = 0; n < _dashLoops.size()-1; n++) {
		uStr += std::to_string(n+1) + ", " ;
	};
	uStr += std::to_string(_dashLoops.size()) + "}; " ;
	uStr = "Plane Surface(" + std::to_string(entry) + uStr ;
	std::cout << uStr << std::endl;

        // dumping out
    	std::cout.rdbuf(originalStreamBuf);
    	file.close();
};

void base2d::Translate(vec2d vec) {
	for(auto& loop:_dashLoops) loop.loopTranslate(vec);
	update();
};

void base2d::RotDegree(double factor) {
	for(auto& loop:_dashLoops) loop.loopRotDegree(factor);
	update();
};

} // namespace geo2d

EW_LEAVE




