#ifndef IMAGEFILESTRUCTUREIMAGERESOURCEDIRECTORYENTRY_H
#define IMAGEFILESTRUCTUREIMAGERESOURCEDIRECTORYENTRY_H

#include "Ifs.h"
#include "IfsImageResourceDataEntry.h"
#include <QString>
#include <windows.h>

class IfsImageResourceDirectory;

// see https://msdn.microsoft.com/en-us/library/ms809762.aspx
class IfsImageResourceDirectoryEntry : public Ifs
{
public:
	IfsImageResourceDirectoryEntry(const void *p_begin_section, quint32 offset, quint16 p_level);
	~IfsImageResourceDirectoryEntry(void);

	const IMAGE_RESOURCE_DIRECTORY_ENTRY * Get(void) const;

	inline const QString & Name(void) const { return m_name; }
	inline bool IsDirectory(void) const { return m_sub_directory != NULL; }
	inline const IfsImageResourceDirectory * SubDirectory(void) const { return m_sub_directory; }
	inline const IfsImageResourceDataEntry * DataEntry(void) const { return m_image_resource_data_entry; }

protected:

private:
	QString getString(const void * start, quint32 offset) const;
	QString m_name;
	IfsImageResourceDirectory * m_sub_directory;
	quint16 m_level;
	IfsImageResourceDataEntry * m_image_resource_data_entry;

};

#endif // IMAGEFILESTRUCTUREIMAGERESOURCEDIRECTORYENTRY_H
