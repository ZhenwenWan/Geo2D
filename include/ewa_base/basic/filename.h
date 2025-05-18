#ifndef __H_EW_BASIC_FILENAME__
#define __H_EW_BASIC_FILENAME__

#include "ewa_base/config.h"
#include "ewa_base/basic/string_detail.h"
#include "ewa_base/basic/string.h"
#include "ewa_base/basic/platform.h"
#include "ewa_base/basic/bitflags.h"
#include "ewa_base/basic/exception.h"
#include "ewa_base/basic/object.h"

EW_ENTER


class DLLIMPEXP_EWA_BASE FileName : public mp_obj
{
public:

	enum
	{
		FLAG_ISOK		= 1 << 0,
		FLAG_RELATIVE	= 1 << 1,
		FLAG_NETSHARE	= 1 << 2,
	};

	FileName();
	FileName(const String& fn);
	FileName(const String& fn,const String& cwd);
	FileName(const String& fn,const String& path,const String& cwd);
	FileName(const arr_1t<String>& arr);

	void set_path(const String& fn);
	void set_path(const String& fp, const String& cwd);
	void set_path(const String& fp, const String& path,const String& cwd);

	void set_cwd(const String& fn,size_t n=1);

	String get_name() const;
	String get_fullpath() const;
	String get_filename() const;
	String get_pathonly(bool sep) const;
	String get_ext() const;
	String get_origin() const;

	bool make_relative_to(const String& s);

	bool is_ok() const;
	bool is_relative() const;

	void Serialize(SerializerHelper);

protected:

	static void _path_split(const String& fn, arr_1t<String>& arr);
	static bool _is_relative(const arr_1t<String>& arr);
	static void _normal_path(arr_1t<String>& arr);

	void _update();

	//String m_sOrigin;
	//String m_sCwd;
	arr_1t<String> m_aFilename;
	arr_1t<String> m_aPath;
	BitFlags flags;

};

EW_LEAVE

#endif
