#ifndef PARSERGENERIC_H
#define PARSERGENERIC_H

#include "ParserPow.h"
#include "ParserItem.h"
#include "ParserException.h"
#include "ListParserContextExec.h"
#include "VariantParam.h"
#include "ParserValue.h"
#include "ParserAction.h"

class ParserGeneric
{
public:

	typedef struct
	{	int Token;
		QString TokenStr;
		QString ExtraValue;
		QString MibFileName;
		int MibFileLine;
	} LexToken_t;

	ParserGeneric(ParserAction & Callback);
	~ParserGeneric(void);

	void RuleBegin(const char * RuleName) throw (ParserException);
	void RuleEnd(const char * RuleName) throw (ParserException);

	void Execute(const char * rule_name) throw (ParserException);

	// to ask the next token to the lexer
	virtual bool NextToken(LexToken_t & token) = 0;

protected:
	// when parser is loaded, you MUST call that function to update some values
	void Finalize(void);

	// to get value read by parser
	ParserValue & Value(void) { return m_value; }

	void Call(ParserPow & pow);

	void Greedy(const ParserPow & pow, int token);

	// 1 item
	void Cardinal_1(const ParserPow & pow,
						 VariantParam param0,
						 VariantParam param1 = VariantParam(),
						 VariantParam param2 = VariantParam(),
						 VariantParam param3 = VariantParam(),
						 VariantParam param4 = VariantParam(),
						 VariantParam param5 = VariantParam());

	// 0 or 1 item
	void Cardinal_0_1(const ParserPow & pow,
							VariantParam param0,
							VariantParam param1 = VariantParam(),
							VariantParam param2 = VariantParam(),
							VariantParam param3 = VariantParam(),
							VariantParam param4 = VariantParam(),
							VariantParam param5 = VariantParam());

	// 0 or n items
	void Cardinal_0_n(const ParserPow & pow,
							VariantParam param0,
							VariantParam param1 = VariantParam(),
							VariantParam param2 = VariantParam(),
							VariantParam param3 = VariantParam(),
							VariantParam param4 = VariantParam(),
							VariantParam param5 = VariantParam());

	// 1 or n items
	void Cardinal_1_n(const ParserPow & pow,
							VariantParam param0,
							VariantParam param1 = VariantParam(),
							VariantParam param2 = VariantParam(),
							VariantParam param3 = VariantParam(),
							VariantParam param4 = VariantParam(),
							VariantParam param5 = VariantParam());

	void Branch(const ParserPow & pow,
					VariantParam branch_0,
					VariantParam branch_1,
					VariantParam branch_2 = VariantParam(),
					VariantParam branch_3 = VariantParam(),
					VariantParam branch_4 = VariantParam(),
					VariantParam branch_5 = VariantParam(),
					VariantParam branch_6 = VariantParam(),
					VariantParam branch_7 = VariantParam(),
					VariantParam branch_8 = VariantParam(),
					VariantParam branch_9 = VariantParam(),
					VariantParam branch_10 = VariantParam(),
					VariantParam branch_11 = VariantParam(),
					VariantParam branch_12 = VariantParam(),
					VariantParam branch_13 = VariantParam(),
					VariantParam branch_14 = VariantParam(),
					VariantParam branch_15 = VariantParam(),
					VariantParam branch_16 = VariantParam());

	void Dump(void) const;
	void Check(void);

	ParserItem * AddToken(const char * RuleName, int token) throw (ParserException);
	ParserItem * AddGreedy(const char * RuleName, int token) throw (ParserException);
	ParserItem * AddJump(const char * RuleName) throw (ParserException);
	ParserItem * AddPoint(const char * RuleName) throw (ParserException);
	ParserItem * AddBegin(const char * RuleName) throw (ParserException);
	ParserItem * AddEnd(const char * RuleName) throw (ParserException);
	ParserItem * AddSub(const char * RuleName, const char * called) throw (ParserException);
	ParserItem * AddCallback(const char * RuleName, int Order) throw (ParserException);

	ParserItem * FindSymbolByName(const char * symbol) const throw (ParserException);

private:
	ParserItem * _execute(const ParserPow &pow, VariantParam & param);
	void _branch(const ParserPow & pow, ParserItem * & start, ParserItem * & end, VariantParam & param);
	ParserItem * _add(ParserItem * item) throw (ParserException);

	ParserItemList m_parser;
	ParserItemMap m_symbol;

	bool _execute_token_type(ListParserContextExec & context, const LexToken_t & token);
	void _execute_greedy_type(ListParserContextExec & context, LexToken_t & token);
	void _execute_jump_type(ListParserContextExec & context);
	void _execute_point_type(ListParserContextExec & context);
	void _execute_begin_type(ListParserContextExec & context);
	void _execute_end_type(ListParserContextExec & context);
	void _execute_sub_type(ListParserContextExec & context);
	void _execute_callback_type(ListParserContextExec & context);

	ParserValue m_value;
	ParserAction * m_callback;

};

#endif // PARSERGENERIC_H
