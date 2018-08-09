#include "IfsResVersion.h"
#include "Debug.h"
#include "Ifs.h"

IfsResVersion::IfsResVersion(const void *p_ptr)
{
	// check pointer
	m_valid = false;
	if(p_ptr == NULL)
		return;

	// create the memory pointer to patrse the structure
	IfsPointer p(p_ptr);

	// get total size of structure
	quint32 size = *p.ToU16();
	m_counter.Push(size);

	// fill the main structure
	if(read_main_part(p) == false)
		return;

	// read StringFileInfo part
	if(m_counter.Last() != 0)
	{
		// padding alignment
		if(padding(p) == false)
			return;

		// read string file info part
		if(read_string_file_info(p) == false)
			return;
	}

	// read VarFileInfo part
	if(m_counter.Last() != 0)
	{
		// padding alignment
		if(padding(p) == false)
			return;

		// read var file info part
		if(read_var_file_info(p) == false)
			return;
	}

	// remove counter
	if(m_counter.Pop() == false)
		return;

	// check if empty
	if(m_counter.IsEmpty() == false)
		return;

	m_valid = true;
}

IfsResVersion::~IfsResVersion(void)
{
}

bool IfsResVersion::read_main_part(IfsPointer & p_ptr)
{
	if(fill_value(m_version.wLength, p_ptr) == false)
		return false;
	//INFO("wLength=%d", m_version.wLength);

	if(fill_value(m_version.wValueLength, p_ptr) == false)
		return false;
	//INFO("wValueLength=%d", m_version.wValueLength);

	if(fill_value(m_version.wType, p_ptr) == false)
			return false;
	//INFO("wType=%d", m_version.wType);

	if(fill_value(m_version.szKey, p_ptr, 0, true) == false)
		return false;
	//INFO("wszKey='%s'", m_version.szKey.toLatin1().data());
	if(m_version.szKey != "VS_VERSION_INFO")
	{
		// it is not a resource version
		ERREUR("It is not really a resource Version");
		return false;
	}

	memset(&m_version.Value, 0, sizeof(m_version.Value));
	if(m_version.wValueLength == sizeof(m_version.Value))
	{
		// read value
		if(fill_value(m_version.Value.dwSignature, p_ptr) == false)
			return false;
		//INFO("Value.dwSignature=%d", m_version.Value.dwSignature);

		if(fill_value(m_version.Value.dwStrucVersion, p_ptr) == false)
				return false;
		//INFO("Value.dwStrucVersion=%d", m_version.Value.dwStrucVersion);

		if(fill_value(m_version.Value.dwFileVersionMS, p_ptr) == false)
				return false;
		//INFO("Value.dwFileVersionMS=%d", m_version.Value.dwFileVersionMS);

		if(fill_value(m_version.Value.dwFileVersionLS, p_ptr) == false)
				return false;
		//INFO("Value.dwFileVersionLS=%d", m_version.Value.dwFileVersionLS);

		if(fill_value(m_version.Value.dwProductVersionMS, p_ptr) == false)
				return false;
		//INFO("Value.dwProductVersionMS=%d", m_version.Value.dwProductVersionMS);

		if(fill_value(m_version.Value.dwProductVersionLS, p_ptr) == false)
				return false;
		//INFO("Value.dwProductVersionLS=%d", m_version.Value.dwProductVersionLS);

		if(fill_value(m_version.Value.dwFileFlagsMask, p_ptr) == false)
				return false;
		//INFO("Value.dwFileFlagsMask=%d", m_version.Value.dwFileFlagsMask);

		if(fill_value(m_version.Value.dwFileFlags, p_ptr) == false)
				return false;
		//INFO("Value.dwFileFlags=%d", m_version.Value.dwFileFlags);

		if(fill_value(m_version.Value.dwFileOS, p_ptr) == false)
				return false;
		//INFO("Value.dwFileOS=%d", m_version.Value.dwFileOS);

		if(fill_value(m_version.Value.dwFileType, p_ptr) == false)
				return false;
		//INFO("Value.dwFileType=%d", m_version.Value.dwFileType);

		if(fill_value(m_version.Value.dwFileSubtype, p_ptr) == false)
				return false;
		//INFO("Value.dwFileSubtype=%d", m_version.Value.dwFileSubtype);

		if(fill_value(m_version.Value.dwFileDateMS, p_ptr) == false)
				return false;
		//INFO("Value.dwFileDateMS=%d", m_version.Value.dwFileDateMS);

		if(fill_value(m_version.Value.dwFileDateLS, p_ptr) == false)
				return false;
		//INFO("Value.dwFileDateLS=%d", m_version.Value.dwFileDateLS);
	}

	return true;
}

