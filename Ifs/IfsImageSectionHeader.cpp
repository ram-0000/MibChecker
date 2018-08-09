#include "IfsImageSectionHeader.h"

IfsImageSectionHeader::IfsImageSectionHeader(const void * p_begin)
	: Ifs(p_begin, sizeof(IMAGE_SECTION_HEADER))
{
}

IfsImageSectionHeader::~IfsImageSectionHeader(void)
{
}

const IMAGE_SECTION_HEADER *IfsImageSectionHeader::Get(void) const
{
	return reinterpret_cast<const IMAGE_SECTION_HEADER *>(Begin());
}

