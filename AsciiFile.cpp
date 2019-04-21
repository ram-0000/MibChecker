#include "AsciiFile.h"
#include <QFile>
#include "Parser/ParserException.h"

AsciiFile::AsciiFile(const QString & filename)
{
	m_name = filename;
}

AsciiFile::~AsciiFile(void)
{
}

bool AsciiFile::Check(void)
{
	if(QFile::exists(m_name) == false)
		throw ParserExceptionFileNotExist(m_name);

	QFile fd(m_name);
	if(fd.open(QIODevice::ReadOnly) == false)
		throw ParserExceptionOpenFile(m_name);

	int line = 1;
	while(fd.atEnd() == false)
	{
		// read a byte
		char data;
		fd.read(&data, sizeof(data));

		// check greater than 32
		if(data >= 32)
			continue;

		// TAB is allowed
		if(data == 0x09)	// TAB
			continue;

		// LF is UNIX line termination
		if(data == 0x0a)	// LF
			throw ParserExceptionUnixEndOfLine();

		if(data == 0x0d)	// CR
		{
			// read another character
			fd.read(&data, sizeof(data));

			if(data == 0x0a)	// CRLF is a DOS end of line
			{
				line++;
				continue;
			}

			// CR only is a MAC end of line
			throw ParserExceptionMacEndOfLine();
		}

		// other characters are not allowed
		throw ParserExceptionBinaryFile(line);
	}

	// close
	fd.close();
	return true;
}
