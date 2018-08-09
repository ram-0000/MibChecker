#include "ErrorDisplay.h"

ErrorDisplay::ErrorDisplay(QTextEdit * error)
{
	m_error = error;

	// declare a virtual cursor for QTextEdit
	m_cursor = new QTextCursor(m_error->textCursor());
}

ErrorDisplay::~ErrorDisplay(void)
{
	if(m_cursor != NULL)
		delete m_cursor;
}


void ErrorDisplay::Add(const QString & Mib, const QString & Error)
{
	// set virtual cursor at the end of QTextEdit
	m_cursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);

	// insert the text
	QString str = Mib + ": " + Error + "\n";
	m_cursor->insertText(str);

	// set the cursor visible
	m_error->ensureCursorVisible();
}

