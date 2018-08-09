#include "IfsImageResourceDataEntry.h"
#include "IfsPointer.h"

IfsImageResourceDataEntry::IfsImageResourceDataEntry(const void * p_begin_section, quint32 offset)
	: Ifs(IfsPointer::s_Add(p_begin_section, offset), sizeof(IMAGE_RESOURCE_DATA_ENTRY))
{
}

IfsImageResourceDataEntry::~IfsImageResourceDataEntry(void)
{
}

const IMAGE_RESOURCE_DATA_ENTRY * IfsImageResourceDataEntry::Get(void) const
{
	return reinterpret_cast<const IMAGE_RESOURCE_DATA_ENTRY *>(Begin());
}

