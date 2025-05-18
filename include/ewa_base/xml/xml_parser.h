#ifndef __H_EW_XML_PARSER__
#define __H_EW_XML_PARSER__

#include "ewa_base/xml/xml_attribute.h"
#include "ewa_base/xml/xml_node.h"
#include "ewa_base/xml/xml_document.h"
#include "ewa_base/basic/bitflags.h"
#include "ewa_base/basic/stringbuffer.h"
#include "ewa_base/basic/scanner_helper.h"
#include "ewa_base/scripting/callable_table.h"
#include "ewa_base/scripting/callable_array.h"


EW_ENTER

template<unsigned N>
class lkt_name
{
public:
	static const int value = (N != '\0' && N != '=' && N != '!' && N != '>' && N != '<' && N != '?' && N != '/' && N != '\\') && (!lkt_whitespace<N>::value);
};


template<typename T>
class NodeLockerT
{
public:
	T& ctx;
	XmlNode* node;

	NodeLockerT(T& parser_) :ctx(parser_)
	{
		node = ctx.CreateNode(XmlNode::XMLNODE_DATA);
		ctx.put_node(node);
	}

	NodeLockerT(T& parser_, const String& p) :ctx(parser_)
	{
		node = ctx.CreateNode(XmlNode::XMLNODE_ELEMENT, p);
		ctx.put_node(node);
	}
	NodeLockerT(T& parser_, XmlNode* pnode) :ctx(parser_)
	{
		node = pnode;
		ctx.put_node(node);
	}
	~NodeLockerT()
	{
		ctx.pop_node();
	}

	void add_attr(const String& name, const String& value)
	{
		node->AppendAttribute(new XmlAttribute(name, value));
	}
};

class XmlParser : public Object
{
public:

	typedef NodeLockerT<XmlParser> NodeLocker;

	typedef ScannerHelper<const char*> helper;
	typedef helper::type mychar;
	typedef helper::pointer mychar_ptr;

	enum
	{
		FLAG_KEEP_COMMENT = 1 << 0,
		FLAG_KEEP_PI = 1 << 0,
	};

	XmlParser();

	virtual bool parse_document(XmlNode& xmldoc,XmlParserConfig& xmlcfg);

	bool parse_document(XmlDocument& xmldoc,XmlParserConfig& xmlcfg);


	void kerror(const String& s, mychar_ptr p1, mychar_ptr p2);

	void kerror(const String& s);

	void kexpected(const String& msg);

	void kexpected_at_line(const String& msg,const String& line);

	inline void put_node(XmlNode* pnode)
	{
		nodes.back()->AppendChild(pnode);
		nodes.push_back(pnode);
	}

	inline void put_node(const String& tag)
	{
		XmlNode* pnode = new XmlNode(tag);
		nodes.back()->AppendChild(pnode);
		nodes.push_back(pnode);
	}

	inline void pop_node()
	{
		nodes.pop_back();
	}

	static XmlNode* CreateNode(int t = XmlNode::XMLNODE_ELEMENT)
	{
		return new XmlNode(t);
	}

	static XmlNode* CreateNode(int t, const String& tag)
	{
		XmlNode* p = CreateNode(t);
		p->SetName(tag);
		return p;
	}

	static XmlNode* CreateNode(int t, const String& tag, const String& value)
	{
		XmlNode* p = CreateNode(t);
		p->SetName(tag);
		p->SetValue(value);
		return p;
	}

	static XmlNode* CreateNode(int t, const String& tag, XmlNode* pchild)
	{
		XmlNode* p = CreateNode(t);
		p->SetName(tag);
		p->AppendChild(pchild);
		return p;
	}

	static XmlAttribute* CreateAttr()
	{
		return new XmlAttribute();
	}

	void AdjustFile(String& file);


	BitFlags flags;
	arr_1t<XmlNode*> nodes;
	LitePtrT<VariantTable> ptable;
	String vfs_path;

protected:

	int m_nDocumentType;

	void handle_exception(std::exception& e);

	template<size_t N>
	inline void skip_tag(mychar_ptr& pcur,const char (&tag)[N])
	{
		if(N<2)
		{
			return;
		}

		lookup_table<lkt2uppercase> lku;
		for(size_t i=0; i<N-1; i++)
		{
			if(lku(pcur[i])!=(mychar)tag[i])
			{
				kexpected(tag);
				return;
			}
		}
		pcur+=N-1;
	}

	StringBufferT<mychar> tempbuf; //string_assign buffer

	inline void add_data(const String& v)
	{
		XmlNode* p = new XmlNode(XmlNode::XMLNODE_DATA);
		p->SetValue(v);
		nodes.back()->AppendChild(p);
	}

	inline void add_node(const String& t,const String& v)
	{
		nodes.back()->AppendChild(new XmlNode(t, v));
	}

	void add_code(const String& v, const String& lang="language-markdown")
	{
		XmlNode* pnode = new XmlNode("code", v);
		pnode->AppendAttribute(new XmlAttribute("class", lang));
		nodes.back()->AppendChild(pnode);
	}

	inline void add_node(XmlNode* pnode)
	{
		nodes.back()->AppendChild(pnode);
	}

	inline void add_attr(const String& name, const String& value)
	{
		nodes.back()->AppendAttribute(new XmlAttribute(name, value));
	}

	mychar_ptr pcur; //current position
	mychar_ptr pbeg;
	mychar_ptr pend;



	//<tag attributes> content </tag>
	void parse_element_node();

	//A document node is a specialized kind of element node.
	//It has a type p but no attributes. Instead it has an optional URL u.
	//The intent of the URL is to specify a specialized data model for this node and its children.
	//A document node looks like this:
	//<!doctype p "u">c1 . . . cm for m>0
	void parse_doctype_node();

