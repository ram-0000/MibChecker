#include "IfsImageResourceDirectory.h"
#include "IfsPointer.h"

IfsImageResourceDirectory::IfsImageResourceDirectory(const void * p_begin_section, quint32 offset, quint16 p_level)
	: Ifs(IfsPointer::s_Add(p_begin_section, offset), sizeof(IMAGE_RESOURCE_DIRECTORY))
{
	// read directory entries, they are starting at the end of this structure
	quint32 new_offset = IfsPointer::s_Diff(End(), p_begin_section);
	quint16 nb_entry = Get()->NumberOfIdEntries + Get()->NumberOfNamedEntries;
	for(int boucle = 0; boucle != nb_entry; boucle++)
	{
		// create entry
		IfsImageResourceDirectoryEntry * entry = new IfsImageResourceDirectoryEntry(p_begin_section, new_offset, p_level);
		m_directory_entry.append(entry);

		// compute start of next entry
		new_offset += entry->Size();
	}
}

IfsImageResourceDirectory::~IfsImageResourceDirectory(void)
{
	for(int boucle = 0; boucle != m_directory_entry.size(); boucle++)
	{
		if(m_directory_entry[boucle] != nullptr)
			delete m_directory_entry[boucle];
	}
	m_directory_entry.clear();
}

const IMAGE_RESOURCE_DIRECTORY *IfsImageResourceDirectory::Get(void) const
{
	return reinterpret_cast<const IMAGE_RESOURCE_DIRECTORY *>(Begin());
}

const IfsImageResourceDirectory * IfsImageResourceDirectory::GetDirectory(const QString & p_name) const
{
	for(int boucle = 0; boucle != m_directory_entry.size(); boucle++)
	{
		const IfsImageResourceDirectoryEntry * p = m_directory_entry[boucle];
		if(p->IsDirectory() == false)
			continue;

		// if name to search is not set, return the first directory entry
		if(p_name.length() == 0)
			return p->SubDirectory();

		// check the name
		if(p->Name() == p_name)
			return p->SubDirectory();
	}
	return nullptr;
}

const IfsImageResourceDataEntry *IfsImageResourceDirectory::GetEntryOffsetData(const QString & p_name) const
{
	for(int boucle = 0; boucle != m_directory_entry.size(); boucle++)
	{
		const IfsImageResourceDirectoryEntry * p = m_directory_entry[boucle];
		if(p->IsDirectory() == true)
			continue;

		// if name to search is not set, return the first directory entry
		if(p_name.length() == 0)
			return p->DataEntry();

		// check the name
		if(p->Name() == p_name)
			return p->DataEntry();
	}
	return nullptr;
}
