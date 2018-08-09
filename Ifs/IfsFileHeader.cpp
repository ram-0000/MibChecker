#include "IfsFileHeader.h"
#include "Debug.h"

IfsFileHeader::IfsFileHeader(const void *p_begin)
	: Ifs(p_begin, sizeof(IMAGE_FILE_HEADER))
{
	// check machine type
	quint16 machine = Get()->Machine;
	if( (machine != 0x014c) &&			// x86
		 (machine != 0x0200) &&			// Intel Itanium
		 (machine != 0x8664) )			// x64
	{
		FATAL("ImageFileStructureFileHeader: bad machine type");
		Reset();
		return;
	}
}

IfsFileHeader::~IfsFileHeader(void)
{
}

const IMAGE_FILE_HEADER *IfsFileHeader::Get(void) const
{
	return reinterpret_cast<const IMAGE_FILE_HEADER *>(Begin());
}

