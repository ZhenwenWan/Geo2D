
#include "geo2d.h"
#include "dash.h"
#include "dashTrain.h"
#include "dashLoop.h"
#include "base2d.h"
#include "polygon.h"
#include <iostream>
using namespace ew;
using namespace ew::geo2d;

EW_ENTER

void OnAssertFailure(char const* ms, char const* file_name, long line) {
std::cout << ms << std::endl;
std::cout << file_name << std::endl;
std::cout << line << std::endl;
}
;


EW_LEAVE

int main() {
	polygon q4(4,1.0);
	polygon q6(6,1.0);
	auto q64 = q6.VBoolCut(q4);
	q64.Export2GMSH_Geo("q64.geo","");

	std::vector<vec2d> ps;
	ps.clear();
	ps.push_back(vec2d(-4.7,-3.0));
	ps.push_back(vec2d(4.7,-3.0));
	ps.push_back(vec2d(4.7,1.2));
	ps.push_back(vec2d(-4.7,1.2));
	polygon p41(4,ps);
	ps.clear();
	ps.push_back(vec2d(-4.7,-2.0));
	ps.push_back(vec2d(4.7,-2.0));
	ps.push_back(vec2d(4.7,0.9));
	ps.push_back(vec2d(-4.7,0.9));
	polygon p42(4,ps);
	
	auto p4A = p41.BoolAdd(p42);
	p4A.Export2GMSH_Geo("p4A.geo","");
	auto p4I = p41.BoolInt(p42);
	p4I.Export2GMSH_Geo("p4I.geo","");
	auto p4C = p41.BoolCut(p42);
	p4C.Export2GMSH_Geo("p4C.geo","");
	auto p4VC = p41.VBoolCut(p42);
	p4VC.Export2GMSH_Geo("p4VC.geo","");

	//how to translate via a vector E
	std::vector<dashLoop> loops;
	polygon p4(4,0.50);
	auto p4r = p4;
	p4r.Export2GMSH_Geo("p4r.geo","");
	p4.RotDegree(45.0);
    p4.Translate(vec2d(0.8,0));
	p4.Export2GMSH_Geo("p4.geo","");

	polygon p5(5,1.0);
	polygon p6(6,1.0);
	polygon p7(7,0.6);
    auto p50 = p5;
    auto p60 = p6;
    p5.Zooming(0.3);
    p6.Zooming(0.3);
	p6.Export2GMSH_Geo("p6.geo","");
    p6.Translate(vec2d(0.3,0.3));
	
	auto Int56 = p50.BoolInt(p60);
	Int56.Export2GMSH_Geo("Int56.geo","");

	auto Int5_6_3 = p50.BoolAdd(p60);
	Int5_6_3.Export2GMSH_Geo("Int5_6_3.geo","");

	auto Cut5_6_0 = p50.BoolCut(p6);
	Cut5_6_0.Export2GMSH_Geo("Cut5_6_0.geo","");

	auto Cut5_6_7 = Cut5_6_0.BoolCut(p4);
	Cut5_6_7.Export2GMSH_Geo("Cut5_6_7.geo","");
	
	polygon p8(8,0.6);
	polygon p9(9,0.6);
	auto p80 = p8;
	auto C5678 = Cut5_6_7.BoolCut(p8);
	C5678.Export2GMSH_Geo("C5678.geo","");
    
	p8.Translate(vec2d(0.0,-1.2));
	auto C8 = Cut5_6_7.BoolAdd(p8);
	C8.Export2GMSH_Geo("C81.geo","");
	
	auto C9 = p9.BoolAdd(C8);
	C9.Export2GMSH_Geo("C91.geo","");

    C8.Zooming(0.3,0.1);
	C9 = p9.BoolAdd(C8);
	C8.Export2GMSH_Geo("C92.geo","");

    C8.Translate(vec2d(0.1,-0.3));
	C9 = p9.BoolAdd(C8);
	C9.Export2GMSH_Geo("C93.geo","");

    C8.RotDegree(30.0);
	C9 = p9.BoolAdd(C8);
	C9.Export2GMSH_Geo("C94.geo","");


	return 0;
}




