#include "ParserException.h"
#include "Debug.h"

void ParserException::CheckNull(const void * Pointer)
{
	if(Pointer == NULL)
		throw ParserExceptionNullPointer();
}

void ParserException::CheckNullOrEmpty(const void * Pointer)
{
	CheckNull(Pointer);
	if((*reinterpret_cast<const char *>(Pointer) == 0) )
		throw ParserExceptionEmptyString();
}

ParserException::ParserException(ParserException_t ident,
											const char * Format,
											const QString & p1 /*= ""*/,
											const QString & p2 /*= ""*/,
											const QString & p3 /*= ""*/,
											const QString & p4 /*= ""*/,
											const QString & p5 /*= ""*/,
											const QString & p6 /*= ""*/,
											const QString & p7 /*= ""*/,
											const QString & p8 /*= ""*/,
											const QString & p9 /*= ""*/)
{
	m_ident = ident;
	m_string = Format;
	P1(p1); if(P1().length() != 0) m_string.replace("%1", P1());
	P2(p2); if(P2().length() != 0) m_string.replace("%2", P2());

	if(p3.length() != 0) m_string.replace("%3", p3);
	if(p4.length() != 0) m_string.replace("%4", p4);
	if(p5.length() != 0) m_string.replace("%5", p5);
	if(p6.length() != 0) m_string.replace("%6", p6);
	if(p7.length() != 0) m_string.replace("%7", p7);
	if(p8.length() != 0) m_string.replace("%8", p8);
	if(p9.length() != 0) m_string.replace("%9", p9);
	ERREUR("%s", m_string.toLatin1().constData());
}
