#include "SnmpImportManager.h"
#include "ParserException.h"
#include "Debug.h"

SnmpImportManager::SnmpImportManager(void)
{
	clear();
}

void SnmpImportManager::clear(void)
{
	m_tbl_symbol.clear();
	m_current_module.clear();
	m_tbl_module.clear();
}

void SnmpImportManager::SetCurrentModule(const QString & mib_module)
{
	if(mib_module.contains(".") == true)
		throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);

	//DEBUG("SetCurrentModule(%s)", mib_module.toLatin1().constData());
	m_current_module = mib_module;

	// check if module is present in module to import
	for(auto & item : m_tbl_module)
	{
		if(item.m_mib_module == mib_module)
		{
			item.m_loaded = true;
			break;
		}
	}
}

void SnmpImportManager::AddImportedSymbol(const QString & symbol)
{
	//DEBUG("AddImportedSymbol(%s)", symbol.toLatin1().constData());
	item_symbol_t item;
	item.m_mib_module = m_current_module;
	item.m_mib_import_from = ""; // not yet known
	item.m_symbol = symbol;
	m_tbl_symbol.push_back(item);
}

void SnmpImportManager::AddImportedModule(const QString & import)
{
	if(import.contains(".") == true)
		throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);

	// update empty import items
	//DEBUG("AddImportedModule(%s)", import.toLatin1().constData());
	for(auto & item : m_tbl_symbol)
	{
		if(item.m_mib_import_from.length() == 0)
			item.m_mib_import_from = import;
	}

	// check if module is present in module to import
	for(const auto & item : m_tbl_module)
	{
		if(item.m_mib_module == import)
			return;
	}

	// not found, add module
	item_module_t item;
	item.m_mib_module = import;
	item.m_loaded = false;
	m_tbl_module.push_back(item);
}

bool SnmpImportManager::CheckSymbolImported(const QString & module, const QString & symbol) const
{
	if(module.contains(".") == true)
		throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);

	for(const auto & item : m_tbl_symbol)
	{
		if( (item.m_mib_module == module) && (item.m_symbol == symbol) )
			return true;
	}

	// not found
	return false;
}

QString SnmpImportManager::GetModuleToLoad(void) const
{
	// for every module
	for(const auto & item : m_tbl_module)
	{
		// check if they are loaded
		if(item.m_loaded == false)
			return item.m_mib_module;
	}

	// no module to load
	return "";
}

QVector<QString> SnmpImportManager::GetImportedModules(const QString & module) const
{
	if(module.contains(".") == true)
		throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);

	QVector<QString> ret;
	for(const auto & item : m_tbl_symbol)
	{
		if(item.m_mib_module != module)
			continue;

		// if already in returned list, skip it
		if(ret.contains(item.m_mib_import_from) == true)
			continue;

		// add module to retuned list
		ret.push_back(item.m_mib_import_from);
	}

	// finished
	return ret;
}

QVector<QString> SnmpImportManager::GetImportedSymbol(const QString & module) const
{
	if(module.contains(".") == true)
		throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);

	QVector<QString> ret;
	for(const auto & item : m_tbl_symbol)
	{
		if(item.m_mib_module != module)
			continue;

		QString str = item.m_mib_import_from + "::" + item.m_symbol;

		// if already in returned list, skip it
		if(ret.contains(str) == true)
			continue;

		// add module to retuned list
		ret.push_back(str);
	}

	// finished
	return ret;
}

QString SnmpImportManager::GetImportedFrom(const QString & symbol) const
{
	for(const auto & item : m_tbl_symbol)
	{
		if(item.m_symbol == symbol)
			return item.m_mib_import_from;
	}
	return "";
}
