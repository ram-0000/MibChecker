#ifndef MIBLEXER_H
#define MIBLEXER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include "ParserException.h"
#include "MappedFile.h"

typedef enum {
	// pseudo tokens
	Tok_First,
	Tok_EndOfFile,
	Tok_String,
	Tok_Uppercase,
	Tok_Lowercase,
	Tok_Integer,
	Tok_Binary,
	Tok_Hexadecimal,

	// operator tokens
	Tok_ASSIGN,
	Tok_ELLIPSIS,
	Tok_DOUBLE_DOT,
	Tok_VERTICAL_BAR,
	Tok_SEMI_COLON,
	Tok_COMMA,
	Tok_DOT,
	Tok_LESS,
	Tok_LEFT_BRACE,
	Tok_LEFT_BRACKET,
	Tok_LEFT_PAREN,
	Tok_RIGHT_BRACE,
	Tok_RIGHT_BRACKET,
	Tok_RIGHT_PAREN,

	// keywords
	Tok_ABSENT,
	Tok_ACCESS,
	Tok_AGENT_CAPABILITIES,
	Tok_ANY,
	Tok_APPLICATION,
	Tok_AUGMENTS,
	Tok_BEGIN,
	Tok_BITS,
	Tok_BIT,
	Tok_BOOLEAN,
	Tok_BY,
	Tok_CHOICE,
	Tok_COMPONENTS,
	Tok_COMPONENT,
	Tok_CONTACT_INFO,
	Tok_CREATION_REQUIRES,
	Tok_DEFAULT,
	Tok_DEFINED,
	Tok_DEFINITIONS,
	Tok_DEFVAL,
	Tok_DESCRIPTION,
	Tok_DISPLAY_HINT,
	Tok_END,
	Tok_ENTERPRISE,
	Tok_ENUMERATED,
	Tok_EXPLICIT,
	Tok_EXPORTS,
	Tok_FALSE,
	Tok_FROM,
	Tok_GROUP,
	Tok_IDENTIFIER,
	Tok_IMPLICIT,
	Tok_IMPLIED,
	Tok_IMPORTS,
	Tok_INCLUDES,
	Tok_INDEX,
	Tok_INTEGER,
	Tok_LAST_UPDATED,
	Tok_MACRO,
	Tok_MANDATORY_GROUPS,
	Tok_MAX_ACCESS,
	Tok_MAX,
	Tok_MIN_ACCESS,
	Tok_MIN,
	Tok_MINUS_INFINITY,
	Tok_MODULE_COMPLIANCE,
	Tok_MODULE_CONFORMANCE,
	Tok_MODULE_IDENTITY,
	Tok_MODULE,
	Tok_NOTIFICATION_GROUP,
	Tok_NOTIFICATION_TYPE,
	Tok_NOTIFICATIONS,
	Tok_NULL,
	Tok_OBJECTS,
	Tok_OBJECT_GROUP,
	Tok_OBJECT_IDENTITY,
	Tok_OBJECT_TYPE,
	Tok_OBJECT,
	Tok_OCTET,
	Tok_OF,
	Tok_OPTIONAL,
	Tok_ORGANIZATION,
	Tok_PLUS_INFINITY,
	Tok_PRESENT,
	Tok_PRIVATE,
	Tok_PRODUCT_RELEASE,
	Tok_REAL,
	Tok_REFERENCE,
	Tok_REVISION,
	Tok_SEQUENCE,
	Tok_SET,
	Tok_SIZE,
	Tok_STATUS,
	Tok_STRING,
	Tok_SUPPORTS,
	Tok_SYNTAX,
	Tok_TAGS,
	Tok_TEXTUAL_CONVENTION,
	Tok_TRAP_TYPE,
	Tok_TRUE,
	Tok_UNITS,
	Tok_UNIVERSAL,
	Tok_VARIABLES,
	Tok_VARIATION,
	Tok_WITH,
	Tok_WRITE_SYNTAX,

	Tok_Last
} Token_t;

class MibLexer
{
public:
	MibLexer(const QString & name);
	~MibLexer(void);

	bool Next(int & token, QString & extra_value);

	static const char * Decode(int token);

	inline QString FileName(void) const { return m_file.FileName(); }
	inline int FileLine(void) const { return m_file.FileLine(); }

private:

	typedef struct
	{	int Token;
		const char * Text;
	} Item_t;

	static Item_t m_table[];
	MappedFile m_file;

	void _read_comment_1(void);
	void _read_identifier(QString & str);
	void _read_integer(QString & str);
	bool _check_binary(const QString & input);
	bool _check_hexadecimal(const QString & input);
	bool _check_keyword(int & token);

};

#endif // MIBLEXER_H
