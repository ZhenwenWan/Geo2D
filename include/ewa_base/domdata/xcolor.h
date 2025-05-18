
#ifndef __H_EW_DOMDATA_XCOLOR__
#define __H_EW_DOMDATA_XCOLOR__


#include "ewa_base/basic/string.h"
#include "ewa_base/serialization/serializer.h"

EW_ENTER


class RGBValue
{
public:
	uint8_t r, g, b;
};

class Variant;

class DLLIMPEXP_EWA_BASE DColor
{
public:

	DColor();
	DColor(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_ = 255u);

	explicit DColor(const Variant& v);
	explicit DColor(const String& c);
	explicit DColor(uint32_t v);


	operator const uint8_t*() const;

	void set(uint8_t r_, uint8_t g_, uint8_t b_);
	void set(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_);

	bool set_by_string(const String& c);
	bool set_by_name(const String& cname);
	bool set_by_rgba(uint32_t v);

	String cast_to_string() const;
	int32_t cast_to_int() const{ return *(int32_t*)this; }

	uint8_t r, g, b, a;

	void Serialize(SerializerHelper sh);

	typedef const DColor& (*color_func_t)();
	static indexer_map<String, color_func_t>& named_colormap();

#define __DEFINE_COLOR_BY_STRING__(NAME, VAL) static const DColor& NAME() {static const DColor _color(VAL); return _color;}
	__DEFINE_COLOR_BY_STRING__(Orange, 0xff3232cc);
	__DEFINE_COLOR_BY_STRING__(Goldenrod, 0xff70dbdb);
	__DEFINE_COLOR_BY_STRING__(Wheat, 0xffbfd8d8);
	__DEFINE_COLOR_BY_STRING__(SpringGreen, 0xff7fff00);
	__DEFINE_COLOR_BY_STRING__(SkyBlue, 0xffcc9932);
	__DEFINE_COLOR_BY_STRING__(SlateBlue, 0xffff7f00);
	__DEFINE_COLOR_BY_STRING__(MediumVioletRed, 0xff9370db);
	__DEFINE_COLOR_BY_STRING__(Purple, 0xffff00b0);
	__DEFINE_COLOR_BY_STRING__(Red, 0xff0000ff);
	__DEFINE_COLOR_BY_STRING__(Yellow, 0xff00ffff);
	__DEFINE_COLOR_BY_STRING__(MediumSpringGreen, 0xff00ff7f);
	__DEFINE_COLOR_BY_STRING__(PaleGreen, 0xff8fbc8f);
	__DEFINE_COLOR_BY_STRING__(Cyan, 0xffffff00);
	__DEFINE_COLOR_BY_STRING__(LightSteelBlue, 0xffbc8f8f);
	__DEFINE_COLOR_BY_STRING__(Orchid, 0xffdb70db);
	__DEFINE_COLOR_BY_STRING__(LightMagenta, 0xffff77ff);
	__DEFINE_COLOR_BY_STRING__(Brown, 0xff2a2aa5);
	__DEFINE_COLOR_BY_STRING__(Green, 0xff00ff00);
	__DEFINE_COLOR_BY_STRING__(CadetBlue, 0xff9f9f5f);
	__DEFINE_COLOR_BY_STRING__(MediumBlue, 0xffcc3232);
	__DEFINE_COLOR_BY_STRING__(Magenta, 0xffff00ff);
	__DEFINE_COLOR_BY_STRING__(Maroon, 0xff6b238e);
	__DEFINE_COLOR_BY_STRING__(OrangeRed, 0xff7f00ff);
	__DEFINE_COLOR_BY_STRING__(Firebrick, 0xff23238e);
	__DEFINE_COLOR_BY_STRING__(Coral, 0xff007fff);
	__DEFINE_COLOR_BY_STRING__(ForestGreen, 0xff238e23);
	__DEFINE_COLOR_BY_STRING__(Aquamarine, 0xff93db70);
	__DEFINE_COLOR_BY_STRING__(Blue, 0xffff0000);
	__DEFINE_COLOR_BY_STRING__(Navy, 0xff8e2323);
	__DEFINE_COLOR_BY_STRING__(Thistle, 0xffd8bfd8);
	__DEFINE_COLOR_BY_STRING__(IndianRed, 0xff2f2f4f);
	__DEFINE_COLOR_BY_STRING__(Gold, 0xff327fcc);
	__DEFINE_COLOR_BY_STRING__(MediumSeaGreen, 0xff426f42);
	__DEFINE_COLOR_BY_STRING__(MidnightBlue, 0xff4f2f2f);
	__DEFINE_COLOR_BY_STRING__(Grey, 0xff808080);
	__DEFINE_COLOR_BY_STRING__(Black, 0xff000000);
	__DEFINE_COLOR_BY_STRING__(MediumForestGreen, 0xff238e6b);
	__DEFINE_COLOR_BY_STRING__(Khaki, 0xff5f9f9f);
	__DEFINE_COLOR_BY_STRING__(DarkGrey, 0xff2f2f2f);
	__DEFINE_COLOR_BY_STRING__(SeaGreen, 0xff6b8e23);
	__DEFINE_COLOR_BY_STRING__(LightGrey, 0xffc0c0c0);
	__DEFINE_COLOR_BY_STRING__(MediumSlateBlue, 0xffff007f);
	__DEFINE_COLOR_BY_STRING__(White, 0xffffffff);

