#ifndef __UTIL_HELPGEN_H__
#define __UTIL_HELPGEN_H__


#include "ewa_base/domdata/symm.h"
#include "ewa_base/util/regex.h"
#include "ewa_base/xml/xml_document.h"

EW_ENTER



class DLLIMPEXP_EWA_BASE FileDesc
{
public:
	String filename;
	String name;
	String type;
};

class DLLIMPEXP_EWA_BASE XmlParserMD;

class DLLIMPEXP_EWA_BASE HelpTopic : public ObjectData
{
public:
	HelpTopic();

	String name;
	String title;
	String body;
	int64_t flags;
	int64_t level;

	DataGrpT<HelpTopic> children;
	arr_1t<FileDesc> content;
	String filepath;
	TopicItem headers;
	TopicItem subtopics;

	VariantTable config;

	XmlDocument xmldoc;


	void update(XmlParserMD& parser);

};


class DLLIMPEXP_EWA_BASE HelpContextHtml : public Object
{
public:
	HelpContextHtml();


	bool generate(const String& path_src,const String& path_dst,Variant cfg=Variant());

protected:

	DataPtrT<ObjectData> m_refData;
};

EW_LEAVE

#endif
