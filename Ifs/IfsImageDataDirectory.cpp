#include "IfsImageDataDirectory.h"

IfsImageDataDirectory::IfsImageDataDirectory(const void * p_begin)
	: Ifs(p_begin, sizeof(IMAGE_DATA_DIRECTORY))
{
}

IfsImageDataDirectory::~IfsImageDataDirectory(void)
{
}

const IMAGE_DATA_DIRECTORY * IfsImageDataDirectory::Get(void) const
{
	return reinterpret_cast<const IMAGE_DATA_DIRECTORY *>(Begin());
}

