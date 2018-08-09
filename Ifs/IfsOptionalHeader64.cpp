#include "IfsOptionalHeader64.h"
#include "Debug.h"

IfsOptionalHeader64::IfsOptionalHeader64(const void *p_begin, quint16 p_size)
	: Ifs(p_begin, p_size)
{
	// check magic number
	quint16 magic = Get()->Magic;
	if(magic != 0x20b)
	{
		FATAL("ImageFileStructureOptionalHeader64: bad magic number");
		Reset();
		return;
	}

	// check the size
	if(p_size != sizeof(IMAGE_OPTIONAL_HEADER64))
	{
		WARNING("ImageFileStructureOptionalHeader64: strange size");
	}

	// create m_ImageDataDirectory vector
	for(quint32 boucle = 0; boucle != Get()->NumberOfRvaAndSizes; boucle++)
	{
		const void * start = &Get()->DataDirectory[boucle];
		m_ImageDataDirectory.append(new IfsImageDataDirectory(start));
	}
}

IfsOptionalHeader64::~IfsOptionalHeader64(void)
{
	for(int boucle = 0; boucle != m_ImageDataDirectory.size(); boucle++)
	{
		if(m_ImageDataDirectory[boucle] != NULL)
			delete m_ImageDataDirectory[boucle];
	}
	m_ImageDataDirectory.clear();
}

const IMAGE_OPTIONAL_HEADER64 *IfsOptionalHeader64::Get(void) const
{
	return reinterpret_cast<const IMAGE_OPTIONAL_HEADER64 *>(Begin());
}

const IfsImageDataDirectory * IfsOptionalHeader64::GetImageDataDirectory(quint16 p_idx) const
{
	if(p_idx >= m_ImageDataDirectory.size())
	{
		FATAL("ImageFileStructureOptionalHeader32: index out of range");
		return NULL;
	}
	return m_ImageDataDirectory[p_idx];
}

