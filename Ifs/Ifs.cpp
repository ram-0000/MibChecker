#include "Ifs.h"
#include <QString>
#include "IfsPointer.h"

Ifs::Ifs(const void * p_begin, const quint32 p_size)
{
	Reset();
	m_begin = p_begin;
	m_size = p_size;
}

Ifs::~Ifs(void)
{
	Reset();
}

void Ifs::Reset(void)
{
	m_begin = NULL;
	m_size = 0;
}

const void * Ifs::Begin(void) const
{
	return m_begin;
}

quint32 Ifs::Size(void) const
{
	return m_size;
}

void Ifs::Size(quint32 p_new_size)
{
	m_size = p_new_size;
}

const void * Ifs::End(void) const
{
	return IfsPointer::s_Add(Begin(), Size());
}

QString Ifs::Indent(quint16 p_level)
{
	QString ret;
	for(int boucle = 0; boucle != p_level; boucle++)
		ret += "   ";
	return ret;
}

quint16 Ifs::number(quint8 p_c1, quint8 p_c2) const
{
quint16 ret = 0;
	ret += p_c2;
	ret <<= 8;
	ret += p_c1;
	return ret;
}

quint32 Ifs::number(quint8 p_c1, quint8 p_c2, quint8 p_c3, quint8 p_c4) const
{
quint32 ret = 0;
	ret += p_c4;
	ret <<= 8;
	ret += p_c3;
	ret <<= 8;
	ret += p_c2;
	ret <<= 8;
	ret += p_c1;
	return ret;
}

