#ifndef IMAGEFILESTRUCTUREDOSHEADER_H
#define IMAGEFILESTRUCTUREDOSHEADER_H

#include "Ifs.h"
#include <windows.h>

// see https://fr.wikipedia.org/wiki/Portable_Executable
class IfsDosHeader : public Ifs
{
public:
	IfsDosHeader(const void * p_begin);
	~IfsDosHeader(void);

	const IMAGE_DOS_HEADER * Get(void) const;

protected:

private:

};

#endif // IMAGEFILESTRUCTUREDOSHEADER_H
