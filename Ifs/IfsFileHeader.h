#ifndef IMAGEFILESTRUCTUREFILEHEADER_H
#define IMAGEFILESTRUCTUREFILEHEADER_H

#include "Ifs.h"
#include <windows.h>

// see https://msdn.microsoft.com/en-us/library/windows/desktop/ms680313%28v=vs.85%29.aspx
class IfsFileHeader : public Ifs
{
public:
	IfsFileHeader(const void * p_begin);
	~IfsFileHeader(void);

	const IMAGE_FILE_HEADER * Get(void) const;

protected:

private:

};

#endif // IMAGEFILESTRUCTUREFILEHEADER_H
