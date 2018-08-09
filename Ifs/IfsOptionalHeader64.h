#ifndef IMAGEFILESTRUCTUREOPTIONALHEADER64_H
#define IMAGEFILESTRUCTUREOPTIONALHEADER64_H

#include "Ifs.h"
#include "IfsImageDataDirectory.h"
#include <QVector>

// see https://msdn.microsoft.com/fr-fr/library/windows/desktop/ms680339%28v=vs.85%29.aspx
class IfsOptionalHeader64 : public Ifs
{
public:
	IfsOptionalHeader64(const void * p_begin, quint16 p_size);
	~IfsOptionalHeader64(void);

	const IMAGE_OPTIONAL_HEADER64 * Get(void) const;
	const IfsImageDataDirectory * GetImageDataDirectory(quint16 p_idx) const;

protected:

private:
	QVector<IfsImageDataDirectory *> m_ImageDataDirectory;

};

#endif // IMAGEFILESTRUCTUREOPTIONALHEADER64_H