bool IfsResVersion::read_string_file_info(IfsPointer & p_ptr)
{
	// get size
	quint32 size = *p_ptr.ToU16();
	m_counter.Push(size);

	// read values
	if(fill_value(m_version.stringFileInfo.wLength, p_ptr) == false)
		return false;
	//INFO("stringFileInfo.wLength=%d", m_version.stringFileInfo.wLength);

	if(fill_value(m_version.stringFileInfo.wValueLength, p_ptr) == false)
			return false;
	//INFO("stringFileInfo.wValueLength=%d", m_version.stringFileInfo.wValueLength);

	if(fill_value(m_version.stringFileInfo.wType, p_ptr) == false)
			return false;
	//INFO("stringFileInfo.wType=%d", m_version.stringFileInfo.wType);

	if(fill_value(m_version.stringFileInfo.szKey, p_ptr, 0, true) == false)
		return false;
	//INFO("stringFileInfo.szKey='%s'", m_version.stringFileInfo.szKey.toLatin1().data());
	if(m_version.stringFileInfo.szKey != "StringFileInfo")
	{
		// it is not a resource version
		ERREUR("It is not really a resource Version");
		return false;
	}

	while(m_counter.Last() != 0)
	{
		if(read_string_table(p_ptr) == false)
			return false;
	}

	// remove counter
	if(m_counter.Pop() == false)
		return false;

	return true;
}

bool IfsResVersion::read_string_table(IfsPointer & p_ptr)
{
	// get size
	quint32 size = *p_ptr.ToU16();
	m_counter.Push(size);

	// read a string table
	ResVersion_StringTable stringTable;
	if(fill_value(stringTable.wLength, p_ptr) == false)
		return false;
	//INFO("stringTable.wLength=%d", stringTable.wLength);

	if(fill_value(stringTable.wValueLength, p_ptr) == false)
			return false;
	//INFO("stringTable.wValueLength=%d", stringTable.wValueLength);

	if(fill_value(stringTable.wType, p_ptr) == false)
			return false;
	//INFO("stringTable.wType=%d", stringTable.wType);

	if(fill_value(stringTable.szKey, p_ptr, 0, true) == false)
		return false;
	//INFO("stringTable.szKey='%s'", stringTable.szKey.toLatin1().data());

	while(m_counter.Last() != 0)
	{
		if(read_string(stringTable, p_ptr) == false)
			return false;
	}

	// add it to main structure
	m_version.stringFileInfo.stringTable.append(stringTable);

	// remove counter
	if(m_counter.Pop() == false)
		return false;

	return true;
}

bool IfsResVersion::read_string(ResVersion_StringTable & stringTable, IfsPointer & p_ptr)
{
	// padding alignment
	if(padding(p_ptr) == false)
		return false;

	// get size
	quint32 size = *p_ptr.ToU16();
	m_counter.Push(size);

	// read a string
	ResVersion_String string;
	if(fill_value(string.wLength, p_ptr) == false)
		return false;
	//INFO("string.wLength=%d", string.wLength);

	if(fill_value(string.wValueLength, p_ptr) == false)
		return false;
	//INFO("string.wValueLength=%d", string.wValueLength);

	if(fill_value(string.wType, p_ptr) == false)
		return false;
	//INFO("string.wType=%d", string.wType);

	if(fill_value(string.szKey, p_ptr, 0, true) == false)
		return false;
	//INFO("string.szKey='%s'", string.szKey.toLatin1().data());

	if(fill_value(string.Value, p_ptr, string.wValueLength, false) == false)
		return false;
	//INFO("string.Value='%s'", string.Value.toLatin1().data());

	// add it to table
	stringTable.string.append(string);

	// remove counter
	if(m_counter.Pop() == false)
		return false;

	return true;
}

bool IfsResVersion::read_var_file_info(IfsPointer & p_ptr)
{
	// get size
	quint32 size = *p_ptr.ToU16();
	m_counter.Push(size);

	// read values
	if(fill_value(m_version.varFileInfo.wLength, p_ptr) == false)
		return false;
	if(fill_value(m_version.varFileInfo.wValueLength, p_ptr) == false)
			return false;
	if(fill_value(m_version.varFileInfo.wType, p_ptr) == false)
			return false;
	if(fill_value(m_version.varFileInfo.szKey, p_ptr, 0, true) == false)
		return false;
	if(m_version.varFileInfo.szKey != "VarFileInfo")
	{
		// it is not a resource version
		ERREUR("It is not really a resource Version");
		return false;
	}

	while(m_counter.Last() != 0)
	{
		if(read_var(p_ptr) == false)
			return false;
	}

	// remove counter
	if(m_counter.Pop() == false)
		return false;

	return true;
}

bool IfsResVersion::read_var(IfsPointer & p_ptr)
{
	// get size
	quint32 size = *p_ptr.ToU16();
	m_counter.Push(size);

	// read a var table
	ResVersion_Var varTable;
	if(fill_value(varTable.wLength, p_ptr) == false)
		return false;
	if(fill_value(varTable.wValueLength, p_ptr) == false)
		return false;
	if(fill_value(varTable.wType, p_ptr) == false)
		return false;
	if(fill_value(varTable.szKey, p_ptr, 0, true) == false)
		return false;
	if(varTable.szKey != "Translation")
	{
		// it is not a resource version
		ERREUR("It is not really a resource Version");
		return false;
	}

	if(fill_value(varTable.Value, p_ptr) == false)
		return false;

	// add it to main structure
	m_version.varFileInfo.var.append(varTable);

	// remove counter
	if(m_counter.Pop() == false)
		return false;

	return true;
}

