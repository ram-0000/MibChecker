#include "ParserGeneric.h"
#include "Debug.h"
#include <QVector>

ParserGeneric::ParserGeneric(ParserAction & Callback)
{
	m_list_symbol.clear();
	m_parser.clear();
	m_callback = &Callback;
}

ParserGeneric::~ParserGeneric(void)
{
	m_list_symbol.clear();
	qDeleteAll(m_parser);
	m_parser.clear();
}

void ParserGeneric::Finalize(void)
{
	int idx = 0;
	for(ParserItemList::iterator it_current = m_parser.begin();
		 it_current != m_parser.end();
		 ++it_current)
	{
		ParserItem * current = *it_current;
		ParserItemList::iterator it_next = it_current + 1;

		ParserItem * next = nullptr;
		if(it_next != m_parser.end())
			next = *it_next;

		// update id values and next pointers
		current->Finalize(idx++, next);
	}
}

void ParserGeneric::Dump(void) const
{
	DEBUG("Dump of parser table");
	for(ParserItemList::const_iterator it = m_parser.begin(); it != m_parser.end(); ++it)
		(*it)->Dump();

	DEBUG("Dump of symbol table");
	for(ParserItemMap::const_iterator it = m_list_symbol.begin(); it != m_list_symbol.end(); ++it)
	{
		DEBUG("%s", it.key().toLatin1().constData());
	}
}

bool ParserGeneric::IsCalled(const char * symbol) const
{
	ParserException::CheckNullOrEmpty(symbol);

	// check symbol table
	for(const auto & item : m_parser)
	{
		// check if item is a sub-caller
		if(item->isSub() == false)
			continue;

		// get name of called sub routine
		const char * called = item->Sub();

		// compare
		if(strcmp(symbol, called) == 0)
			return true;
	}
	return false;
}

ParserItem * ParserGeneric::FindSymbolByName(const char * symbol, bool throw_exception /*= true*/) const
{
	ParserException::CheckNullOrEmpty(symbol);

	// try to find the symbol
	ParserItemMap::const_iterator it = m_list_symbol.find(symbol);

	// found ?
	if(it == m_list_symbol.end())
	{
		if(throw_exception == true)
			throw ParserExceptionSymbolNotFound(symbol);
		else
			return nullptr;
	}

	// return it
	return it.value();
}

void ParserGeneric::RuleBegin(const char * RuleName)
{
	ParserException::CheckNullOrEmpty(RuleName);

	AddBegin(RuleName);
}

void ParserGeneric::RuleEnd(const char * RuleName)
{
	ParserException::CheckNullOrEmpty(RuleName);

	AddEnd(RuleName);
}

ParserItem * ParserGeneric::AddToken(const char * RuleName,
												 int token)
{
	ParserException::CheckNullOrEmpty(RuleName);

	// create item
	ParserItem * item = new ParserItem(RuleName, ParserItem::TypeToken);
	_add(item);
	item->Token(token);
	return item;
}

ParserItem * ParserGeneric::AddGreedy(const char * RuleName,
												 int token)
{
	ParserException::CheckNullOrEmpty(RuleName);

	// create item
	ParserItem * item = new ParserItem(RuleName, ParserItem::TypeGreedy);
	_add(item);
	item->Token(token);
	return item;
}

ParserItem * ParserGeneric::AddJump(const char * RuleName)
{
	ParserException::CheckNullOrEmpty(RuleName);

	// create item
	ParserItem * item = new ParserItem(RuleName, ParserItem::TypeJump);
	return _add(item);
}

ParserItem * ParserGeneric::AddPoint(const char * RuleName)
{
	ParserException::CheckNullOrEmpty(RuleName);

	// create item
	ParserItem * item = new ParserItem(RuleName, ParserItem::TypePoint);
	return _add(item);
}

ParserItem * ParserGeneric::AddBegin(const char * RuleName)
{
	ParserException::CheckNullOrEmpty(RuleName);

	// check if the item is already existing in symbol table
	if(m_list_symbol.find(RuleName) != m_list_symbol.end())
		throw ParserExceptionSymbolAlreadyExist(RuleName);

	// create item
	ParserItem * item = new ParserItem(RuleName, ParserItem::TypeBegin);
	_add(item);

	// insert item into symbol list
	m_list_symbol.insert(RuleName, item);

	return item;
}

