#ifndef MAPPEDFILE_H
#define MAPPEDFILE_H

#include <QString>
#include <QFile>
#include <QFileInfo>
#include "ParserException.h"

class MappedFile
{
public:
	MappedFile(const QString & name);
	~MappedFile(void);

	inline bool IsOpen(void) const { return m_file.isOpen(); }

	bool Check(const char * str);
	bool CheckWs(void);
	bool CheckCrLf(void);
	bool CheckRange(char first, char last, char * read);
	bool CheckRange(const char * range, char * read = nullptr);

	bool IsEndOfFile(void) const;

	inline QString FileName(void) const { return m_file_name.completeBaseName(); }
	inline int FileLine(void) const { return m_file_line; }

	void Advance(quint64 nb = 1);

	char ReadCharacter(void);
	QString ReadUntil(const char *until);
	inline char Carac(void) const { return *m_current; }

private:

	QFileInfo m_file_name;
	QString m_folder_name;
	int m_file_line;

	QFile m_file;
	qint64 m_size;
	uchar * m_map;

	const char * m_current;
	qint64 m_remaining;
	qint64 m_already_read;

	inline const char * String(void) const { return m_current; }

};

#endif // MAPPEDFILE_H
