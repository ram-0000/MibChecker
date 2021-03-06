#include "MibCheck.h"
#include "Debug.h"
#include "Conf.h"
#include <QDir>
#include <QDateTime>
#include "AsciiFile.h"

// TODO : refaire le greedy

MibCheck::MibCheck(void)
{
}

void MibCheck::_check(OverwriteAlgorithm Algo, const QString & File)
{
	// record filename
	m_mib_filename = File;

	if(m_mib_filename.contains(".") == true)
		throw ParserExceptionBadExtension(m_mib_filename);

	MibFolder folder_manager(Conf::InputFolder());
	QString input_mib = folder_manager.BuildMibFile(m_mib_filename);
	QString output_mib = folder_manager.OutputMibFileName();
	m_mib_name = folder_manager.MibName();
	input_mib = folder_manager.InputMibFileName();

	// remove read only flag (if set)
	QFile::setPermissions(input_mib,
									QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner
								 | QFile::ReadUser | QFile::WriteUser | QFile::ExeUser
								 | QFile::ReadGroup | QFile::WriteGroup | QFile::ExeGroup
								 | QFile::ReadOther | QFile::WriteOther | QFile::ExeOther);

	// check if file is ASCII
	AsciiFile af(input_mib);
	if(af.Check() == false)
		return;

	// check if overwrite is possible
	_check_overwrite(Algo, input_mib, output_mib);

	// load SNMP tree
	MibFolder folder_manager2(Conf::InputFolder());
	m_mib_tree.Load(folder_manager2, m_mib_filename);
	//m_mib_tree.Dump();

	// check if files must be generated
	if(Algo == CheckOnly)
		return;

	// check if overwrite is possible
	_check_overwrite(Algo, input_mib, output_mib);

	// build full mib file name
	QDir dir;

	// generate included files
	if(dir.mkpath(folder_manager.OutputIncFolder()) == false)
		throw ParserExceptionCreateFolder(folder_manager.OutputIncFolder());
	_inc();

	// generate def files
	if(dir.mkpath(folder_manager.OutputDefFolder()) == false)
		throw ParserExceptionCreateFolder(folder_manager.OutputDefFolder());
	QFile::remove(folder_manager.OutputDefFileName());
	_def(folder_manager.OutputDefFileName());

	// generate html files
	if(dir.mkpath(folder_manager.OutputHtmlFolder()) == false)
		throw ParserExceptionCreateFolder(folder_manager.OutputHtmlFolder());
	QFile::remove(folder_manager.OutputHtmlFileName());
	_html(folder_manager.OutputHtmlFileName());

	// rename mib file
	if(dir.mkpath(folder_manager.OutputMibFolder()) == false)
		throw ParserExceptionCreateFolder(folder_manager.OutputMibFolder());
	QFile::remove(folder_manager.OutputMibFileName());
	if(QFile::rename(input_mib, folder_manager.OutputMibFileName()) == false)
		throw ParserExceptionRenameFile(folder_manager.OutputMibFileName());
}

bool MibCheck::Check(OverwriteAlgorithm Algo, const QString & File)
{
	try
	{
		m_mib_tree.clear();
		_check(Algo, File);
		return true;
	}
	catch(const ParserException & e)
	{
		DEBUG("%s", e.toString().toLatin1().constData());
		m_error_message = e.toString();
		_remediate(e);
		return false;
	}
}

void MibCheck::_remediate(const ParserException & e)
{
	if(e.Ident() == ParserException::FileAlreadyExist)
	{
		// remove file
		// P2 = input file
		QFile::remove(e.P2());
		return;
	}

	if(e.Ident() == ParserException::BadExtension)
	{
		// rename input file to have correct extension
		QString old_name = Conf::InputFolder() + "/" + e.P1();
		QString new_name = old_name;

		// remove wrong extension
		int pos = new_name.lastIndexOf('.');
		if(pos != -1)
			new_name.remove(pos, new_name.length());
		new_name += Conf::MibExtension();
		QFile::rename(old_name, new_name);
		return;
	}

	if(e.Ident() == ParserException::CurrentFileSmaller)
	{
		// remove input file
		// P1 = input file
		QFile::remove(e.P1());
		return;
	}

	if(e.Ident() == ParserException::MibInvalidName)
	{
		// rename input file to reflect MIB module name
		// P1 is input file module name
		// P2 is module name as read in the file itself
		QString old_name = Conf::InputFolder() + "/" + e.P1() + Conf::MibExtension();
		QString new_name = Conf::InputFolder() + "/" + e.P2() + Conf::MibExtension();
		QFile::rename(old_name, new_name);
		return;
	}
}

