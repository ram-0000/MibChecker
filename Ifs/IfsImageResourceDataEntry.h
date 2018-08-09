#ifndef IFSIMAGERESOURCEDATAENTRY_H
#define IFSIMAGERESOURCEDATAENTRY_H

#include "Ifs.h"
#include <windows.h>

// see http://www.csn.ul.ie/~caolan/pub/winresdump/winresdump/doc/pefile.html
class IfsImageResourceDataEntry : public Ifs
{
public:
	IfsImageResourceDataEntry(const void * p_begin_section, quint32 offset);
	~IfsImageResourceDataEntry(void);

	const IMAGE_RESOURCE_DATA_ENTRY * Get(void) const;

protected:

private:

};

#endif // IFSIMAGERESOURCEDATAENTRY_H
