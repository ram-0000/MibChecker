#ifndef PARSEREXCEPTION_H
#define PARSEREXCEPTION_H

#include <QString>

class ParserException
{
public:
	static void CheckNull(const void * Pointer);
	static void CheckNullOrEmpty(const void * Pointer);

	typedef enum { NullPointer, EmptyString, FindMibFile, FileWithBom, EmptyFile, OpenFile, MemoryMapping, QuotedString, NotLL1,
						SyntaxError, SymbolNotFound, ShouldNotArrive, PopEmptyStack, CallbackNotFound,
						SnmpValueNotFound, SnmpTypeNotFound, MibInvalidName, CreateFolder, RenameFile, CreateFile,
						FileNotExist, FileAlreadyExist, CurrentFileSmaller, BadExtension
	} ParserException_t;

	ParserException(ParserException_t ident,
						 const char * Format,
						 const QString & p1 = "",
						 const QString & p2 = "",
						 const QString & p3 = "",
						 const QString & p4 = "",
						 const QString & p5 = "",
						 const QString & p6 = "",
						 const QString & p7 = "",
						 const QString & p8 = "",
						 const QString & p9 = "");

	inline const QString & toString(void) const { return m_string; }
	inline ParserException_t Ident(void) const { return m_ident; }

	inline const QString & P1(void) const { return m_p1; }
	inline void P1(const QString & val) { m_p1 = val; }

	inline const QString & P2(void) const { return m_p2; }
	inline void P2(const QString & val) { m_p2 = val; }

protected:

private:
	QString m_string;
	ParserException_t m_ident;
	QString m_p1;
	QString m_p2;

};

class ParserExceptionNullPointer : public ParserException
{
public:
	ParserExceptionNullPointer(void)
		: ParserException(NullPointer, "Bug: Null pointer") { }
};

class ParserExceptionEmptyString : public ParserException
{
public:
	ParserExceptionEmptyString(void)
		: ParserException(EmptyString, "Bug: Empty string") { }
};

class ParserExceptionFindMibFile : public ParserException
{
public:
	inline ParserExceptionFindMibFile(const QString & mib)
		: ParserException(FindMibFile, "Unable to find Mib file %1", mib) { }
};

class ParserExceptionFileWithBom : public ParserException
{
public:
	inline ParserExceptionFileWithBom(const QString & mib)
		: ParserException(FileWithBom, "Mib file %1 has BOM encoding, you MUST remove it manually", mib) { }
};

class ParserExceptionEmptyFile : public ParserException
{
public:
	inline ParserExceptionEmptyFile(const QString & filename)
		: ParserException(EmptyFile, "MIB file %1 is empty", filename) { }
};

class ParserExceptionOpenFile : public ParserException
{
public:
	inline ParserExceptionOpenFile(const QString & filename)
		: ParserException(OpenFile, "Unable to open MIB file %1", filename) { }
};

class ParserExceptionMemoryMapping : public ParserException
{
public:
	ParserExceptionMemoryMapping(const QString & filename)
		: ParserException(MemoryMapping, "Unable to map MIB file %1", filename) { }
};

class ParserExceptionQuotedString : public ParserException
{
public:
	ParserExceptionQuotedString(const QString & filename, int line)
		: ParserException(QuotedString, "Quoted String Error in MIB file %1, line %2",
								filename, QString::number(line)) { }
};

class ParserExceptionNotLL1 : public ParserException
{
public:
	ParserExceptionNotLL1(const QString & filename,
								 int line,
								 const QString & token,
								 const QString & rule1,
								 const QString callstack1,
								 const QString & rule2,
								 const QString callstack2)
	: ParserException(NotLL1, "Bug: MIB File %1(%2), Token '%3', Match rule %4 in %5 and rule %6 in %7",
			 filename, QString::number(line), token, rule1, callstack1, rule2, callstack2) { }
};

class ParserExceptionSyntaxError : public ParserException
{
public:
	ParserExceptionSyntaxError(const QString & filename, int line)
		: ParserException(SyntaxError, "Syntax error in MIB file %1 line %2",
								filename, QString::number(line)) { }
};

