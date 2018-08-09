#ifndef ERRORDISPLAY_H
#define ERRORDISPLAY_H

#include <QTextEdit>

class ErrorDisplay
{
public:
	ErrorDisplay(QTextEdit * error);
	~ErrorDisplay(void);

	void Add(const QString & Mib, const QString & Error);
	inline void Clear(void) { m_error->clear(); }

private:
	QTextEdit * m_error;
	QTextCursor * m_cursor;
};

#endif // ERRORDISPLAY_H
