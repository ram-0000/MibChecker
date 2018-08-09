#include "IfsImageFile.h"
#include <QCoreApplication>
#include "IfsPointer.h"
#include "Debug.h"

// TODO ImageFile: Add more check when reading
// TODO ImageFile: Add global validity function

IfsImageFile::IfsImageFile(const char * Filename /*= NULL*/)
{
	// store file path
	m_map = NULL;
	m_dos_header = NULL;
	m_nt_headers = NULL;
	m_image_section_header.clear();
	m_image_resource_directory = NULL;
	m_valid = false;

	if(Filename == NULL)
		m_filename = QCoreApplication::applicationFilePath();
	else
		m_filename = Filename;

	// open the file
	m_file.setFileName(m_filename);
	if(m_file.open(QFile::ReadOnly) == false)
	{
		// unable to open the file
		ERREUR("ImageFile: Unable to open file %s", m_filename.toUtf8().constData());
		return;
	}

	// get pointer on memory
	m_map = m_file.map(0, m_file.size());
	if(m_map == NULL)
	{
		// unable to map the file
		ERREUR("ImageFile: Unable to map file %s", m_filename.toUtf8().constData());
		return;
	}

	// create the dos header, it start from begining
	m_dos_header = new IfsDosHeader(m_map);

	// create the nt headers, it start from dos_header + e_lfanew
	const void * start = IfsPointer::s_Add(m_dos_header->Begin(), m_dos_header->Get()->e_lfanew);
	m_nt_headers = new IfsNtHeaders(start);

	// create image section header list
	const void * start_image_section = m_nt_headers->End();
	quint16 nb_section = m_nt_headers->GetFileHeader()->Get()->NumberOfSections;
	quint16 size_section = sizeof(IMAGE_SECTION_HEADER);
	for(int boucle = 0; boucle != nb_section; boucle++)
	{
		// start of section
		m_image_section_header.append(new IfsImageSectionHeader(start_image_section));
		start_image_section = IfsPointer::s_Add(start_image_section, size_section);
	}

	// create resource directory entry
	const IfsImageSectionHeader * p;
	for(int i = 0; i != m_image_section_header.size(); i++)
	{
		p = m_image_section_header[i];
		if(strcmp(reinterpret_cast<const char *>(p->Get()->Name), ".rsrc") == 0)
		{
			const void * start = IfsPointer::s_Add(m_map, p->Get()->PointerToRawData);
			m_image_resource_directory = new IfsImageResourceDirectory(start, 0, 0);
			break;
		}
	}

	if(m_image_resource_directory == NULL)
		return;

	// all seems correct
	m_valid = true;
}

IfsImageFile::~IfsImageFile(void)
{
	if(m_image_resource_directory != NULL)
		delete m_image_resource_directory;

	for(int boucle = 0; boucle != m_image_section_header.size(); boucle++)
		delete m_image_section_header[boucle];
	m_image_section_header.clear();

	if(m_dos_header != NULL)
		delete m_dos_header;
	if(m_nt_headers != NULL)
		delete m_nt_headers;

	// unmap file
	if(m_map != NULL)
		m_file.unmap(m_map);

	// close the file
	if(m_file.isOpen() == true)
		m_file.close();

	// forget the filename
	m_filename.clear();
}

const void * IfsImageFile::GetResourceVersionPtr(void) const
{
	// create resource Version
	if(m_image_resource_directory == NULL)
	{
		// no resource to parse
		ERREUR("No resource to parse");
		return NULL;
	}

	// parse first level
	// 16 is the identifier of Version resource
	const IfsImageResourceDirectory * level_1 = m_image_resource_directory->GetDirectory("16");
	if(level_1 == NULL)
	{
		ERREUR("No version resource");
		return NULL;
	}

	// parse second level
	// get first entry (it is theorically possible to have several Version)
	const IfsImageResourceDirectory * level_2 = level_1->GetDirectory("");
	if(level_2 == NULL)
	{
		ERREUR("No version resource");
		return NULL;
	}

	const IfsImageResourceDataEntry * data_entry = level_2->GetEntryOffsetData("");
	if(data_entry == NULL)
	{
		ERREUR("No version resource");
		return NULL;
	}

	// offset is relative to m_image_resource_directory
	const void * ptr = GetPointerFromRVA(data_entry->Get()->OffsetToData);
	return ptr;
}

const void * IfsImageFile::GetPointerFromRVA(quint32 p_rva) const
{
	const IfsImageSectionHeader * Section = GetEnclosingSectionHeader(p_rva);
	if(Section == NULL)
		return NULL;

	quint32 delta = Section->Get()->VirtualAddress - Section->Get()->PointerToRawData;
	const void * ptr = m_map;
	ptr = IfsPointer::s_Add(ptr, p_rva);
	ptr = IfsPointer::s_Sub(ptr, delta);
	return ptr;
}

const IfsImageSectionHeader * IfsImageFile::GetEnclosingSectionHeader(quint32 p_rva) const
{
	for(int boucle = 0; boucle != m_image_section_header.size(); boucle++)
	{
		const IfsImageSectionHeader * p = m_image_section_header[boucle];

		// This 3 line idiocy is because Watcom's linker actually sets the
		// Misc.VirtualSize field to 0.  (!!! - Retards....!!!)
		quint32 size = p->Get()->Misc.VirtualSize;
		if(size == 0)
			size = p->Get()->SizeOfRawData;

		// Is the RVA within this section?
		if( (p_rva >= p->Get()->VirtualAddress) &&
			 (p_rva < p->Get()->VirtualAddress + size))
			return p;
	}
	return NULL;
}

