#ifndef IMAGEFILESTRUCTUREIMAGERESOURCEDIRECTORY_H
#define IMAGEFILESTRUCTUREIMAGERESOURCEDIRECTORY_H

#include "Ifs.h"
#include "IfsImageResourceDirectoryEntry.h"
#include <QVector>
#include <windows.h>

// see https://msdn.microsoft.com/en-us/library/ms809762.aspx
class IfsImageResourceDirectory : public Ifs
{
public:
	IfsImageResourceDirectory(const void * p_begin_section, quint32 offset, quint16 p_level);
	~IfsImageResourceDirectory(void);

	const IMAGE_RESOURCE_DIRECTORY * Get(void) const;

	const IfsImageResourceDirectory * GetDirectory(const QString & p_name) const;
	const IfsImageResourceDataEntry * GetEntryOffsetData(const QString & p_name) const;

protected:

private:
	QVector<IfsImageResourceDirectoryEntry *> m_directory_entry;

};

#endif // IMAGEFILESTRUCTUREIMAGERESOURCEDIRECTORY_H
