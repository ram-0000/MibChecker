#ifndef IFS_H
#define IFS_H

#include <QtGlobal>

class Ifs
{
public:
	Ifs(const void * p_begin, const quint32 p_size);
	~Ifs(void);

	void Reset(void);

	const void * Begin(void) const;
	quint32 Size(void) const;
	void Size(quint32 p_new_size);
	const void * End(void) const;
	static QString Indent(quint16 p_level);

protected:

	quint16 number(quint8 p_c1, quint8 p_c2) const;
	quint32 number(quint8 p_c1, quint8 p_c2, quint8 p_c3, quint8 p_c4) const;

private:
	const void *m_begin;
	quint32 m_size;
};

#endif // IFS_H
