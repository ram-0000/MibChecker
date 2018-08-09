#include "IfsImageResourceDirectoryEntry.h"
#include "IfsImageResourceDirectory.h"
#include "IfsPointer.h"

IfsImageResourceDirectoryEntry::IfsImageResourceDirectoryEntry(const void * p_begin_section, quint32 offset, quint16 p_level)
	: Ifs(IfsPointer::s_Add(p_begin_section, offset), sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY))
{
	m_image_resource_data_entry = NULL;

	// save the level
	m_level = p_level;

	quint32 mask = 0x80000000;
	quint32 anti_mask = ~mask;

	// get name or identifier
	if( (Get()->Name & mask) == mask)
	{
		// entry is a name
		getString(p_begin_section, Get()->Name & anti_mask);
	}
	else
	{
		// entry is an integer
		m_name = QString::number(Get()->Name);
	}

	// check if entry or subdirectory
	m_sub_directory = NULL;
	if( (Get()->OffsetToData & mask) == mask)
	{
		quint32 new_offset = Get()->OffsetToData & anti_mask;
		m_sub_directory = new IfsImageResourceDirectory(p_begin_section, new_offset, m_level + 1);
	}
	else
	{
		// it is an entry, load data entry
		m_image_resource_data_entry = new IfsImageResourceDataEntry(p_begin_section, Get()->OffsetToData);
	}
}

IfsImageResourceDirectoryEntry::~IfsImageResourceDirectoryEntry(void)
{
	if(m_sub_directory != NULL)
		delete m_sub_directory;

	if(m_image_resource_data_entry != NULL)
		delete m_image_resource_data_entry;
}

const IMAGE_RESOURCE_DIRECTORY_ENTRY *IfsImageResourceDirectoryEntry::Get(void) const
{
	return reinterpret_cast<const IMAGE_RESOURCE_DIRECTORY_ENTRY *>(Begin());
}

QString IfsImageResourceDirectoryEntry::getString(const void * /*start*/, quint32 /*offset*/) const
{
	// TODO IfsImageResourceDirectoryEntry: read the unicode string
	return "to be completed";
}
