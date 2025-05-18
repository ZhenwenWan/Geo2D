#ifndef __H_EW_XML_DOCUMENT__
#define __H_EW_XML_DOCUMENT__

#include "ewa_base/xml/xml_node.h"
#include "ewa_base/collection/arr_1t.h"
#include "ewa_base/scripting/variant.h"

EW_ENTER


class DLLIMPEXP_EWA_BASE XmlParserConfig
{
public:
	enum
	{
		TYPE_XML,
		TYPE_HTML,
		TYPE_MARKDOWN,
		TYPE_MARKDOWN_X,
	};

	XmlParserConfig(int t);

	bool LoadFile(const String& s);
	void SetString(const char* p);
	void SetString(const String& s);

	int type;
	const char* pstr;
	unsigned size;

	Variant config;
	Variant rparam;
	String filename;

	LitePtrT<VariantTable> ptable;
	arr_1t<XmlNode*> nodes;

	void SetParam(Variant v);

	String vfs_path;

	BitFlags flags;

	enum
	{
		FLAG_NEED_MATHJAX	=1<<0,
		FLAG_NO_INJECTION	=1<<1,
	};

protected:
	StringBuffer buffer;

};

class TopicItem
{
public:
	String name;
	String anchor;
	arr_1t<TopicItem> items;
};


class DLLIMPEXP_EWA_BASE XmlDocument : public XmlNode
{
public:
	friend class XmlParser;


	XmlDocument();

	// deep copy
	XmlDocument(const XmlDocument& o);
	const XmlDocument& operator=(const XmlDocument& o);

	~XmlDocument();

	bool SaveXml(StringBuffer& sb);
	bool SaveXml(const String& s);
	bool LoadStr(const char* p);
	bool LoadStr(const String& s);

	bool LoadXml(const String& s);
	bool LoadMarkdown(const String& s);

	bool LoadDocument(XmlParserConfig& cfg);

	void SetDocumentType(int t);
	int GetDocumentType() const;

	void Lift2Html(const String& title=empty_string);

protected:
	int m_nDocumentType;
};


EW_LEAVE
#endif
