#ifndef MIBFOLDER_H
#define MIBFOLDER_H

#include <QString>

class MibFolder
{
public:
	MibFolder(const QString & input_input_folder);

	QString BuildMibFile(const QString & mib_filename);

	inline const QString MibName(void) { return m_mib_name; }
	inline const QString & InputMibFolder(void) const { return m_input_mib_folder; }
	inline const QString & OutputMibFolder(void) const { return m_output_mib_folder; }
	inline const QString & OutputMibFileName(void) const { return m_output_mib_filename; }
	inline const QString & OutputDefFolder(void) const { return m_output_def_folder; }
	inline const QString & OutputDefFileName(void) const { return m_output_def_filename; }
	inline const QString & OutputHtmlFolder(void) const { return m_output_html_folder; }
	inline const QString & OutputHtmlFileName(void) const { return m_output_html_filename; }

private:

	QString m_mib_name;
	QString m_input_mib_folder;
	QString m_output_mib_folder;
	QString m_output_mib_filename;
	QString m_output_def_folder;
	QString m_output_def_filename;
	QString m_output_html_folder;
	QString m_output_html_filename;
};

#endif // MIBFOLDER_H
