#ifndef IFS_IMAGEFILE_H
#define IFS_IMAGEFILE_H

#include <QString>
#include <QFile>
#include "IfsDosHeader.h"
#include "IfsNtHeaders.h"
#include "IfsImageSectionHeader.h"
#include "IfsImageResourceDirectory.h"

class IfsImageFile
{
public:
	IfsImageFile(const char * Filename = nullptr);
	~IfsImageFile(void);
	inline bool IsValid(void) const { return m_valid; }

	const void * GetResourceVersionPtr(void) const;

private:
	QString m_filename;
	QFile m_file;
	quint8 * m_map;
	IfsDosHeader * m_dos_header;
	IfsNtHeaders * m_nt_headers;
	bool m_valid;
	QVector<IfsImageSectionHeader *> m_image_section_header;
	IfsImageResourceDirectory * m_image_resource_directory;

	const void * GetPointerFromRVA(quint32 p_rva) const;
	const IfsImageSectionHeader * GetEnclosingSectionHeader(quint32 p_rva) const;

};

#endif // IFS_IMAGEFILE_H