bool IfsResVersion::fill_value(quint16 & p_val, IfsPointer & p_ptr)
{
	// get value
	p_val = *p_ptr.ToU16();

	// compute remaining size and make the pointer advance
	quint16 size = sizeof(p_val);
	p_ptr.Add(size);
	if(m_counter.Decremente(size) == false)
		return false;
	return true;
}

bool IfsResVersion::fill_value(quint32 & p_val, IfsPointer & p_ptr)
{
	// get value
	p_val = *p_ptr.ToU32();

	// compute remaining size and make the pointer advance
	quint16 size = sizeof(p_val);
	p_ptr.Add(size);
	if(m_counter.Decremente(size) == false)
		return false;
	return true;
}

bool IfsResVersion::fill_value(QString & p_val, IfsPointer & p_ptr, quint32 p_len, bool p_pad)
{
	// read the whole string
	quint16 nb_read = 0;
	 while(*p_ptr.ToQChar() != QChar(0))
	{
		// complete the string
		p_val += *p_ptr.ToQChar();

		// next char
		p_ptr.Add(2);
		if(m_counter.Decremente(2) == false)
			return false;
		nb_read++;
	}

	// skip the 0 terminal charac
	p_ptr.Add(2);
	if(m_counter.Decremente(2) == false)
		return false;
	nb_read++;

	if( (p_len != 0) && (p_len > nb_read) )
	{
		// skip remaining characters
		for(quint16 boucle = nb_read; boucle != p_len; boucle++)
		{
			p_ptr.Add(2);
			if(m_counter.Decremente(2) == false)
				return false;
		}
	}

	// padding alignment ?
	if(p_pad == true)
		if(padding(p_ptr) == false)
			return false;
	return true;
}

bool IfsResVersion::padding(IfsPointer & p_ptr)
{
	// perform alignment on 4 byte frontier
	quint32 frontier = 4;
	while( (reinterpret_cast<quint32>(p_ptr.Get()) & (frontier - 1)) != 0)
	{
		p_ptr.Add(1);
		if(m_counter.Decremente() == false)
			return false;
	}
	return true;
}

quint8 IfsResVersion::GetFileVersion1(void) const
{
	if(IsValid() == false)
		return 0;
	return m_version.Value.dwFileVersionLS & 0x000000ff;
}

quint8 IfsResVersion::GetFileVersion2(void) const
{
	if(IsValid() == false)
		return 0;
	return (m_version.Value.dwFileVersionLS >> 8) & 0x000000ff;
}

quint8 IfsResVersion::GetFileVersion3(void) const
{
	if(IsValid() == false)
		return 0;
	return m_version.Value.dwFileVersionMS & 0x000000ff;
}

quint8 IfsResVersion::GetFileVersion4(void) const
{
	if(IsValid() == false)
		return 0;
	return (m_version.Value.dwFileVersionMS >> 8) & 0x000000ff;
}

quint8 IfsResVersion::GetProductVersion1(void) const
{
	if(IsValid() == false)
		return 0;
	return m_version.Value.dwProductVersionLS & 0x000000ff;
}

quint8 IfsResVersion::GetProductVersion2(void) const
{
	if(IsValid() == false)
		return 0;
	return (m_version.Value.dwProductVersionLS >> 8) & 0x000000ff;
}

quint8 IfsResVersion::GetProductVersion3(void) const
{
	if(IsValid() == false)
		return 0;
	return m_version.Value.dwProductVersionMS & 0x000000ff;
}

quint8 IfsResVersion::GetProductVersion4(void) const
{
	if(IsValid() == false)
		return 0;
	return (m_version.Value.dwProductVersionMS >> 8) & 0x000000ff;
}

QString IfsResVersion::String(const QString & Name, const QString & CodePage /*= ""*/) const
{
	if(IsValid() == false)
		return "";

	// find the String Table
	const QVector<ResVersion_String> * table = NULL;
	for(int boucle = 0; boucle != m_version.stringFileInfo.stringTable.size(); boucle++)
	{
		if(CodePage.length() == 0)
		{
			table = &m_version.stringFileInfo.stringTable[boucle].string;
			break;
		}

		if(m_version.stringFileInfo.stringTable[boucle].szKey == CodePage)
		{
			table = &m_version.stringFileInfo.stringTable[boucle].string;
			break;
		}
	}

	// check if something found
	if(table == NULL)
		return "";

	for(int boucle = 0; boucle != table->size(); boucle++)
	{
		if((*table)[boucle].szKey == Name)
			return (*table)[boucle].Value;
	}

	// nothing found
	return "";
}