class ParserExceptionSymbolNotFound : public ParserException
{
public:
	ParserExceptionSymbolNotFound(const QString & symbol, const QString & rule)
		: ParserException(SymbolNotFound, "Bug: Unable to find symbol '%1' called from rule '%2'", symbol, rule) { }
	ParserExceptionSymbolNotFound(const QString & symbol)
		: ParserException(SymbolNotFound, "Bug: Unable to find symbol '%1'", symbol) { }
};

class ParserExceptionShouldNotArrive : public ParserException
{
public:
	ParserExceptionShouldNotArrive(const QString & file, int line)
		: ParserException(ShouldNotArrive, "Bug: Should not arrive in %1 line %2", file, QString::number(line)) { }
};

class ParserExceptionPopEmptyStack : public ParserException
{
public:
	ParserExceptionPopEmptyStack(void)
		: ParserException(PopEmptyStack, "Bug: Try to pop an empty stack") { }
};

class ParserExceptionCallbackNotFound : public ParserException
{
public:
	ParserExceptionCallbackNotFound(const QString & rule)
		: ParserException(CallbackNotFound, "Bug: Unable to find callback funtion for rule '%1'", rule) { }
};

class ParserExceptionSnmpValueNotFound : public ParserException
{
public:
	ParserExceptionSnmpValueNotFound(const QString & member, const QString & mib, int line)
		: ParserException(SnmpValueNotFound, "OID value %1 (in %2, line %3) is not defined nor imported",
								member, mib, QString::number(line)) { }
	ParserExceptionSnmpValueNotFound(const QString & value)
		: ParserException(SnmpValueNotFound, "Unable to find value %1", value) { }
};

class ParserExceptionSnmpTypeNotFound : public ParserException
{
public:
	ParserExceptionSnmpTypeNotFound(const QString & type,
												const QString & mib,
												int line)
		: ParserException(SnmpTypeNotFound, "Named Type %1 (in %2, line %3) is not defined nor imported",
								type, mib, QString::number(line)) { }
};

class ParserExceptionMibInvalidName : public ParserException
{
public:
	ParserExceptionMibInvalidName(const QString & mib, const QString & module)
		: ParserException(MibInvalidName, "Mibfile %1 and module name %2 are inconsistent", mib, module) { }
};

class ParserExceptionCreateFolder : public ParserException
{
public:
	ParserExceptionCreateFolder(const QString & folder)
		: ParserException(CreateFolder, "Unable to create folder %1", folder) { }
};

class ParserExceptionCreateFile : public ParserException
{
public:
	ParserExceptionCreateFile(const QString & file)
		: ParserException(CreateFile, "Unable to create file %1", file) { }
};

class ParserExceptionFileNotExist : public ParserException
{
public:
	ParserExceptionFileNotExist(const QString & file)
		: ParserException(FileNotExist, "File %1 does not exist", file) { }
};

class ParserExceptionFileAlreadyExist : public ParserException
{
public:
	ParserExceptionFileAlreadyExist(const QString & existing_file, const QString & deleted_file)
		: ParserException(FileAlreadyExist, "File %1 is already existing, deleting file %2", existing_file, deleted_file) { }
};

class ParserExceptionCurrentFileSmaller : public ParserException
{
public:
	ParserExceptionCurrentFileSmaller(const QString & file)
		: ParserException(CurrentFileSmaller, "Current file %1 is smaller", file) { }
};

class ParserExceptionRenameFile : public ParserException
{
public:
	ParserExceptionRenameFile(const QString & file)
		: ParserException(RenameFile, "Unable to rename file %1", file) { }
};

class ParserExceptionBadExtension : public ParserException
{
public:
	ParserExceptionBadExtension(const QString & file)
		: ParserException(BadExtension, "File %1 has wrong extension, you MUST rename it manually", file) { }
};

#endif // PARSEREXCEPTION_H