	__DEFINE_COLOR_BY_STRING__(Lime, "#00ff00");
	__DEFINE_COLOR_BY_STRING__(Gray, "#808080");
	__DEFINE_COLOR_BY_STRING__(Olive, "#808000");
	__DEFINE_COLOR_BY_STRING__(Silver, "#c0c0c0");
	__DEFINE_COLOR_BY_STRING__(Teal, "#008080");
	__DEFINE_COLOR_BY_STRING__(VividYellow, "#ffd400");
	__DEFINE_COLOR_BY_STRING__(Calamus, "#c77eb5");
	__DEFINE_COLOR_BY_STRING__(AncientPurple, "#7d5886");
	__DEFINE_COLOR_BY_STRING__(Lazuli, "#2a5caa");
	__DEFINE_COLOR_BY_STRING__(Galois, "#7f7522");
#undef __DEFINE_COLOR_BY_STRING__

	static DColor rgb(double red, double green, double blue, double alpha = 1.0);

	/* gray:
	* 0:   white
	* 1.0: black
	*/
	static DColor gray(double g, double alpha = 1.0);

	/* hue ring:
	* 0:   red
	* 60:  yellow
	* 120: green
	* 180: cyan
	* 240: blue
	* 300: magenta
	* saturation: 0 - 1.0
	* value:      0 - 1.0
	*/
	static DColor hsv(double hue /* 色调，角度制0DEG-360DEG */, double saturation /* 饱和度 */, double value /* 明度 */, double alpha = 1.0);

	/* hue ring:
	* 0:   red
	* 60:  yellow
	* 120: green
	* 180: cyan
	* 240: blue
	* 300: magenta
	* saturation: 0 - 1.0
	* lightness:  0 - 1.0
	*/
	static DColor hsl(double hue /* 色调，角度制0DEG-360DEG */, double saturation /* 饱和度 */, double lightness /* 亮度 */, double alpha = 1.0);

	static DColor RainbowClassic(int ci, int n);
};

inline bool operator==(const DColor lhs, const DColor rhs)
{
	return lhs.cast_to_int() == rhs.cast_to_int();
}
inline bool operator!=(const DColor lhs, const DColor rhs)
{
	return lhs.cast_to_int() != rhs.cast_to_int();
}
inline bool operator<(const DColor lhs, const DColor rhs)
{
	return lhs.cast_to_int() < rhs.cast_to_int();
}
inline bool operator<=(const DColor lhs, const DColor rhs)
{
	return lhs.cast_to_int() <= rhs.cast_to_int();
}
inline bool operator>(const DColor lhs, const DColor rhs)
{
	return lhs.cast_to_int() > rhs.cast_to_int();
}
inline bool operator>=(const DColor lhs, const DColor rhs)
{
	return lhs.cast_to_int() >= rhs.cast_to_int();
}

EW_LEAVE

namespace tl
{
	template<> struct is_pod<ew::RGBValue> : public value_type<true>{};
};

#endif

