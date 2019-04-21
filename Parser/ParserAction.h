#ifndef PARSERACTION_H
#define PARSERACTION_H

#include "ParserValue.h"

class ParserAction
{
public:
	ParserAction(void);
	virtual ~ParserAction(void);

	virtual void OnAction(int MibLine,
								 const char * Rule,
								 int Order,
								 ParserValue & Value) = 0;
};

#endif // PARSERACTION_H
