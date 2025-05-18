
#ifndef __H_EW_LOG_TARGET__
#define __H_EW_LOG_TARGET__


#include "ewa_base/basic/pointer.h"
#include "ewa_base/basic/object.h"
#include "ewa_base/basic/object_ex.h"
#include "ewa_base/basic/bitflags.h"
#include "ewa_base/basic/file.h"
#include "ewa_base/collection/arr_1t.h"
#include "ewa_base/scripting/callable_data.h"


EW_ENTER

class DLLIMPEXP_EWA_BASE LogRecord;
class DLLIMPEXP_EWA_BASE CmdLineParser;

class DLLIMPEXP_EWA_BASE LogTarget : public CallableData
{
public:

	enum
	{
		FLAG_NEWLINE	=1<<0,
		FLAG_SHOWALL	=1<<1,
		FLAG_SHOWDETAIL	=1<<2,
		FLAG_CACHED		=1<<5,
		FLAG_COLORED	=1<<8,
		FLAG_MUTED		=1<<9,
	};

	virtual void Parse(CmdLineParser* pcmdline=NULL);

	void HandleEx(const LogRecord& o);

	LogTarget();

	// cast logrecord to string
	virtual String Format(const LogRecord& o) const;

	// flush if cached.
	virtual void Flush();

	BitFlags flags;
	String logfmt;

	static const char* ms_default_logfmt;

protected:

	// Handle the record
	virtual void Handle(const LogRecord& o)=0;
};

// LogNull, ignore all messages.
class DLLIMPEXP_EWA_BASE LogNull : public LogTarget
{
public:
	LogNull();
	virtual void Handle(const LogRecord&);

	DECLARE_OBJECT_INFO(LogNull, ObjectInfo);
};

// LogConsole, display messages in console window.
class DLLIMPEXP_EWA_BASE LogConsole : public LogTarget
{
public:
	LogConsole();

	virtual void Handle(const LogRecord&);
	virtual void Flush();

	DECLARE_OBJECT_INFO(LogConsole, ObjectInfo);

protected:
	arr_1t<int> aColors;
};

// LogFile, write messages to file.
// if not append mode, file will be truncated
class DLLIMPEXP_EWA_BASE LogFile : public LogTarget
{
public:

	LogFile();
	LogFile(const String& filename,bool app=true);
	~LogFile();

	virtual void Handle(const LogRecord&);
	bool good(){return fp.good();}

	bool open(const String& filename,bool app=true);
	void close();

	DECLARE_OBJECT_INFO(LogFile, ObjectInfo);

protected:
	File fp;
	AtomicSpin spin;
};


class DLLIMPEXP_EWA_BASE LogCache : public LogTarget
{
public:
	arr_1t<LogRecord> aRecords;
	AtomicMutex mutex;

	LogCache();

	void Serialize(SerializerHelper sh);
	virtual void Handle(const LogRecord& o);

	DECLARE_OBJECT_INFO(LogCache,ObjectInfo);
};


class DLLIMPEXP_EWA_BASE LogPtr : public ObjectT<LogTarget,LogTarget>
{
public:
	typedef ObjectT<LogTarget, LogTarget> basetype;
	LogPtr(LogTarget* p = NULL);

	void Handle(const LogRecord& o);
	void Flush();

	DECLARE_OBJECT_INFO(LogPtr, ObjectInfo);
};



class DLLIMPEXP_EWA_BASE LogGrp : public LogTarget
{
public:
	DataGrpT<LogTarget> aTargets;

	void Handle(const LogRecord& o);
	void Flush();

	DECLARE_OBJECT_INFO(LogGrp, ObjectInfo);
};

EW_LEAVE
#endif
