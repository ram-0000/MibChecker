#ifndef IMAGEFILESTRUCTUREIMAGESECTIONHEADER_H
#define IMAGEFILESTRUCTUREIMAGESECTIONHEADER_H

#include "Ifs.h"
#include <windows.h>

// see https://msdn.microsoft.com/en-us/library/windows/desktop/ms680341%28v=vs.85%29.aspx
class IfsImageSectionHeader : public Ifs
{
public:
	IfsImageSectionHeader(const void * p_begin);
	~IfsImageSectionHeader(void);

	const IMAGE_SECTION_HEADER * Get(void) const;

protected:

private:

};

#endif // IMAGEFILESTRUCTUREIMAGESECTIONHEADER_H
