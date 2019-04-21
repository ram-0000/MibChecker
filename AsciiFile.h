#ifndef ASCIIFILE_H
#define ASCIIFILE_H

#include <QString>

class AsciiFile
{
public:
	AsciiFile(const QString & filename);
	virtual ~AsciiFile(void);

	bool Check(void);

protected:

private:
	QString m_name;
};

#endif // ASCIIFILE_H
