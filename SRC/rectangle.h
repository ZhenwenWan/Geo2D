#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "base2d.h"

EW_ENTER

namespace geo2d {

class rectangle : public base2d {
	public:
		vec2d LLeft;  // the lower left point
		vec2d LRight; // the lower right point
		vec2d ULeft;  // the upper left point
		vec2d URight; // the upper right point
		vec2d center; // the central point
		double width, height;
		rectangle(){};
		rectangle(vec2d pnt1,vec2d pnt2) : base2d(aLoops(pnt1,pnt2)) {};
		~rectangle(){};

	protected:

	private:
		std::vector<dashLoop> aLoops(vec2d pnt1,vec2d pnt2);

};

} // namespace geo2d

EW_LEAVE



#endif

