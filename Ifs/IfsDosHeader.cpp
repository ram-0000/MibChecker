#include "IfsDosHeader.h"
#include "Debug.h"

IfsDosHeader::IfsDosHeader(const void *p_begin)
	: Ifs(p_begin, sizeof(IMAGE_DOS_HEADER))
{
	// check magic number
	quint16 e_magic = Get()->e_magic;
	if(e_magic != number('M', 'Z') )
	{
		FATAL("ImageFileStructureDosHeader: bad magic number");
		Reset();
		return;
	}
}

IfsDosHeader::~IfsDosHeader(void)
{
}

const IMAGE_DOS_HEADER *IfsDosHeader::Get(void) const
{
	return reinterpret_cast<const IMAGE_DOS_HEADER *>(Begin());
}


