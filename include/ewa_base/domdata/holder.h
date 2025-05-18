
#ifndef __H_EW_DOMDATA_HOLDER__
#define __H_EW_DOMDATA_HOLDER__

#include "ewa_base/domdata/dobject.h"
#include "ewa_base/domdata/symm.h"
#include "ewa_base/domdata/daxis_d.h"
#include "ewa_base/domdata/dfigdata.h"
#include "ewa_base/domdata/dfigure.h"

EW_ENTER


class FigureHolder
{
public:

	DFigure* new_figure();
	DFigure* get_figure();

	void set_figure(DFigure* p);

	static const int CPX_MODE_PSEUDO_TIME = 0;
	static const int CPX_MODE_MAGNITUDE   = 1;
	static const int CPX_MODE_REALPART    = 2;
	static const int CPX_MODE_IMAGPART    = 3;
	static const int CPX_MODE_PHASE_1     = 4; // Phase: -180 ~ +180
	static const int CPX_MODE_PHASE_2     = 5; // Phase: 0 ~ 360

	void data(DFigData* pdat);

	void plot(Variant x, Variant y);
	void plot(Variant y);
	void cpx_mode(Variant mode);

	void line_style(int n,double d);
	void marker_style(int n, double d);
	void line_style(int n);
	void marker_style(int n);
	void line_style(Variant s);
	void marker_style(Variant s);
	void line_width(double d);
	void marker_size(double d);
	void line_color(DColor c);
	void marker_color(DColor c);

	void set_line_property(const String& propname, Variant value);

	void polar(Variant x, Variant y);
	void polar_deg(Variant x, Variant y);


	void polar3d(Variant x, Variant y, Variant r,Variant v=Variant(),Variant opt=Variant());
	void polar3d_deg(Variant x, Variant y, Variant r, Variant v = Variant(),Variant opt=Variant());

	void model(Variant m,int t=0,const mat4d& m4=mat4d::id,double alpha=1.0);
	void stlmesh(Variant m, int type, Variant value);

	void image3d(Variant x, Variant y, Variant v,double scale,double shift);
	void image3d(Variant x, Variant y, Variant v);
	void image3d(Variant v);

	void imagesc(Variant x, Variant y, Variant v);
	void imagesc(Variant v);

	void bar3d(Variant data, Variant width);

	void info(const String& v,bool no_legend=false);
	void tip_func(CallableData* p);

	void add_text(const String& value, const String& name,const vec3d& pos,const vec3d& shf,const vec3d& pxl);

	void add_tip_text(Variant p, const String& v = empty_string, const String& n = empty_string);
	void add_tip_line(Variant p1, Variant p2, const String& v = empty_string, const String& n = empty_string);


	void delay_translate(bool v);

	void title(const String& v);
	void xlabel(const String& v);
	void ylabel(const String& v);
	void legend(bool flag);

	void axis_dir(int d, double lo, double hi);
	void axis(double xmin, double xmax, double ymin, double ymax);
	void axis_equal(bool flag);
	void axis_rmin(double r, bool b);
	void axis_cut_minus(bool b);
	void axis_polar(double amin,bool minus,bool mirror);

	void colormap(bool flag);
	void colormap(double vmin, double vmax);
	void colormap(const arr_1t<XColorItem>& val);
	void colormap(const String& option);
	void colormap(const Variant& var);

	void colormap_mode(int f); // 自动范围
	void colormap_sym(bool f);  // 自动对称

	void data_style(const String& v, int f=0, int n = -1);

	void axis_range(int d, double& lo, double& hi);

	void data_mapping(double scale, double shift, double val_0);
	void data_mapping(bool enable);

	void grid(int r, int c);
	void row();
	void col();
	void end();

	void end_coord(const String& v=empty_string);
	void end_item(const String& v);

	void set_property(const String& name, Variant value);

	
protected:

	void _push_state(DataPtrT<DObject> p);
	void _popq_state();

	
	void _ensure_figure();

	// Check if the item is type T
	// true: just return
	// fasle: throw an exception
	template<typename T>
	void _check_item_type();

	// Check if the item is type T
	// true: return its reference
	// false: create a new instance and return its reference 
	template<typename T>
	T* _ensure_item_type();

	// Check if the item is type T
	// true: return its pointer
	// false: throw an exception
	template<typename T>
	T& _assert_item_type();

	// Check if the data is type T
	// true: return its pointer
	// false: throw an exception
	template<typename T>
	T& _assert_data_type();

	// Create a new item of type T and return its reference
	template<typename T>
	T& _create_data_type();

	void kerror(const String&);
	void kerror();

	DataPtrT<DFigure> m_refData;

	void _imagesc(Variant x,Variant y,Variant z);
	void _set_pos(arr_xt<double>& pos, Variant val, size_t n);

};



EW_LEAVE

#endif

