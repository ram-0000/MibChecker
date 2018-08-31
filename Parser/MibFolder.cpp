#include "MibFolder.h"
#include "ParserException.h"
#include <QFileInfo>
#include "Debug.h"
#include "Conf.h"

MibFolder::MibFolder(const QString & input_input_folder)
{
	m_input_mib_folder = input_input_folder;
}

QString MibFolder::BuildMibFile(const QString & mib_filename)
{
	m_mib_name = mib_filename;
	if(MibName().endsWith(Conf::MibExtension()) == true)
		m_mib_name = MibName().left(MibName().length() - Conf::MibExtension().length());

	m_input_mib_filename = InputMibFolder();
	m_input_mib_filename += "/";
	m_input_mib_filename += MibName();
	m_input_mib_filename += Conf::MibExtension();

	m_output_mib_folder = Conf::OutputMibFolder();
	m_output_mib_folder += "/";
	m_output_mib_folder += MibName()[0];

	m_output_mib_filename = OutputMibFolder();
	m_output_mib_filename += "/";
	m_output_mib_filename += MibName();
	m_output_mib_filename += Conf::MibExtension();

	m_output_def_folder = Conf::OutputDefFolder();
	m_output_def_folder += "/";
	m_output_def_folder += MibName()[0];

	m_output_def_filename = OutputDefFolder();
	m_output_def_filename += "/";
	m_output_def_filename += MibName();
	m_output_def_filename += Conf::DefExtension();

	m_output_html_folder = Conf::OutputHtmlFolder();
	m_output_html_folder += "/";
	m_output_html_folder += MibName()[0];

	m_output_html_filename = OutputHtmlFolder();
	m_output_html_filename += "/";
	m_output_html_filename += MibName();
	m_output_html_filename += Conf::HtmlExtension();

	// build filename
	// 1st, we try to find MIB file in local repository
	QFileInfo check_file1(OutputMibFileName());
	if( (check_file1.exists() == true) && (check_file1.isFile() == true) )
		return OutputMibFileName();

	// 2nd, we try to find MIB file in input folder
	QString path = InputMibFolder();
	path += "/";
	path += MibName();
	path += Conf::MibExtension();

	// check if file exists and if it is a real file
	QFileInfo check_file2(path);
	if( (check_file2.exists() == true) && (check_file2.isFile() == true) )
		return path;

	// not found
	throw ParserExceptionFindMibFile(MibName());
}
