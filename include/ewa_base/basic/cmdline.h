#ifndef __H_EW_BASIC_CMDLINE__
#define __H_EW_BASIC_CMDLINE__

#include "ewa_base/config.h"
#include "ewa_base/basic/string.h"
#include "ewa_base/collection/arr_1t.h"
#include "ewa_base/collection/bst_set.h"
#include "ewa_base/collection/bst_map.h"

EW_ENTER

class DLLIMPEXP_EWA_BASE CmdLineData
{
public:
	CmdLineData();
	~CmdLineData();

	bool match(const String& s);
	bool match(const String& s, String& v);
	bool match(const String& s, int& v);

	bool match_option(const String& s){String v; return match(s, v);}

	bool found(const String& s);
	bool found(const String& s, String& v);
	bool found(const String& s, int& v);


	void update(int argc, char** argv);
	void update(int argc, wchar_t** argv);
	void update(const String& cmdline);

	void set(const String& name, const String& value);

	arr_1t<String> args;
	arr_1t<char*> argv;
	int argc;

protected:

	bst_map<String, String> vals;
	
	void _do_update();
};


class AppModuleData
{
public:
	String name;
	String version;
	arr_1t<String> info;

	static AppModuleData& get(const String& name);
};


class DLLIMPEXP_EWA_BASE CmdLineParser
{
public:

	enum
	{
		CMDLINE_SWITCH,
		CMDLINE_OPTION,
		CMDLINE_PARAM,
	};

	CmdLineParser();
	~CmdLineParser();

	static CmdLineParser& current();

	void AddSwitch(const String& s);
	void AddOption(const String& s);
	void AddOption(const String& s,const String& d);

	void AddParam(int n = 1);

	bool parse(int argc, char** argv);
	bool parse(const arr_1t<String>& argv);
	bool parse();

	void show_usage();
	void show_version();

	bool Found(const String& s);
	bool Found(const String& s, String& v);
	bool Found(const String& s, int32_t& v);
	bool Found(const String& s, int64_t& v);
	bool Found(const String& s, double& v);

	int64_t GetValue(const String& s, int64_t v);
	String GetValue(const String& s, const String& v);

	size_t GetParamCount();
	String& GetParam(size_t i = 0);

	void SetModuleData(const AppModuleData& p);


	typedef bst_map<String, String>::iterator option_iterator;

	option_iterator options_begin();
	option_iterator options_end();

	void SetParam(const String& value);


private:

	class OptionType
	{
	public:
		OptionType():type(0),tags(0){}
		void set(int t,int n=0){type=t;tags=n;}
		int type;
		int tags;
	};

	bst_map<String, OptionType> m_aCmdLineOptions;
	bst_map<String, String> m_aOptionsDefault;

	arr_1t<String> m_aParams;
	bst_set<String> m_aSwitchs;
	bst_map<String, String> m_aOptions;
	AppModuleData m_tModuleData;
};

EW_LEAVE
#endif
