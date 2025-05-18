#ifndef POLYGON_H
#define POLYGON_H
#include "base2d.h"

EW_ENTER

namespace geo2d {

class polygon : public base2d {
	public:
		int Num;  // polygon vertexes
		polygon(){};
		polygon(int n, std::vector<vec2d> verts) : base2d(aLoops(n, verts)) {};
		polygon(int n, double radius) : base2d(bLoops(n, radius)) {};
		~polygon(){};

	protected:

	private:
		std::vector<dashLoop> aLoops(int n, std::vector<vec2d> verts);
		std::vector<dashLoop> bLoops(int n, double radius);

};

} // namespace geo2d

EW_LEAVE



#endif

