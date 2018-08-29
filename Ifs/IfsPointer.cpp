#include "IfsPointer.h"

IfsPointer::IfsPointer(const void * p_ptr)
{
	Reset();
	Set(p_ptr);
}

IfsPointer::IfsPointer(const IfsPointer & p_other)
{
	Reset();
	Set(p_other.Get());
}

IfsPointer::~IfsPointer(void)
{
	Reset();
}

IfsPointer & IfsPointer::operator=(const IfsPointer & p_other)
{
	Reset();
	Set(p_other.Get());
	return *this;
}

const void * IfsPointer::operator()(void)
{
	return Get();
}

const void * IfsPointer::Reset(void)
{
	return Set(nullptr);
}

const void * IfsPointer::Set(const void * p_ptr)
{
	m_ptr = p_ptr;
	return m_ptr;
}

const void * IfsPointer::Get(void) const
{
	return m_ptr;
}

const void * IfsPointer::Add(quint32 p_val)
{
	Set(ToU8() + p_val);
	return Get();
}

const void * IfsPointer::Sub(quint32 p_val)
{
	Set(ToU8() - p_val);
	return Get();
}

const quint8 * IfsPointer::ToU8(void) const
{
	return reinterpret_cast<const quint8 *>(Get());
}

const quint16 * IfsPointer::ToU16(void) const
{
	return reinterpret_cast<const quint16 *>(Get());
}

const quint32 * IfsPointer::ToU32(void) const
{
	return reinterpret_cast<const quint32 *>(Get());
}

const quint64 * IfsPointer::ToU64(void) const
{
	return reinterpret_cast<const quint64 *>(Get());
}

const qint8 * IfsPointer::ToS8(void) const
{
	return reinterpret_cast<const qint8 *>(Get());
}

const qint16 * IfsPointer::ToS16(void) const
{
	return reinterpret_cast<const qint16 *>(Get());
}

const qint32 * IfsPointer::ToS32(void) const
{
	return reinterpret_cast<const qint32 *>(Get());
}

const qint64 * IfsPointer::ToS64(void) const
{
	return reinterpret_cast<const qint64 *>(Get());
}

const char * IfsPointer::ToChar(void) const
{
	return reinterpret_cast<const char *>(Get());
}

const QChar * IfsPointer::ToQChar(void) const
{
	return reinterpret_cast<const QChar *>(Get());
}

const void * IfsPointer::Align(quint32 p_frontier)
{
	while( (reinterpret_cast<quint32>(Get()) & (p_frontier - 1)) != 0)
	{
		Add(1);
	}

	return Get();
}

const quint8 * IfsPointer::s_ToU8(const void * p_ptr)
{
	return reinterpret_cast<const quint8 *>(p_ptr);
}

const void * IfsPointer::s_Add(const void * p_ptr, quint32 p_val)
{
	return reinterpret_cast<const quint8 *>(p_ptr) + p_val;
}

const void * IfsPointer::s_Sub(const void * p_ptr, quint32 p_val)
{
	return reinterpret_cast<const quint8 *>(p_ptr) - p_val;
}

quint32 IfsPointer::s_Diff(const void * p_ptr1, const void * p_ptr2)
{
	return s_ToU8(p_ptr1) - s_ToU8(p_ptr2);
}