	//A processing instruction (PI) node is always a leaf node.
	//It only has an instruction i associated with it.
	//The instruction is a sequence of zero or more characters, without any restrictions,
	//except that the sequence may not start with the three characters `xml' (upper, lower or mixed case) followed by a space or newline.
	//It looks like this in the XML document:
	//<?i?>
	void parse_instruction_node();


	//<!-- This is a comment -->
	void parse_comment_node();

	virtual void parse_subnodes();

	virtual void parse_subnodes2();

	void parse_data();
	void parse_cdata();

	void parse_attributes();

	void parse_value();
	void parse_string1();
	void parse_string2();


	void string_assign(String& s0,mychar_ptr p1,mychar_ptr p2);


};



class XmlParserMD : public XmlParser
{
public:

	typedef XmlParser basetype;

	XmlParserMD();

	bool parse_document(XmlNode& xmldoc, XmlParserConfig& xmlcfg);

	void generate_empty_page(XmlNode& node, TopicItem& item);

	virtual void parse_subnodes();

	void parse_block();
	void parse_markdown();

	void parse_node_rawtext(mychar_ptr tagvalue1);
	void parse_node_script(mychar_ptr tagvalue1);

	void handle_markdown_ex(const String& s);
	void handle_markdown(const String& s);
	void handle_markdown(const arr_1t<String>& s);
	void handle_markdown_temp();

	static String make_link(const String& pre, const String& anchor);
	XmlNode* handle_markdown_lines(arr_1t<String>& lines);

	int make_tag_level(int v);

	int n_level;

	class LineItem
	{
	public:
		LineItem(const String& s = "");
		char update_tag();
		String line;
		unsigned tag_count;
		char tag;
	};


	typedef arr_1t<LineItem>::iterator string_line_iterator;

	void handle_markdown_lines_part(string_line_iterator& it1, string_line_iterator& it2);
	void handle_markdown_lines_item(string_line_iterator& it1, string_line_iterator& it2);

	void handle_markdown_line(const String& line);

	void handle_table_tr(const String& line,bool is_header=false);

	void markdown_format(const String& line);
	void markdown_format_line(const String& line);

	void markdown_add_anchor_link(String& anchor, const String& title);

	String make_anchor(const String& anchor);

	String markdown_inline_format(const String& s);

	void markdown_read_params(arr_1t<String>& vv, mychar_ptr& p);


	bool is_node_tag();
	bool is_line_beg();

	arr_1t<LineItem> temp;
	int text_encode_type;

	void update_flags();

	void handle_definition(const arr_1t<String>& vv);
	void handle_definition_image(const arr_1t<String>& vv);
	void handle_definition_image_inline(const arr_1t<String>& vv);

	void handle_definition_graph(const arr_1t<String>& vv);
	void handle_definition_anchor(const arr_1t<String>& vv);
	void handle_definition_anchor(const String& name,const String& text=empty_string);

	void handle_definition_link(const arr_1t<String>& vv);
	void handle_definition_term(const arr_1t<String>& vv);
	void handle_definition_block(const arr_1t<String>& vv);
	void handle_definition_topic(const arr_1t<String>& vv);

	void handle_topic_item(TopicItem& item);

	using XmlParser::add_node;
	void add_node(const arr_1t<String>& vv);


	bst_set<XmlNode*> a_ref_pending;
	int n_ref_handled;

	int n_table;
	int n_image;
	String s_label_table;
	String s_label_image;
	String s_name;


	String add_header(String& val, int lv);
	void add_keyword(const String& keyword);
	void add_navigation(const String& val);

	void set_prefix(const String& image, const String& table = "");

	TopicItem headers;
	TopicItem subtopics;
	
	bst_set<XmlNode*> pending_ref;


	class XCellInfo
	{
	public:

		XCellInfo();

		void handle(char ch);

		int align_type;
		int font_type;
		int font_style;
		int border_type;
		int width;

		void apply(bst_set<String>& cls, int type);

	private:

		void _handle_style(char ch);

	};


	class TableInfo
	{
	public:
		arr_1t<XCellInfo> columns;
		arr_1t<XCellInfo> rows;

		String style;
		size_t header_rows;
		size_t current_row;
		size_t column_size;
		arr_1t<arr_1t<int> > cellspan;

		void handle(const String& ss);
		void clear();

	private:
		void _handle_flags(arr_1t<XCellInfo>& arr, const char* p);
	};

	TableInfo table_info;

	void update_document(XmlNode& xmldoc, XmlParserConfig& xmlcfg);

protected:
	void _do_update_node(XmlNode* node, XmlParserConfig& xmlcfg);
	void _do_update_node_custom(XmlNode* node, XmlParserConfig& xmlcfg);

	bool _do_markdown_update(XmlNode* node, int flag, XmlParserConfig& xmlcfg);
	bool _do_markdown_update_children(XmlNode* node, int flag, XmlParserConfig& xmlcfg);
	bool _do_markdown_update_slider(XmlNode* node, int flag, XmlParserConfig& xmlcfg);
	bool _do_markdown_update_group(XmlNode* node, int flag, XmlParserConfig& xmlcfg);

	String filename;

};




class XmlParserWriter : public Object
{
public:

	XmlParserWriter(XmlDocument& xmldoc_);

	XmlDocument& xmldoc;

	bool save_document(StringBuffer& sb);

protected:

	void savestring(const String& v);
	void savenode(XmlNode* pnode, int lv = 0);
	void tabindent(int lv);

	StringBuffer savebuf;
};

EW_LEAVE
#endif
