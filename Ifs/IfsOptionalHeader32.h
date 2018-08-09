#ifndef IMAGEFILESTRUCTUREOPTIONALHEADER32_H
#define IMAGEFILESTRUCTUREOPTIONALHEADER32_H

#include "Ifs.h"
#include "IfsImageDataDirectory.h"
#include <QVector>
#include <windows.h>

// see https://msdn.microsoft.com/en-us/library/windows/desktop/ms680339(v=vs.85).aspx
class IfsOptionalHeader32 : public Ifs
{
public:
	IfsOptionalHeader32(const void * p_begin, quint16 p_size);
	~IfsOptionalHeader32(void);

	const IMAGE_OPTIONAL_HEADER32 * Get(void) const;
	const IfsImageDataDirectory * GetImageDataDirectory(quint16 p_idx) const;

protected:

private:
	QVector<IfsImageDataDirectory *> m_ImageDataDirectory;

};

#endif // IMAGEFILESTRUCTUREOPTIONALHEADER32_H