ParserItem * ParserGeneric::AddEnd(const char * RuleName)
{
	ParserException::CheckNullOrEmpty(RuleName);

	// create item
	ParserItem * item = new ParserItem(RuleName, ParserItem::TypeEnd);
	return _add(item);
}

ParserItem * ParserGeneric::AddSub(const char * RuleName,
													const char * called)
{
	ParserException::CheckNullOrEmpty(RuleName);
	ParserException::CheckNullOrEmpty(called);

	// create item
	ParserItem * item = new ParserItem(RuleName, ParserItem::TypeSub);
	_add(item);
	item->Sub(called);
	return item;
}

ParserItem * ParserGeneric::AddCallback(const char * RuleName,
													int Order)
{
	ParserException::CheckNullOrEmpty(RuleName);

	// create item
	ParserItem * item = new ParserItem(RuleName, ParserItem::TypeCallback);
	_add(item);

	item->Callback(m_callback, Order);
	return item;
}

ParserItem * ParserGeneric::_add(ParserItem * item)
{
	ParserException::CheckNull(item);

	// add item to list
	m_parser.push_back(item);
	return item;
}

void ParserGeneric::Execute(void)
{
	// search the program entry point
	ParserItem * item = FindSymbolByName(ParserItem::Root());
	ParserException::CheckNull(item);

	// get next item following current item
	item = item->Next();
	ParserException::CheckNull(item);

	// create context
	ListParserContextExec context(item);

	// reset value
	m_value.clear();

	// get a token
	LexToken_t token;
	NextToken(token);

	while(context.Current().CallStackSize() != 0)
	{
		// dump current pc
		//context.Current().PcGet()->Dump();

		if(context.Current().PcGet()->isToken() == true)
		{
			if(_execute_token_type(context, token) == true)
				NextToken(token);
		}
		else if(context.Current().PcGet()->isGreedy() == true)
			_execute_greedy_type(context, token);
		else if(context.Current().PcGet()->isJump() == true)
			_execute_jump_type(context);
		else if(context.Current().PcGet()->isPoint() == true)
			_execute_point_type(context);
		else if(context.Current().PcGet()->isBegin() == true)
			_execute_begin_type(context);
		else if(context.Current().PcGet()->isEnd() == true)
			_execute_end_type(context);
		else if(context.Current().PcGet()->isSub() == true)
			_execute_sub_type(context);
		else if(context.Current().PcGet()->isCallback() == true)
			_execute_callback_type(context);
		else
			throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);
	}
}

bool ParserGeneric::_execute_token_type(ListParserContextExec & context, const LexToken_t & token)
{
	// check if right token
	if(context.Current().PcGet()->Token() == token.Token)
	{
		// check if a solution is already found, in that case, this is not a LL1 grammar
		if(context.IsFound() == true)
		{
			throw ParserExceptionNotLL1(token.MibFileName,
												 token.MibFileLine,
												 token.TokenStr,
												 context.Found().PcGet()->Rule(),
												 context.Found().CallStackStr(),
												 context.Current().PcGet()->Rule(),
												 context.Current().CallStackStr());
		}

		/*DEBUG("DEBUG Parsing: File %s(%d), Token '%s' Match rule '%s'",
				token.MibFileName.toLatin1().constData(),
				token.MibFileLine,
				token.TokenStr.toLatin1().constData(),
				context.Current().PcGet()->Rule());*/

		// advance the PC
		context.Current().PcNext();

		// record found context
		context.CurrentIntoFound();
	}

	// remove that context
	context.CurrentRemove();

	// check if other context are available
	if(context.size() != 0)
		return false;

	// all tries are exhausted
	if(context.IsFound() == false)
	{
		// no valid branch found, it is a syntax error
		throw ParserExceptionSyntaxError(token.MibFileName, token.MibFileLine);
	}

	// one branch (and only one) has been found valid
	// execute any Callback delayed
	ParserItemList & callbacks = context.Found().getCallbackDelayed();
	QVector<ParserValue> & values = context.Found().getValueDelayed();
	int idx_value = 0;
	for(ParserItemList::iterator it = callbacks.begin();
		 it != callbacks.end();
		 ++it)
	{
		ParserItem * i = *it;
		ParserException::CheckNull(i);
		if(i->isCallback() == false)
			throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);

		// get function pointer
		int order;
		ParserAction * callback = i->Callback(order);
		ParserException::CheckNull(callback);
		const char * rule = i->Rule();
		ParserException::CheckNullOrEmpty(rule);

		// call the function
		callback->OnAction(token.MibFileLine, rule, order, values[idx_value]);
		idx_value++;
	}
	callbacks.clear();
	values.clear();

	// set new context
	context.FoundIntoCurrent();
	context.FoundReset();

	// add value
	if(token.ExtraValue.length() != 0)
		m_value.set(token.ExtraValue);
	else
		m_value.set(token.TokenStr);

	// read next token
	return true;
}

