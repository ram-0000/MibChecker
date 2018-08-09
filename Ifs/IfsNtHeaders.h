#ifndef IMAGEFILESTRUCTURENTHEADERS_H
#define IMAGEFILESTRUCTURENTHEADERS_H

#include "Ifs.h"
#include "IfsFileHeader.h"
#include "IfsOptionalHeader32.h"
#include "IfsOptionalHeader64.h"

// see https://msdn.microsoft.com/en-us/library/windows/desktop/ms680336%28v=vs.85%29.aspx
class IfsNtHeaders : public Ifs
{
public:
	IfsNtHeaders(const void * p_begin);
	~IfsNtHeaders(void);

	const IMAGE_NT_HEADERS * Get(void) const;
	const IfsFileHeader * GetFileHeader(void) const;
	const IfsOptionalHeader32 * GetOptionalHeader32(void) const;
	const IfsOptionalHeader64 * GetOptionalHeader64(void) const;

protected:

private:
	IfsFileHeader * m_file_header;
	IfsOptionalHeader32 * m_optional_header32;
	IfsOptionalHeader64 * m_optional_header64;

};

#endif // IMAGEFILESTRUCTURENTHEADERS_H
