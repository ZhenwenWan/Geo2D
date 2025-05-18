#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "base2d.h"

EW_ENTER

namespace geo2d {

class triangle : public base2d {
	public:
		vec2d A, B, C;  // triangle vertexes
		triangle(){};
		triangle(vec2d p1,vec2d p2, vec2d p3) : base2d(aLoops(p1,p2,p3)) {};
		~triangle(){};

	protected:

	private:
		std::vector<dashLoop> aLoops(vec2d p1,vec2d p2,vec2d p3);

};

} // namespace geo2d

EW_LEAVE



#endif