void ParserGeneric::_execute_greedy_type(ListParserContextExec & context, LexToken_t & token)
{
	int int_token = context.Current().PcGet()->Token();

	// read until end of file or until token
	for(;;)
	{
		// read a token
		bool ret = NextToken(token);

		// check for end of file
		if(ret == false)
			break;

		// check for waited token
		if(token.Token == int_token)
			break;
	}

	// advance PC
	context.Current().PcNext();
}

void ParserGeneric::_execute_jump_type(ListParserContextExec & context)
{
	// create multiple context for each jump
	const ParserItemList & j = context.Current().PcGet()->Jump();
	for(ParserItemList::const_iterator it = j.begin(); it != j.end(); ++it)
	{
		if(it == j.begin())
		{
			// set new PC on current context
			context.Current().PcSet(*it);
		}
		else
		{
			// duplicate current context and set PC
			context.CurrentDuplicate(*it);
		}
	}
}

void ParserGeneric::_execute_point_type(ListParserContextExec & context)
{
	// ignore it
	context.Current().PcNext();
}

void ParserGeneric::_execute_begin_type(ListParserContextExec & /*context*/)
{
	throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);
}

void ParserGeneric::_execute_end_type(ListParserContextExec & context)
{
	context.Current().CallStackPop();
}

void ParserGeneric::_execute_sub_type(ListParserContextExec & context)
{
	// get entry point
	ParserItem * called = FindSymbolByName(context.Current().PcGet()->Sub());
	ParserException::CheckNull(called);

	// get next item following current item
	called = called->Next();
	ParserException::CheckNull(called);

	context.Current().CallStackPush(called);
}

void ParserGeneric::_execute_callback_type(ListParserContextExec & context)
{
	// record item and value, it will be executed later
	ParserItem * item = context.Current().PcGet();
	context.Current().getCallbackDelayed().push_back(item);
	context.Current().getValueDelayed().push_back(m_value);

	// and advance PC
	context.Current().PcNext();
}

void ParserGeneric::Greedy(const ParserPow & pow, int token)
{
	AddGreedy(pow.Rule(), token);
}

void ParserGeneric::Cardinal_1(const ParserPow & pow,
										 VariantParam param0,
										 VariantParam param1 /*= VariantParam()*/,
										 VariantParam param2 /*= VariantParam()*/,
										 VariantParam param3 /*= VariantParam()*/,
										 VariantParam param4 /*= VariantParam()*/,
										 VariantParam param5 /*= VariantParam()*/,
										 VariantParam param6 /*= VariantParam()*/,
										 VariantParam param7 /*= VariantParam()*/,
										 VariantParam param8 /*= VariantParam()*/,
										 VariantParam param9 /*= VariantParam()*/)
{
	_execute(pow, param0);
	_execute(pow, param1);
	_execute(pow, param2);
	_execute(pow, param3);
	_execute(pow, param4);
	_execute(pow, param5);
	_execute(pow, param6);
	_execute(pow, param7);
	_execute(pow, param8);
	_execute(pow, param9);
}

