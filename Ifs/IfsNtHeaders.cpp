#include "IfsNtHeaders.h"
#include "Debug.h"

IfsNtHeaders::IfsNtHeaders(const void * p_begin)
	: Ifs(p_begin, sizeof(IMAGE_NT_HEADERS))
{
	// check magic number
	quint32 signature = Get()->Signature;
	if(signature != number('P', 'E', 0, 0) )
	{
		FATAL("ImageFileStructureDosHeader: bad signature number");
		Reset();
		return;
	}

	// create file header
	const void * start_header = &Get()->FileHeader;
	m_file_header = new IfsFileHeader(start_header);

	const void * start_optional = m_file_header->End();
	quint16 size_optional= m_file_header->Get()->SizeOfOptionalHeader;

	// check if 32 or 64 bit header
	const quint16 * ptr = reinterpret_cast<const quint16 *>(start_optional);
	quint16 magic = *ptr;

	if(magic == 0x10b)
	{
		// 32 bits version
		m_optional_header32 = new IfsOptionalHeader32(start_optional, size_optional);
		m_optional_header64 = nullptr;
	}
	else if(magic == 0x20b)
	{
		// 64 bits version
		m_optional_header64 = new IfsOptionalHeader64(start_optional, size_optional);
		m_optional_header32 = nullptr;
	}
	else
	{
		// not supported version
		FATAL("ImageFileStructureDosHeader: unsupported optional header version");
		Reset();
		return;
	}
}

IfsNtHeaders::~IfsNtHeaders(void)
{
	if(m_file_header != nullptr)
		delete m_file_header;
	if(m_optional_header32 != nullptr)
		delete m_optional_header32;
	if(m_optional_header64 != nullptr)
		delete m_optional_header64;
}

const IMAGE_NT_HEADERS *IfsNtHeaders::Get(void) const
{
	return reinterpret_cast<const IMAGE_NT_HEADERS *>(Begin());
}

const IfsFileHeader * IfsNtHeaders::GetFileHeader(void) const
{
	return m_file_header;
}

const IfsOptionalHeader32 * IfsNtHeaders::GetOptionalHeader32(void) const
{
	return m_optional_header32;
}

const IfsOptionalHeader64 * IfsNtHeaders::GetOptionalHeader64(void) const
{
	return m_optional_header64;
}

