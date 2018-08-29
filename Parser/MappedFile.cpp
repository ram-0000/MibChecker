#include "MappedFile.h"
#include "Debug.h"

MappedFile::MappedFile(const QString & name) throw (ParserException)
{
	// save filename
	m_file_name = name;
	m_file.setFileName(name);
	m_map = nullptr;
	m_size = 0;
	m_already_read = 0;
	m_file_line = 0;

	// open the file
	if(m_file.open(QIODevice::ReadOnly) == false)
	{
		// unable to open filename
		throw ParserExceptionOpenFile(FileName());
		return;
	}

	// map the file
	m_size = m_file.size();
	m_map = m_file.map(0, m_size);
	if(m_map == nullptr)
	{
		// unable to map filename
		throw ParserExceptionMemoryMapping(FileName());
		return;
	}

	m_file_line = 1;
	m_remaining = m_size;
	m_already_read = 0;
	m_current = reinterpret_cast<const char *>(m_map);
}

MappedFile::~MappedFile(void)
{
	if(m_file.isOpen() == true)
	{
		if(m_map != nullptr)
		{
			// unmap the file
			m_file.unmap(m_map);
			m_map = nullptr;
			m_size = 0;
		}
		m_file.close();
	}
}

bool MappedFile::CheckWs(void)
{
	const char * current = String();
	if(current == nullptr)
		return false;
	if(m_remaining < 1)
		return false;
	char carac = *current;

	if( (carac != '\t') &&		// horizontal tabulation	(ascii code  9)
		 (carac != '\n') &&		// line feed					(ascii code 10)
		 (carac != '\v') &&		// vertical tabulation		(ascii code 11)
		 (carac != '\f') &&		// form feed					(ascii code 12)
		 (carac != '\r') &&		// carriage return			(ascii code 13)
		 (carac != ' ') )			// space							(ascii code 32)
		return false;

	// advance 1 carac
	Advance();
	return true;
}

bool MappedFile::CheckCrLf(void)
{
	const char * current = String();
	if(current == nullptr)
		return false;
	if(m_remaining < 1)
		return false;
	char carac= * current;

	if( (carac != 10) &&			// line feed
		 (carac != 13) )			// carriage return
		return false;

	// advance 1 carac
	Advance();
	return true;
}

bool MappedFile::Check(const char * str)
{
	const char * current = String();
	if(current == nullptr)
		return false;

	if(str == nullptr)
		return false;
	size_t len = strlen(str);
	if(len > m_remaining)
		return false;

	// perform compare
	if(memcmp(current, str, len) != 0)
		return false;

	// advance 1 carac
	Advance(len);
	return true;
}

bool MappedFile::CheckRange(const char * range, char * read /*= nullptr*/)
{
	if( (range == nullptr) || (*range == 0) )
		return false;
	int len = strlen(range);
	if(IsEndOfFile() == true)
		return false;

	const char * current = String();
	char carac = *current;
	for(int boucle = 0; boucle != len; boucle++)
	{
		if(carac != range[boucle])
			continue;

		// found
		if(read != nullptr)
			*read = carac;
		Advance();
		return true;
	}

	// not found
	return false;
}

bool MappedFile::CheckRange(char first,
									 char last,
									 char * read)
{
	if(IsEndOfFile() == true)
		return false;
	const char *current = String();
	char carac = *current;

	if(read != nullptr)
		*read = carac;

	if( (carac < first) ||
		 (carac > last) )
		return false;

	// advance 1 carac
	Advance();
	return true;
}

void MappedFile::Advance(quint64 nb /*= 1*/)
{
	for(quint64 boucle = 0; boucle != nb; boucle++)
	{
		// check if something is remaining
		if(m_remaining == 0)
			return;

		// check for end of line
		if(*m_current == '\n')
			m_file_line++;

		// advance internal pointers
		m_remaining--;
		m_already_read++;
		m_current++;
	}
}


bool MappedFile::IsEndOfFile(void) const
{
	if(m_current == nullptr)
		return true;
	if(m_remaining == 0)
		return true;
	return false;
}

char MappedFile::ReadCharacter(void)
{
	char carac = 0;
	if(IsEndOfFile() == true)
		return carac;

	carac = *m_current;
	Advance();

	return carac;
}

QString MappedFile::ReadUntil(const char * until)
{
	QString ret;

	if(until == nullptr)
		return ret;
	size_t len_until = strlen(until);

	for(;;)
	{
		if(IsEndOfFile() == true)
			break;

		if(len_until > m_remaining)
			break;

		// perform compare
		if(memcmp(m_current, until, len_until) == 0)
		{
			Advance(len_until);
			break;
		}

		// read 1 carac
		ret += ReadCharacter();
	}
	return ret;
}