void ParserGeneric::Cardinal_0_1(const ParserPow & pow,
											VariantParam param0,
											VariantParam param1 /*= VariantParam()*/,
											VariantParam param2 /*= VariantParam()*/,
											VariantParam param3 /*= VariantParam()*/,
											VariantParam param4 /*= VariantParam()*/,
											VariantParam param5 /*= VariantParam()*/,
											VariantParam param6 /*= VariantParam()*/,
											VariantParam param7 /*= VariantParam()*/,
											VariantParam param8 /*= VariantParam()*/,
											VariantParam param9 /*= VariantParam()*/)
{
	// start
	ParserItem * start1 = AddJump(pow.Rule());

	// actions
	ParserItem * item1 = _execute(pow, param0);
	_execute(pow, param1);
	_execute(pow, param2);
	_execute(pow, param3);
	_execute(pow, param4);
	_execute(pow, param5);
	_execute(pow, param6);
	_execute(pow, param7);
	_execute(pow, param8);
	_execute(pow, param9);

	// end
	ParserItem * end1 = AddPoint(pow.Rule());

	// make jumps
	start1->Jump(item1);
	start1->Jump(end1);
}

void ParserGeneric::Cardinal_0_n(const ParserPow & pow,
											VariantParam param0,
											VariantParam param1 /*= VariantParam()*/,
											VariantParam param2 /*= VariantParam()*/,
											VariantParam param3 /*= VariantParam()*/,
											VariantParam param4 /*= VariantParam()*/,
											VariantParam param5 /*= VariantParam()*/,
											VariantParam param6 /*= VariantParam()*/,
											VariantParam param7 /*= VariantParam()*/,
											VariantParam param8 /*= VariantParam()*/,
											VariantParam param9 /*= VariantParam()*/)
{
	// start
	ParserItem * start1 = AddJump(pow.Rule());

	// actions
	ParserItem * item1 = _execute(pow, param0);
	_execute(pow, param1);
	_execute(pow, param2);
	_execute(pow, param3);
	_execute(pow, param4);
	_execute(pow, param5);
	_execute(pow, param6);
	_execute(pow, param7);
	_execute(pow, param8);
	_execute(pow, param9);

	// end
	ParserItem * start2 = AddJump(pow.Rule());
	ParserItem * end1 = AddPoint(pow.Rule());

	// make jumps
	start1->Jump(item1);
	start1->Jump(end1);
	start2->Jump(item1);
	start2->Jump(end1);
}

void ParserGeneric::Cardinal_1_n(const ParserPow & pow,
											VariantParam param0,
											VariantParam param1 /*= VariantParam()*/,
											VariantParam param2 /*= VariantParam()*/,
											VariantParam param3 /*= VariantParam()*/,
											VariantParam param4 /*= VariantParam()*/,
											VariantParam param5 /*= VariantParam()*/,
											VariantParam param6 /*= VariantParam()*/,
											VariantParam param7 /*= VariantParam()*/,
											VariantParam param8 /*= VariantParam()*/,
											VariantParam param9 /*= VariantParam()*/)
{
	// actions
	ParserItem * item1 = _execute(pow, param0);
	_execute(pow, param1);
	_execute(pow, param2);
	_execute(pow, param3);
	_execute(pow, param4);
	_execute(pow, param5);
	_execute(pow, param6);
	_execute(pow, param7);
	_execute(pow, param8);
	_execute(pow, param9);

	// end
	ParserItem * start1 = AddJump(pow.Rule());
	ParserItem * end1 = AddPoint(pow.Rule());

	// make jumps
	start1->Jump(item1);
	start1->Jump(end1);
}

void ParserGeneric::_branch(const ParserPow & pow,
									 ParserItem * & start,
									 ParserItem * & end,
									 VariantParam & param)
{
	start = nullptr;
	end = nullptr;

	if(param.isNull() == false)
	{
		start = _execute(pow, param);
		end = AddJump(pow.Rule());
	}
}

