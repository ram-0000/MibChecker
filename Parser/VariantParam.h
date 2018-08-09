#ifndef VARIANTPARAM_H
#define VARIANTPARAM_H

#include "MibLexer.h"

class VariantParam
{
public:
public:
	inline VariantParam(void) { m_name = NULL; m_token = Tok_First; }
	inline VariantParam(const char * name)
		{ m_name = name; m_token = Tok_First; }
	inline VariantParam(int token)
		{ m_name = NULL; m_token = token; }

	inline bool isNull(void) const { return ( (isToken() == false) && (isName() == false) ); }

	inline bool isName(void) const { return m_name != NULL; }
	inline const char * toName(void) const { return m_name; }

	inline bool isToken(void) const { return m_token != Tok_First; }
	inline int toToken(void) const { return m_token; }

private:
	int m_token;
	const char * m_name;
};

#endif // VARIANTPARAM_H
