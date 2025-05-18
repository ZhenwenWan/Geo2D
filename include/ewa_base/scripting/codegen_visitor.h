#ifndef __H_EW_SCRIPTING_CODEGEN_VISITOR__
#define __H_EW_SCRIPTING_CODEGEN_VISITOR__

#include "ewa_base/scripting/parser_visitor.h"
#include "ewa_base/scripting/codegen.h"
#include "ewa_base/scripting/callable_code.h"
#include "ewa_base/scripting/callable_table.h"
#include "ewa_base/scripting/executor.h"

EW_ENTER


class DLLIMPEXP_EWA_BASE TNodeVisitorCG : public TNodeVisitorRecursive
{
public:
	typedef TNodeVisitorRecursive basetype;

	TNodeVisitorCG(CodeGen& g);

	virtual void kerror(const String& s);

	virtual void parse(TNode* q);

	virtual void visit_enter(TNode* node);

	virtual void visit_leave(TNode*);

	using basetype::visit;

	virtual VisReturnType visit(TNode_statement_macro*,VisExtraParam);

	CodeGen& cgen;
	DataPtrT<VisExtraHandler> cg_extra;
};




EW_LEAVE
#endif
