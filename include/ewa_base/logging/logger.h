#ifndef __H_EW_LOG_LOGGING__
#define __H_EW_LOG_LOGGING__

#include "ewa_base/collection/indexer_set.h"
#include "ewa_base/logging/logrecord.h"
#include "ewa_base/basic/object.h"
#include "ewa_base/basic/functor.h"

EW_ENTER

class DLLIMPEXP_EWA_BASE LogTarget;
class DLLIMPEXP_EWA_BASE LoggerImpl;



class DLLIMPEXP_EWA_BASE Logger : public Object
{
public:

	enum
	{
		MSG_NEVER,
		MSG_IF_ANY,
		MSG_IF_WARNING,
		MSG_IF_ERROR,
		MSG_MUTED=1<<4,
	};

	Logger(int src=0,int id=0);
	Logger(const String& src,int id=0);
	Logger(const Logger& o);
	Logger& operator=(const Logger& o);

	~Logger();

	void swap(Logger& logger);

	// set/get the default logtarget.
	static void def(LogTarget* p);
	static LogTarget* def();

	// set/get logtarget.
	void reset(LogTarget* p);
	LogTarget* get();

	// set/get source
	String Src(const String& src);
	int Src(int src);
	int Src();


	class SrcLocker
	{
	public:

		Logger& logger;
		int old_src;

		SrcLocker(const String& src_);
		SrcLocker(const String& src_,Logger& logger_);
		~SrcLocker();

	private:

		void _init(const String& src_);
	};

	// set/get id
	int Id();
	void Id(int id);

	void ErrLevel(int lv);

	int NumError();
	int NumWarning();

	// cache mode, LogRecords will not be send to logtarget immediately.
	bool Cache();
	void Cache(bool value);

	// cache mode, LogRecords will not be send to logtarget immediately.
	bool Detail();
	void Detail(bool value);

	// handle the logrecord, just send to the logtarget.
	void Handle(LogRecord& o);

	// flush cached logrecords to logtarget, return Ok() and Clear();
	bool Test(int t=MSG_IF_ANY,const String& title="");

	//return true if no error logrecords.
	bool Ok();

	//clear error
	void Clear();

	void Clear(arr_1t<LogRecord>& msgs);

	void DoLog(int v,const String&);

	arr_1t<LogRecord>& CachedRecords();

#ifdef EW_C11
	void DoLog(int v,String&&);
#endif


#ifndef EW_NDEBUG
	STRING_FORMATER_FORMAT_FUNCS_SB(void LogDebug,DoLog(LOGLEVEL_DEBUG,fb);)
#else
	inline void LogDebug(...) {}
#endif

	STRING_FORMATER_FORMAT_FUNCS_SB(void LogInfo,DoLog(LOGLEVEL_INFO,fb);)
	STRING_FORMATER_FORMAT_FUNCS_SB(void LogTrace,DoLog(LOGLEVEL_TRACE,fb);)
	STRING_FORMATER_FORMAT_FUNCS_SB(void Print,DoLog(LOGLEVEL_PRINT,fb);)
	STRING_FORMATER_FORMAT_FUNCS_SB(void PrintLn,DoLog(LOGLEVEL_PRINTLN,fb);)
	STRING_FORMATER_FORMAT_FUNCS_SB(void PrintTmp,DoLog(LOGLEVEL_PRINTTMP,fb);)
	STRING_FORMATER_FORMAT_FUNCS_SB(void LogMessage,DoLog(LOGLEVEL_MESSAGE,fb);)
	STRING_FORMATER_FORMAT_FUNCS_SB(void LogDetail,DoLog(LOGLEVEL_DETAIL,fb);)
	STRING_FORMATER_FORMAT_FUNCS_SB(void LogCommand,DoLog(LOGLEVEL_COMMAND,fb);)
	STRING_FORMATER_FORMAT_FUNCS_SB(void LogWarning,DoLog(LOGLEVEL_WARNING,fb);)
	STRING_FORMATER_FORMAT_FUNCS_SB(void LogError,DoLog(LOGLEVEL_ERROR,fb);)
	STRING_FORMATER_FORMAT_FUNCS_SB(void LogLiteError, DoLog(LOGLEVEL_LITE_ERROR, fb);)
	STRING_FORMATER_FORMAT_FUNCS_SB(void LogFatal,DoLog(LOGLEVEL_FATAL,fb);)

	DECLARE_OBJECT_INFO(Logger, ObjectInfoInvokable<Logger>)


	bool operator==(const Logger& o) const
	{
		return impl == o.impl;
	}

protected:

	LoggerImpl* impl;
};


template<> class hash_t<Logger> : public hash_pod<Logger> {};

DEFINE_OBJECT_NAME(Logger, "logger");

// get thread private logger;
DLLIMPEXP_EWA_BASE Logger& this_logger();



// LoggerSwap, swap logger with this_logger().
class DLLIMPEXP_EWA_BASE LoggerSwap : public Logger, private NonCopyable
{
public:
	LoggerSwap();
	LoggerSwap(LogTarget* p);
	LoggerSwap(Logger& logger);
	~LoggerSwap();
};

// LoggerSwap, swap logger with this_logger().
class DLLIMPEXP_EWA_BASE LoggerAuto : public Logger, private NonCopyable
{
public:
	LoggerAuto(int t = MSG_NEVER, const String& title = "");
	~LoggerAuto();
	Logger logger_old;

	int test_type;
	String test_title;

	static Functor<int(arr_1t<LogRecord>&,int,const String&)> test_func;
};


class DLLIMPEXP_EWA_BASE LogSource
{
public:

	LogSource();

	size_t size();
	int get(const String& s);

	String operator[](size_t i);

	static LogSource& current();

protected:
	indexer_set<String> m_aSources;
	AtomicSpin spin;
};


class LoggerEx : public Logger
{
public:
	typedef Logger basetype;

	LoggerEx(int flag=0);
	LoggerEx(Logger& logger_,int flag=0);
	~LoggerEx();

	bool has_new_error();
	bool has_new_error_only();

	void Handle(LogRecord& o);

protected:

	int flags;
	int err_count;
	int wrn_count;
};


template<unsigned N>
class lkt_loglevel_state
{
public:
	static const bool value = N != LOGLEVEL_TRACE && N != LOGLEVEL_INFO;
};

EW_LEAVE

#endif
