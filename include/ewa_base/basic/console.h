#ifndef __H_EW_BASIC_CONSOLE__
#define __H_EW_BASIC_CONSOLE__

#include "ewa_base/config.h"
#include "ewa_base/basic/formater.h"
#include "ewa_base/basic/object.h"
#include "ewa_base/basic/pointer.h"

EW_ENTER

class TeeFileData : public ObjectData
{
public:
	virtual void Close() = 0;
};

class DLLIMPEXP_EWA_BASE Console : public Object
{
public:
	enum
	{
		COLOR_R=1<<2,
		COLOR_G=1<<1,
		COLOR_B=1<<0,
		COLOR_S=1<<3,
		COLOR_D=COLOR_R|COLOR_G|COLOR_B,
	};

	static void SetColor(int color);

	STRING_FORMATER_FORMAT_FUNCS_SB(static void Write,ColoredWrite(fb);)
	STRING_FORMATER_FORMAT_FUNCS_SB(static void WriteLine,ColoredWriteLine(fb);)

	static void ColoredWrite(const String& s,int color=-1);
	static void ColoredWriteLine(const String& s,int color=-1);


	static void ColoredWriteEx(const String& s,int color,int type);

	static String ReadLine();

	static void Pause();
	static void SetUtf8(bool flag);

	static void SetTeeFile(const String& file,bool append);

	static DataPtrT<TeeFileData> AddTeeFile(const String& file,bool append);

	DECLARE_OBJECT_INFO_NO_CREATE(Console, ObjectInfoInvokable<Console>)

};

EW_LEAVE
#endif
