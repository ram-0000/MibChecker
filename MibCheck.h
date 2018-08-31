#ifndef MIBCHECK_H
#define MIBCHECK_H

#include <QString>
#include <QTextStream>
#include <QTemporaryFile>
#include "Parser/MibFolder.h"
#include "Parser/SnmpTree.h"

class MibCheck
{
public:
	MibCheck(void);

	typedef enum { CheckOnly,
						NeverOverwrite,
						OverwriteIfGreaterSize,
						AlwaysOverwrite
					 } OverwriteAlgorithm;

	bool Check(OverwriteAlgorithm Algo, const QString & File);
	inline const QString & Error(void) const { return m_error_message; }

private:

	QString m_error_message;
	QString m_mib_filename;
	QString m_mib_name;
	SnmpTree m_mib_tree;

	void _check(OverwriteAlgorithm Algo, const QString & File);
	void _check_overwrite(OverwriteAlgorithm Algo, const QString & input_mib, const QString & output_mib);
	void _def(const QString & Name);
	void _html(const QString & Name);
	void _print(QTextStream & s,
					const QString & p1 = "",
					const QString & p2 = "",
					const QString & p3 = "",
					const QString & p4 = "",
					const QString & p5 = "",
					const QString & p6 = "",
					const QString & p7 = "",
					const QString & p8 = "");
	void _print_type(QTextStream & s,
						  const SnmpType & type,
						  int indent,
						  int & idx);

	void _remediate(const ParserException & e);
};

#endif // MIBCHECK_H
