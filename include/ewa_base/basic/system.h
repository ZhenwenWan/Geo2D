#ifndef __H_EW_BASIC_SYSTEM__
#define __H_EW_BASIC_SYSTEM__

#include "ewa_base/config.h"
#include "ewa_base/basic/string_detail.h"
#include "ewa_base/basic/string.h"
#include "ewa_base/basic/platform.h"
#include "ewa_base/basic/bitflags.h"
#include "ewa_base/basic/exception.h"

#ifdef EW_MSVC
#pragma comment(lib, "Iphlpapi.lib")
#endif

#define EW_FUNCTION_TRACER(lv) FunctionTracer __function_tracer(__FUNCTION__,lv);

#define EW_FUNCTION_TRACER_LOG_INFO() FunctionTracer __function_tracer(__FUNCTION__,LOGLEVEL_INFO, false);

EW_ENTER

class DLLIMPEXP_EWA_BASE Stream;
class DLLIMPEXP_EWA_BASE CmdLineData;
class DLLIMPEXP_EWA_BASE VariantTable;

template<typename T>
class DLLIMPEXP_EWA_BASE StringBufferT;




class CmdCommandBuffer : protected StringBufferT < wchar_t >
{
public:
	typedef StringBufferT < wchar_t > basetype;
	CmdCommandBuffer(const String& s);

    operator const char*();
    operator const wchar_t*();

	using basetype::data;

protected:
    String s_buffer;
};


class MemoryCtxData
{
public:

	MemoryCtxData();

	void clear();

	int64_t m_nHint;
	int64_t m_nMemTotalPhys;
	int64_t m_nMemAvailPhys;
	int64_t m_nMemTotalVirtual;
	int64_t m_nMemAvailVirtual;
	int64_t m_nMemTotalPageFile;
	int64_t m_nMemAvailPageFile;

	int64_t m_nMemWorkingSetSize;
	int64_t m_nMemPeakWorkingSetSize;

};

class CPUID
{
public:


	String serial;
	BitFlags flags;

	enum
	{
		FLAG_SUPPORT_SSE=1<<0,
		FLAG_SUPPORT_AVX=1<<1,
	};

	static CPUID& GetCpu(int id = 1);

};


class StartupInfo;



class DLLIMPEXP_EWA_BASE System
{
public:


	static String Args2Command(const arr_1t<String>& args);

	static bool Execute(const String& s,StartupInfo* p);

	static bool Execute(const String& s,bool showwin);
	static bool Execute(const String& s,int& ret,bool showwin);
	static bool Execute(const String& s,StringBuffer& result, bool showwin,int* pret=NULL);
	static bool ExecuteRedirect(const String& s,Stream& stream, bool showwin);

	static const String& GetModulePath();

	static int GetCacheLineSize();
	static int GetCpuCount();
	static int GetPageSize();

	static int GetPid();

	static double GetCpuTime();

	// Debug break.
	static void DebugBreak();

	// Exit process with return code v.
	static void Exit(int v);

	static void CheckErrno(const String& s);
	static void CheckError(const String& s);

	static void SetLastError(const String& msg);
	static String GetLastError();

	static bool GetMemoryStatus(MemoryCtxData& ctx);


	static String GetUsername();
	static String GetEnv(const String& name,const String& value_if_not_found="");

	static bool IsPathSep(char ch);
	static char GetPathSep();
	static String GetPathSepAsString();

	static String AdjustPath(const String& path,bool sep);
	static String MakePath(const String& file,const String& path);
	static String MakePath(const arr_1t<String>& arr);
	static bool IsRelative(const String& file);

	static String GetCwd();
	static bool SetCwd(const String& s);

	static String GetHardwareSignature();

	static int64_t GetMachineHint();

	static CmdLineData& GetCmdData();

	static size_t Backtrace(void** stack,size_t frames,size_t skip=0);

#ifndef EW_NDEBUG
	STRING_FORMATER_FORMAT_FUNCS_SB(static void LogDebug,DoLog2(LOGLEVEL_DEBUG,fb.c_str());)
#else
	inline static void LogDebug(...) {}
#endif


	STRING_FORMATER_FORMAT_FUNCS_SB(static void LogTrace,DoLog2(LOGLEVEL_TRACE,fb.c_str());)
	STRING_FORMATER_FORMAT_FUNCS_SB(static void Print,DoLog2(LOGLEVEL_PRINT,fb.c_str());)
	STRING_FORMATER_FORMAT_FUNCS_SB(static void PrintLn,DoLog2(LOGLEVEL_PRINTLN,fb.c_str());)
	STRING_FORMATER_FORMAT_FUNCS_SB(static void LogMessage,DoLog2(LOGLEVEL_MESSAGE,fb.c_str());)
	STRING_FORMATER_FORMAT_FUNCS_SB(static void LogCommand,DoLog2(LOGLEVEL_COMMAND,fb.c_str());)
	STRING_FORMATER_FORMAT_FUNCS_SB(static void LogWarning,DoLog2(LOGLEVEL_WARNING,fb.c_str());)
	STRING_FORMATER_FORMAT_FUNCS_SB(static void LogError,DoLog2(LOGLEVEL_ERROR,fb.c_str());)
	STRING_FORMATER_FORMAT_FUNCS_SB(static void LogFatal,DoLog2(LOGLEVEL_FATAL,fb.c_str());)

	STRING_FORMATER_FORMAT_FUNCS_SB(static void LogInfo,DoLog2(LOGLEVEL_INFO,fb.c_str());)


	static bool SetLogFile(const String& fn,bool app=true);

	// default is disabled
	static void SetLogEnable(bool f);

#define P4_LOGLEVEL int v,
	STRING_FORMATER_FORMAT_FUNCS_4(static void DoLog, FormatStateSb, DoLog2(v, fb.c_str()), P4_LOGLEVEL)
#undef P4_LOGLEVEL


	static void DoLog2(int v,const char* msg);

};


class FunctionTracer
{
public:
	FunctionTracer(const char* s,int lv=LOGLEVEL_TRACE,bool sys=true);
	~FunctionTracer();
private:
	const char* func;
	int level;
	bool b_sys;
};


class DLLIMPEXP_EWA_BASE CwdLocker
{
public:
	CwdLocker();
	CwdLocker(const String& file);

	~CwdLocker();

protected:
	String old_cwd;
};


class ProcessMonitor : public NonCopyableAndNonNewable
{
public:

	ProcessMonitor();
	~ProcessMonitor();

	static ProcessMonitor& current();

	void handle(MemoryCtxData& ctx, const char* phase="");

	void start();

	void stop();

	void idle();

	void report();

	void set(const String& name, const String& value);
	void add(const String& name, const String& value);

	BitFlags flags;
};


EW_LEAVE

#endif