void MibCheck::_inc(void)
{
	for(const auto & type : m_mib_tree.m_global_types)
	{
		// skip entry if file is selected by user
		if(type.isUserChoice() == false)
			_create_inc_file(type.MibModule());
	}

	for(const auto & value : m_mib_tree.m_global_values)
	{
		// skip entry if file is selected by user
		if(value.isUserChoice() == false)
			_create_inc_file(value.MibModule());
	}
}

void MibCheck::_create_inc_file(const QString & module)
{
	// some times, module may be empty
	if(module.length() == 0)
		return;

	// build path name
	QString path = Conf::OutputIncFolder();
	path += "/";
	path += module[0];
	path += "/";
	path += module;
	path += Conf::IncExtension();

	// create file
	if(QFile::exists(path) == false)
	{
		DEBUG("Creating included file %s", path.toLatin1().constData());
		QFile file(path);
		file.open(QIODevice::WriteOnly | QIODevice::Truncate);
		file.close();

		// check that file is created
		if(QFile::exists(path) == false)
			WARNING("Unable to create file %s", path.toLatin1().constData());
	}
	else
	{
		//DEBUG("Included file %s is already existing", path.toLatin1().constData());
	}
}

void MibCheck::_def(const QString & Name)
{
	// create filename
	QFile file(Name);
	if(file.open(QIODevice::WriteOnly) == false)
		throw ParserExceptionCreateFile(Name);
	QTextStream s(&file);
	bool first = false;

	// write all types
	for(const auto & type : m_mib_tree.m_global_types)
	{
		// skip entry if file is not selected by user
		if(type.isUserChoice() == false)
			continue;

		// to handle CRLF at end of line but not at end of last line
		if(first == true) s << "\n";

		// write something like this "89:NodeIfConfEntry:"
		s << type.MibLine() << ":" << type.Name() << ":";
		first = true;
	}

	// write all values
	for(const auto & value : m_mib_tree.m_global_values)
	{
		// skip entry if file is not selected by user
		if(value.isUserChoice() == false)
			continue;

		// to handle CRLF at end of line but not at end of last line
		if(first == true) s << "\n";

		// write something like this "38:necProductDepend:1.3.6.1.4.1.119.2.3"
		s << value.MibLine() << ":" << value.Name() << ":" << value.m_oid.toString();
		first = true;
	}

	// close file
	file.close();
}

