#ifndef __H_EW_SCRIPTING_PARSER__
#define __H_EW_SCRIPTING_PARSER__


#include "ewa_base/scripting/scanner.h"
#include "ewa_base/scripting/parser_nodes.h"
#include "ewa_base/scripting/stackstate.h"

EW_ENTER


class DLLIMPEXP_EWA_BASE TNode_statement_list;
class DLLIMPEXP_EWA_BASE tokItem;

template<typename T>
class read_node_handler;

template<int N>
class read_expr_handler;

class DLLIMPEXP_EWA_BASE Parser : public CGenState
{
public:

	Parser();
	Parser(CGenState& cgs);

	DataPtrT<TNode_val_function> parse(const String& p);
	DataPtrT<TNode_val_function> parse(const char* p1,const char* p2);
	DataPtrT<TNode_val_function> parse(tokItem* t);

	String get_comment(int line);



	indexer_map<String,int> op_table;

protected:

	void read_expr_with_init(DataPtrT<TNode_statement>& stmt_init, DataPtrT<TNode_expression>& expr_cond);

	template<typename T>
	friend class read_node_handler;

	template<int N>
	friend class read_expr_handler;

	void match(tokType type);
	void match(tokType type,const String& s);
	void match_key(const String& s);

	bool test(tokType type);
	bool test(tokType type,const String& s);
	bool test_key(const String& s);

	void kerror(const String& s);
	void kexpected(const String& s);

	tokItem* pcur;
	tokItem* pbeg;

	BitFlags flags;

	Scanner scanner;
};

EW_LEAVE

#endif


