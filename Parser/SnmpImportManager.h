#ifndef SNMPIMPORTMANAGER_H
#define SNMPIMPORTMANAGER_H

#include <QString>
#include <QVector>
#include <QStringList>

class SnmpImportManager
{
public:
	SnmpImportManager(void);

	void clear(void);

	void SetCurrentModule(const QString & mib_module);

	void AddImportedSymbol(const QString & symbol);
	void AddImportedModule(const QString & import);

	bool CheckSymbolImported(const QString & module, const QString & symbol) const;
	QVector<QString> GetImportedModules(const QString & module) const;
	QVector<QString> GetImportedSymbol(const QString & module) const;
	QString GetImportedFrom(const QString & symbol) const;
	QString GetModuleToLoad(void) const;

private:
	typedef struct { QString m_mib_module; QString m_mib_import_from; QString m_symbol; } item_symbol_t;
	QVector<item_symbol_t> m_tbl_symbol;

	typedef struct { QString m_mib_module; bool m_loaded; } item_module_t;
	QVector<item_module_t> m_tbl_module;

	QString m_current_module;
};
#endif // SNMPIMPORTMANAGER_H