void ParserGeneric::Branch(const ParserPow & pow,
									VariantParam branch_0,
									VariantParam branch_1 /*= VariantParam()*/,
									VariantParam branch_2 /*= VariantParam()*/,
									VariantParam branch_3 /*= VariantParam()*/,
									VariantParam branch_4 /*= VariantParam()*/,
									VariantParam branch_5 /*= VariantParam()*/,
									VariantParam branch_6 /*= VariantParam()*/,
									VariantParam branch_7 /*= VariantParam()*/,
									VariantParam branch_8 /*= VariantParam()*/,
									VariantParam branch_9 /*= VariantParam()*/,
									VariantParam branch_10 /*= VariantParam()*/,
									VariantParam branch_11 /*= VariantParam()*/,
									VariantParam branch_12 /*= VariantParam()*/,
									VariantParam branch_13 /*= VariantParam()*/,
									VariantParam branch_14 /*= VariantParam()*/,
									VariantParam branch_15 /*= VariantParam()*/,
									VariantParam branch_16 /*= VariantParam()*/,
									VariantParam branch_17 /*= VariantParam()*/,
									VariantParam branch_18 /*= VariantParam()*/,
									VariantParam branch_19 /*= VariantParam()*/)
{
	ParserItem * start = AddJump(pow.Rule());

	ParserItem * start_branch_0;
	ParserItem * end_branch_0;
	_branch(pow, start_branch_0, end_branch_0, branch_0);

	ParserItem * start_branch_1;
	ParserItem * end_branch_1;
	_branch(pow, start_branch_1, end_branch_1, branch_1);

	ParserItem * start_branch_2;
	ParserItem * end_branch_2;
	_branch(pow, start_branch_2, end_branch_2, branch_2);

	ParserItem * start_branch_3;
	ParserItem * end_branch_3;
	_branch(pow, start_branch_3, end_branch_3, branch_3);

	ParserItem * start_branch_4;
	ParserItem * end_branch_4;
	_branch(pow, start_branch_4, end_branch_4, branch_4);

	ParserItem * start_branch_5;
	ParserItem * end_branch_5;
	_branch(pow, start_branch_5, end_branch_5, branch_5);

	ParserItem * start_branch_6;
	ParserItem * end_branch_6;
	_branch(pow, start_branch_6, end_branch_6, branch_6);

	ParserItem * start_branch_7;
	ParserItem * end_branch_7;
	_branch(pow, start_branch_7, end_branch_7, branch_7);

	ParserItem * start_branch_8;
	ParserItem * end_branch_8;
	_branch(pow, start_branch_8, end_branch_8, branch_8);

	ParserItem * start_branch_9;
	ParserItem * end_branch_9;
	_branch(pow, start_branch_9, end_branch_9, branch_9);

	ParserItem * start_branch_10;
	ParserItem * end_branch_10;
	_branch(pow, start_branch_10, end_branch_10, branch_10);

	ParserItem * start_branch_11;
	ParserItem * end_branch_11;
	_branch(pow, start_branch_11, end_branch_11, branch_11);

	ParserItem * start_branch_12;
	ParserItem * end_branch_12;
	_branch(pow, start_branch_12, end_branch_12, branch_12);

	ParserItem * start_branch_13;
	ParserItem * end_branch_13;
	_branch(pow, start_branch_13, end_branch_13, branch_13);

	ParserItem * start_branch_14;
	ParserItem * end_branch_14;
	_branch(pow, start_branch_14, end_branch_14, branch_14);

	ParserItem * start_branch_15;
	ParserItem * end_branch_15;
	_branch(pow, start_branch_15, end_branch_15, branch_15);

	ParserItem * start_branch_16;
	ParserItem * end_branch_16;
	_branch(pow, start_branch_16, end_branch_16, branch_16);

	ParserItem * start_branch_17;
	ParserItem * end_branch_17;
	_branch(pow, start_branch_17, end_branch_17, branch_17);

	ParserItem * start_branch_18;
	ParserItem * end_branch_18;
	_branch(pow, start_branch_18, end_branch_18, branch_18);

	ParserItem * start_branch_19;
	ParserItem * end_branch_19;
	_branch(pow, start_branch_19, end_branch_19, branch_19);

	ParserItem * end = AddPoint(pow.Rule());

	// add jump
	start->Jump(start_branch_0);
	if(start_branch_1 != nullptr) start->Jump(start_branch_1);
	if(start_branch_2 != nullptr) start->Jump(start_branch_2);
	if(start_branch_3 != nullptr) start->Jump(start_branch_3);
	if(start_branch_4 != nullptr) start->Jump(start_branch_4);
	if(start_branch_5 != nullptr) start->Jump(start_branch_5);
	if(start_branch_6 != nullptr) start->Jump(start_branch_6);
	if(start_branch_7 != nullptr) start->Jump(start_branch_7);
	if(start_branch_8 != nullptr) start->Jump(start_branch_8);
	if(start_branch_9 != nullptr) start->Jump(start_branch_9);
	if(start_branch_10 != nullptr) start->Jump(start_branch_10);
	if(start_branch_11 != nullptr) start->Jump(start_branch_11);
	if(start_branch_12 != nullptr) start->Jump(start_branch_12);
	if(start_branch_13 != nullptr) start->Jump(start_branch_13);
	if(start_branch_14 != nullptr) start->Jump(start_branch_14);
	if(start_branch_15 != nullptr) start->Jump(start_branch_15);
	if(start_branch_16 != nullptr) start->Jump(start_branch_16);
	if(start_branch_17 != nullptr) start->Jump(start_branch_17);
	if(start_branch_18 != nullptr) start->Jump(start_branch_18);
	if(start_branch_19 != nullptr) start->Jump(start_branch_19);

	end_branch_0->Jump(end);
	if(end_branch_1 != nullptr) end_branch_1->Jump(end);
	if(end_branch_2 != nullptr) end_branch_2->Jump(end);
	if(end_branch_3 != nullptr) end_branch_3->Jump(end);
	if(end_branch_4 != nullptr) end_branch_4->Jump(end);
	if(end_branch_5 != nullptr) end_branch_5->Jump(end);
	if(end_branch_6 != nullptr) end_branch_6->Jump(end);
	if(end_branch_7 != nullptr) end_branch_7->Jump(end);
	if(end_branch_8 != nullptr) end_branch_8->Jump(end);
	if(end_branch_9 != nullptr) end_branch_9->Jump(end);
	if(end_branch_10 != nullptr) end_branch_10->Jump(end);
	if(end_branch_11 != nullptr) end_branch_11->Jump(end);
	if(end_branch_12 != nullptr) end_branch_12->Jump(end);
	if(end_branch_13 != nullptr) end_branch_13->Jump(end);
	if(end_branch_14 != nullptr) end_branch_14->Jump(end);
	if(end_branch_15 != nullptr) end_branch_15->Jump(end);
	if(end_branch_16 != nullptr) end_branch_16->Jump(end);
	if(end_branch_17 != nullptr) end_branch_17->Jump(end);
	if(end_branch_18 != nullptr) end_branch_18->Jump(end);
	if(end_branch_19 != nullptr) end_branch_19->Jump(end);
}

ParserItem * ParserGeneric::_execute(const ParserPow & pow, VariantParam & param)
{
	if(param.isNull() == true)
		return nullptr;

	if(param.isToken() == true)
		return AddToken(pow.Rule(), param.toToken());

	if(param.isName() == true)
		return AddSub(pow.Rule(), param.toName());

	throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);
	return nullptr;
}

void ParserGeneric::Call(ParserPow & pow)
{
	AddCallback(pow.Rule(), pow.Order());
}

void ParserGeneric::CheckParser(void) const
{
	// iterate through the tree
	for(const auto & item : m_parser)
	{
		if(item->isSub() == true)
		{
			// check that called sub is existing
			if(FindSymbolByName(item->Sub(), false) == nullptr)
			{
				throw ParserExceptionRuleNotExist(item->Sub(), item->Rule());
			}
		}

		if(item->isBegin() == true)
		{
			if( (item->isRoot() == false) && (IsCalled(item->Rule()) == false) )
			{
				throw ParserExceptionRuleNotCalled(item->Rule());
			}
		}
	}
}
