#ifndef IMAGEFILESTRUCTUREIMAGEDATADIRECTORY_H
#define IMAGEFILESTRUCTUREIMAGEDATADIRECTORY_H

#include "Ifs.h"
#include <windows.h>

// see https://msdn.microsoft.com/fr-fr/library/windows/desktop/ms680305%28v=vs.85%29.aspx
class IfsImageDataDirectory : public Ifs
{
public:
	IfsImageDataDirectory(const void * p_begin);
	~IfsImageDataDirectory(void);

	const IMAGE_DATA_DIRECTORY * Get(void) const;

protected:

private:

};

#endif // IMAGEFILESTRUCTUREIMAGEDATADIRECTORY_H