void MibCheck::_html(const QString & Name)
{
	// create filename
	QFile file(Name);
	if(file.open(QIODevice::WriteOnly) == false)
		throw ParserExceptionCreateFile(Name);
	QTextStream s(&file);

	_print(s, "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"");
	_print(s, "\"http://www.w3.org/TR/html4/loose.dtd\">");

	_print(s, "<?php");
	_print(s, "include (\"../../Function.php\");", "\n");
	_print(s, "printf(\"<html>\\n\");", "\n");

	_print(s, "header_write(\"", m_mib_name, "\");", "\n");

	_print(s, "body_write_begin(\"", m_mib_name, "\");", "\n");

	// used modules first
	_print(s, "$id=0;");
	QVector<QString> list_import = m_mib_tree.m_import_manager.GetImportedModules(m_mib_filename);
	for(const auto & item : list_import)
		_print(s, "body_imported_module($id++, \"", item, "\");");
	_print(s, "body_imported_module_end($id);", "\n");

	// imported symbols
	_print(s, "$id=0;");
	QVector<QString> list_symbol = m_mib_tree.m_import_manager.GetImportedSymbol(m_mib_filename);
	for(const auto & item : list_symbol)
		_print(s, "body_imported_symbol($id++, \"", item, "\");");
	_print(s, "body_imported_symbol_end($id);", "\n");

	// defined types
	_print(s, "$id=0;");
	for(const auto & type : m_mib_tree.m_global_types)
	{
		if(type.isUserChoice() == false)
			continue;

		_print(s, "$table=array();");
		int idx = 0;
		_print_type(s, type, 0, idx);
		_print(s,
				 "body_defined_oid($id++, \"\", \"",
				 m_mib_filename + "::" + type.Name(),
				 "\", \"",
				 type.Description(),
				 "\", $table);",
				 "\n");
	}

	// defined values
	_print(s, "$id=0;");
	for(const auto & value : m_mib_tree.m_global_values)
	{
		if(value.isUserChoice() == false)
			continue;

		_print(s, "$table=array();");
		int idx = 0;
		_print_type(s, value.m_type, 0, idx);
		_print(s, "body_defined_oid($id++, \"",
				 value.m_oid.toString(),
				 "\", \"",
				 m_mib_filename + "::" + value.Name(),
				 "\", \"",
				 value.m_type.Description(),
				 "\", $table);",
				 "\n");
	}
	_print(s, "body_write_end();", "\n");
	_print(s, "printf(\"</html>\\n\");", "\n");
	_print(s, "?>");

	// close file
	file.close();
}

void MibCheck::_print_type(QTextStream & s,
									const SnmpType & type,
									int indent,
									int & idx)
{
	QString str = type.TypeStr();
	if(type.isTypeDefined() == true)
	{
		str = m_mib_tree.m_import_manager.GetImportedFrom(type.getTypeDefined());
		if(str.length() == 0)
			str = m_mib_name;
		str += "::";
		str += type.getTypeDefined();
	}

	_print(s, "$table[", QString::number(idx), "]['Level']=", QString::number(indent), ";");
	_print(s, "$table[", QString::number(idx), "]['Name']=\"", str, "\";");

	// do not write constraint if empty
	if(type.getTypeExtended().isEmpty() == false)
		_print(s, "$table[", QString::number(idx), "]['Constraint']=\"", type.getTypeExtended(), "\";");

	// do not write name when indent is 0 because it is already written
	QString name = "";
	if(indent != 0)
		name = type.Name();

	// do not write alias if empty
	if(name.isEmpty() == false)
		_print(s, "$table[", QString::number(idx), "]['Alias']=\"", name, "\";");
	idx++;

	// display compound part
	const QVector<SnmpType> & list_compound = type.getCompound();
	for(const auto & compound : list_compound)
		_print_type(s, compound, indent + 1, idx);
}

void MibCheck::_print(QTextStream & s,
							 const QString & p1 /*= ""*/,
							 const QString & p2 /*= ""*/,
							 const QString & p3 /*= ""*/,
							 const QString & p4 /*= ""*/,
							 const QString & p5 /*= ""*/,
							 const QString & p6 /*= ""*/,
							 const QString & p7 /*= ""*/,
							 const QString & p8 /*= ""*/)
{
	if(p1.length() != 0) s << p1;
	if(p2.length() != 0) s << p2;
	if(p3.length() != 0) s << p3;
	if(p4.length() != 0) s << p4;
	if(p5.length() != 0) s << p5;
	if(p6.length() != 0) s << p6;
	if(p7.length() != 0) s << p7;
	if(p8.length() != 0) s << p8;
	s << "\n";
}

void MibCheck::_check_overwrite(OverwriteAlgorithm Algo,
										  const QString & input_mib,
										  const QString & output_mib)
{
	// check that input mib file is existing
	QFile input(input_mib);
	if(input.exists() == false)
		throw ParserExceptionFileNotExist(input_mib);

	// check if output file is existing
	QFile output(output_mib);
	if(output.exists() == false)
	{
		// output file does not exist, no problem
		return;
	}

	// if check only, it is authorized
	if(Algo == CheckOnly)
		return;

	// output file is existing, check with algorithm
	if(Algo == AlwaysOverwrite)
		return;

	if(Algo == NeverOverwrite)
		throw ParserExceptionFileAlreadyExist(output_mib, input_mib);

	if(Algo == OverwriteIfGreaterSize)
	{
		qint64 input_size = input.size();
		qint64 output_size = output.size();
		if(input_size > output_size)
			return;
		throw ParserExceptionCurrentFileSmaller(input_mib);
	}

	// unknow algorithm
	throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);
}
