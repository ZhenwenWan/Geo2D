#ifndef __LOGGER_UTIL_H__
#define __LOGGER_UTIL_H__

#include "ewa_base/logging/logger.h"

EW_ENTER
// 用于复杂对象检测时的局部Logger状态扩展
// 可以记住局部对象的Check结果
class DLLIMPEXP_EWA_BASE LoggerLocal : public Logger
{
public:

	typedef Logger basetype;

	LoggerLocal(Logger& _logger = this_logger(), int _crit_level = LOGLEVEL_LITE_ERROR) : basetype(_logger), ok(true), crit_level(_crit_level) {}

	Logger& operator() () 
	{
		return *this;
	}

	bool Ok() const
	{
		return ok;
	}

	void Handle(LogRecord& o)
	{
		if (o.m_nLevel >= crit_level)
		{
			ok = false;
		}
		
		basetype::Handle(o);
	}

	//void DoLog(int v, const String& msg)
	//{
	//	if (v >= crit_level)
	//	{
	//		ok = false;
	//	}
	//	
	//	logger.DoLog(v, msg);
	//}

	void Assert(bool flag, const String& msg, int v = LOGLEVEL_ERROR)
	{
		if (flag)
		{
			return;
		}

		return DoLog(v, msg);
	}

	//STRING_FORMATER_FORMAT_FUNCS_SB(void LogInfo, DoLog(LOGLEVEL_INFO, fb);)
	//STRING_FORMATER_FORMAT_FUNCS_SB(void LogTrace, DoLog(LOGLEVEL_TRACE, fb);)
	//STRING_FORMATER_FORMAT_FUNCS_SB(void Print, DoLog(LOGLEVEL_PRINT, fb);)
	//STRING_FORMATER_FORMAT_FUNCS_SB(void PrintLn, DoLog(LOGLEVEL_PRINTLN, fb);)
	//STRING_FORMATER_FORMAT_FUNCS_SB(void PrintTmp, DoLog(LOGLEVEL_PRINTTMP, fb);)
	//STRING_FORMATER_FORMAT_FUNCS_SB(void LogMessage, DoLog(LOGLEVEL_MESSAGE, fb);)
	//STRING_FORMATER_FORMAT_FUNCS_SB(void LogDetail, DoLog(LOGLEVEL_DETAIL, fb);)
	//STRING_FORMATER_FORMAT_FUNCS_SB(void LogCommand, DoLog(LOGLEVEL_COMMAND, fb);)
	//STRING_FORMATER_FORMAT_FUNCS_SB(void LogWarning, DoLog(LOGLEVEL_WARNING, fb);)
	//STRING_FORMATER_FORMAT_FUNCS_SB(void LogError, DoLog(LOGLEVEL_ERROR, fb);)
	//STRING_FORMATER_FORMAT_FUNCS_SB(void LogLiteError, DoLog(LOGLEVEL_LITE_ERROR, fb);)
	//STRING_FORMATER_FORMAT_FUNCS_SB(void LogFatal, DoLog(LOGLEVEL_FATAL, fb);)
private:
	//Logger& logger;
	bool ok; // 初始状态true
	int crit_level;
};

EW_LEAVE
#endif