#ifndef IFS_POINTER_H
#define IFS_POINTER_H

#include <QtGlobal>
#include <QChar>

class IfsPointer
{
public:
	IfsPointer(const void * p_ptr);
	IfsPointer(const IfsPointer & p_other);
	~IfsPointer(void);

	IfsPointer & operator=(const IfsPointer & p_other);
	const void * operator()(void);

	const void * Reset(void);
	const void * Set(const void * p_ptr);
	const void * Get(void) const;

	const quint8 * ToU8(void) const;
	const quint16 * ToU16(void) const;
	const quint32 * ToU32(void) const;
	const quint64 * ToU64(void) const;

	const qint8 * ToS8(void) const;
	const qint16 * ToS16(void) const;
	const qint32 * ToS32(void) const;
	const qint64 * ToS64(void) const;

	const char * ToChar(void) const;
	const QChar * ToQChar(void) const;

	const void * Add(quint32 p_val);
	const void * Sub(quint32 p_val);

	const void * Align(quint32 p_frontier);

	static const quint8 * s_ToU8(const void * p_ptr);
	static const void * s_Add(const void * p_ptr, quint32 p_val);
	static const void * s_Sub(const void * p_ptr, quint32 p_val);
	static quint32 s_Diff(const void * p_ptr1, const void * p_ptr2);

private:
	const void * m_ptr;
};

#endif // IFS_POINTER_H
