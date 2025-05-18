#ifndef __H_EW_BASIC_CONFIG__
#define __H_EW_BASIC_CONFIG__

#include "ewa_base/scripting/callable_table.h"

EW_ENTER


class DLLIMPEXP_EWA_BASE ConfigBase : public CallableObject
{
public:

	String GetString(const String& key, const String& def = "");


	bool Load(const String& s = "");
	bool Save(const String& s = "");

	bool Init(const String& s);

	void CfgUpdate(int lv, const String& s, double& v, double v1, double v2);

	void CfgUpdate(int lv, const String& s, int32_t& v, int v1, int v2);
	void CfgUpdate(int lv, const String& s, int32_t& v);
	void CfgUpdate(int lv, const String& s, int64_t& v);
	void CfgUpdate(int lv, const String& s, String& v);
	void CfgUpdate(int lv, const String& s, DColor& v);
	void CfgUpdate(int lv, const String& s, double& v);
	void CfgUpdate(int lv, const String& s, TimePoint& v);
	void CfgUpdate(int lv, const String& s, arr_1t<String>& v);

	void CfgUpdate(int lv, const String& s, BitFlags& v, int m);

	typedef VariantTable map_type;
	map_type values;

	static ConfigBase& GetConfig(const String& fp);
	static ConfigBase& current();

	int __getindex(Executor&, const String&);

protected:
	String s_file;
	AtomicMutex mutex;

	template<typename T>
	void SetValue(const String& key, const T& value)
	{
		LockGuard<AtomicMutex> lock(mutex);
		values[key].reset(value);
	}

	template<typename T>
	bool GetValue(const String& key, T& value)
	{
		LockGuard<AtomicMutex> lock(mutex);
		if (values.find1(key) < 0)
		{
			return false;
		}
		if (values[key].get<T>(value))
		{
			return true;
		}
		return false;
	}
};


class IniConfigFile
{
public:

	bool readfile(const String& file);

	bool run_cmd(const String& cmd);

	bool parse(const char* p0);

	bool get(const String& name, String& val);

protected:
	bst_map<String, String> nvmap;

};

class MemoryCtxData;

class IniConfigFileEx : public IniConfigFile
{
public:

	template<typename T>
	bool get_memory_value(const String& name, T& val)
	{
		auto it = nvmap.find(name);
		if (it == nvmap.end()) return false;
		return _get_memory_value((*it).second.c_str(), val);
	}

	template<typename T>
	static bool _get_memory_value(const char* p0, T& val)
	{
		const char* p1 = p0;

		while (*p1 && *p1 == ' ') p1++;

		val = 0;
		while (*p1 >= '0' && *p1 <= '9')
		{
			val = val * 10 + (*p1) - '0';
			p1++;
		}

		while (*p1 && *p1 == ' ') p1++;


		if ((p1[0] == 'k' || p1[0] == 'K') && p1[1] == 'B')
		{
			val = val * 1024;
			return true;
		}

		if ((p1[0] == 'm' || p1[0] == 'M') && p1[1] == 'B')
		{
			val = val * 1024 * 1024;
			return true;
		}

		if ((p1[0] == 'g' || p1[0] == 'G') && p1[1] == 'B')
		{
			val = val * 1024 * 1024 * 1024;
			return true;
		}

		if (p1[0] == 0)
		{
			return true;
		}

		return false;

	}

	bool get_memory_status(MemoryCtxData& ctx);

};


EW_LEAVE
#endif

