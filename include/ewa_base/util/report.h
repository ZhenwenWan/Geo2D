#ifndef __UTIL_REPORT_H__
#define __UTIL_REPORT_H__


#include "ewa_base/domdata/dobject.h"
#include "ewa_base/domdata/symm.h"
#include "ewa_base/xml/xml_parser.h"

EW_ENTER


class ReportGenerator : public NonCopyable
{
public:

	bool handle(const String& buffer,StringBuffer& output, Variant param=Variant());

	bool handle_template(const String& name,StringBuffer& output, Variant param=Variant());

	static bool normal_template_filename(String& result,const String& name);
};

EW_LEAVE

#endif
