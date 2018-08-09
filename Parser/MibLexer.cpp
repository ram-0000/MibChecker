#include "MibLexer.h"
#include <QFile>
#include "Debug.h"

MibLexer::Item_t MibLexer::m_table[] = {

	// pseudo tokens
	{ Tok_First						, "Tok_First" },
	{ Tok_EndOfFile				, "Tok_EndOfFile" },
	{ Tok_String					, "Tok_String" },
	{ Tok_Uppercase				, "Tok_Uppercase" },
	{ Tok_Lowercase				, "Tok_Lowercase" },
	{ Tok_Integer					, "Tok_Integer" },
	{ Tok_Binary					, "Tok_Binary" },
	{ Tok_Hexadecimal				, "Tok_Hexadecimal" },

	// operator tokens
	{ Tok_ASSIGN					, "::=" },
	{ Tok_ELLIPSIS					, "..." },
	{ Tok_DOUBLE_DOT				, ".." },
	{ Tok_VERTICAL_BAR			, "|" },
	{ Tok_SEMI_COLON				, ";" },
	{ Tok_COMMA						, "," },
	{ Tok_DOT						, "." },
	{ Tok_LESS						, "<" },
	{ Tok_LEFT_BRACE				, "{" },
	{ Tok_LEFT_BRACKET			, "[" },
	{ Tok_LEFT_PAREN				, "(" },
	{ Tok_RIGHT_BRACE				, "}" },
	{ Tok_RIGHT_BRACKET			, "]" },
	{ Tok_RIGHT_PAREN				, ")" },

	// keyword tokens
	{ Tok_ABSENT					, "ABSENT" },
	{ Tok_ACCESS					, "ACCESS" },
	{ Tok_AGENT_CAPABILITIES	, "AGENT-CAPABILITIES" },
	{ Tok_ANY						, "ANY" },
	{ Tok_APPLICATION				, "APPLICATION" },
	{ Tok_AUGMENTS					, "AUGMENTS" },
	{ Tok_BEGIN						, "BEGIN" },
	{ Tok_BITS						, "BITS" },			// WARNING, BITS before BIT
	{ Tok_BIT						, "BIT" },
	{ Tok_BOOLEAN					, "BOOLEAN" },
	{ Tok_BY							, "BY" },
	{ Tok_CHOICE					, "CHOICE" },
	{ Tok_COMPONENTS				, "COMPONENTS" },	// WARNING, COMPONENTS before COMPONENT
	{ Tok_COMPONENT				, "COMPONENT" },
	{ Tok_CONTACT_INFO			, "CONTACT-INFO" },
	{ Tok_CREATION_REQUIRES		, "CREATION-REQUIRES" },
	{ Tok_DEFAULT					, "DEFAULT" },
	{ Tok_DEFINED					, "DEFINED" },
	{ Tok_DEFINITIONS				, "DEFINITIONS" },
	{ Tok_DEFVAL					, "DEFVAL" },
	{ Tok_DESCRIPTION				, "DESCRIPTION" },
	{ Tok_DISPLAY_HINT			, "DISPLAY-HINT" },
	{ Tok_END						, "END" },
	{ Tok_ENTERPRISE				, "ENTERPRISE" },
	{ Tok_ENUMERATED				, "ENUMERATED" },
	{ Tok_EXPLICIT					, "EXPLICIT" },
	{ Tok_EXPORTS					, "EXPORTS" },
	{ Tok_FALSE						, "FALSE" },
	{ Tok_FROM						, "FROM" },
	{ Tok_GROUP						, "GROUP" },
	{ Tok_IDENTIFIER				, "IDENTIFIER" },
	{ Tok_IMPLICIT					, "IMPLICIT" },
	{ Tok_IMPLIED					, "IMPLIED" },
	{ Tok_IMPORTS					, "IMPORTS" },
	{ Tok_INCLUDES					, "INCLUDES" },
	{ Tok_INDEX						, "INDEX" },
	{ Tok_INTEGER					, "INTEGER" },
	{ Tok_LAST_UPDATED			, "LAST-UPDATED" },
	{ Tok_MACRO						, "MACRO" },
	{ Tok_MANDATORY_GROUPS		, "MANDATORY-GROUPS" },
	{ Tok_MAX_ACCESS				, "MAX-ACCESS" },	// WARNING, MAX-ACCESS before MAX
	{ Tok_MAX						, "MAX" },
	{ Tok_MIN_ACCESS				, "MIN-ACCESS" },	// WARNING, MIN-ACCESS before MIN
	{ Tok_MIN						, "MIN" },
	{ Tok_MINUS_INFINITY			, "MINUS-INFINITY" },
	{ Tok_MODULE_COMPLIANCE		, "MODULE-COMPLIANCE" },	// WARNING, MODULE-COMPLIANCE & MODULE-IDENTITY before MODULE
	{ Tok_MODULE_IDENTITY		, "MODULE-IDENTITY" },
	{ Tok_MODULE					, "MODULE" },
	{ Tok_NOTIFICATION_GROUP	, "NOTIFICATION-GROUP" },
	{ Tok_NOTIFICATION_TYPE		, "NOTIFICATION-TYPE" },
	{ Tok_NOTIFICATIONS			, "NOTIFICATIONS" },
	{ Tok_NULL						, "NULL" },
	{ Tok_OBJECTS					, "OBJECTS" },
	{ Tok_OBJECT_GROUP			, "OBJECT-GROUP" },
	{ Tok_OBJECT_IDENTITY		, "OBJECT-IDENTITY" },
	{ Tok_OBJECT_TYPE				, "OBJECT-TYPE" },
	{ Tok_OBJECT					, "OBJECT" },
	{ Tok_OCTET						, "OCTET" },
	{ Tok_OF							, "OF" },
	{ Tok_OPTIONAL					, "OPTIONAL" },
	{ Tok_ORGANIZATION			, "ORGANIZATION" },
	{ Tok_PLUS_INFINITY			, "PLUS-INFINITY" },
	{ Tok_PRESENT					, "PRESENT" },
	{ Tok_PRIVATE					, "PRIVATE" },
	{ Tok_PRODUCT_RELEASE		, "PRODUCT-RELEASE" },
	{ Tok_REAL						, "REAL" },
	{ Tok_REFERENCE				, "REFERENCE" },
	{ Tok_REVISION					, "REVISION" },
	{ Tok_SEQUENCE					, "SEQUENCE" },
	{ Tok_SET						, "SET" },
	{ Tok_SIZE						, "SIZE" },
	{ Tok_STATUS					, "STATUS" },
	{ Tok_STRING					, "STRING" },
	{ Tok_SUPPORTS					, "SUPPORTS" },
	{ Tok_SYNTAX					, "SYNTAX" },
	{ Tok_TAGS						, "TAGS" },
	{ Tok_TEXTUAL_CONVENTION	, "TEXTUAL-CONVENTION" },
	{ Tok_TRAP_TYPE				, "TRAP-TYPE" },
	{ Tok_TRUE						, "TRUE" },
	{ Tok_UNITS						, "UNITS" },
	{ Tok_UNIVERSAL				, "UNIVERSAL" },
	{ Tok_VARIABLES				, "VARIABLES" },
	{ Tok_VARIATION				, "VARIATION" },
	{ Tok_WITH						, "WITH" },
	{ Tok_WRITE_SYNTAX			, "WRITE-SYNTAX" },

	{ Tok_Last						, "Tok_Last" },

};


MibLexer::MibLexer(const QString & name) throw (ParserException)
	: m_file(name)
{
}

MibLexer::~MibLexer(void)
{
}

const char * MibLexer::Decode(int token)
{
	for(int boucle = 0; boucle != sizeof(m_table) / sizeof(m_table[0]); boucle++)
	{
		Item_t * cur = &m_table[boucle];
		if(token == cur->Token)
			return cur->Text;
	}

	// unknown token
	return "Unknown token";
}

bool MibLexer::Next(int & token, QString & extra_value) throw (ParserException)
{
	// default value
	extra_value = "";

	// check if file is open
	if(m_file.IsOpen() == false)
	{
		throw ParserExceptionOpenFile(FileName());
		token = Tok_EndOfFile;
		return false;
	}

	for(;;)
	{
		// check for end of file ?
		if(m_file.IsEndOfFile() == true)
		{
			token = Tok_EndOfFile;
			//DEBUG("Lexer %d: End of file", (int)FileLine());
			return false;
		}

		// check if white space
		if(m_file.CheckWs() == true)
			continue;

		// check if comment
		if(m_file.Check("--") == true)
		{
			// read until end of comment
			_read_comment_1();
			//DEBUG("Lexer %d: End of comment one line", (int)FileLine());
			continue;
		}

		// check if comment
		if(m_file.Check("/*") == true)
		{
			// read until end of comment
			m_file.ReadUntil("*/");
			//DEBUG("Lexer %d: End of multi line comment", (int)FileLine());
			continue;
		}

		// checking if keyword
		if(_check_keyword(token) == true)
		{
			// get string of token
			extra_value = Decode(token);
			int new_token = 0;
			char first_carac = extra_value[0].toLatin1();
			if( (first_carac >= 'A') && (first_carac <= 'Z') )
				new_token = Tok_Uppercase;
			else if( (first_carac >= 'a') && (first_carac <= 'z') )
				new_token = Tok_Lowercase;
			else
			{
				// it cannot be start of identifier, it is a token
				//DEBUG("Lexer %d: token", (int)FileLine());
				return true;
			}

			// if next character is in [A-Z][a-z][0-9][-], it is not a token but an an identifier
			char next = m_file.Carac();
			bool identifier = false;
			if( (next >= 'A') && (next <= 'Z') )
				identifier = true;
			if( (next >= 'a') && (next <= 'z') )
				identifier = true;
			if( (next >= '0') && (next <= '9') )
				identifier = true;
			if(next == '-')
				identifier = true;
			if(identifier == false)
			{
				//DEBUG("Lexer %d: token", (int)FileLine());
				return true;
			}

			// it is an identifier, get the begin of the string
			extra_value = Decode(token);
			_read_identifier(extra_value);
			token = new_token;
			return true;
		}

		// check if double quoted string
		if(m_file.Check("\"") == true)
		{
			// read until end of string
			extra_value = m_file.ReadUntil("\"");
			token = Tok_String;
			//DEBUG("Lexer %d: String token", (int)FileLine());
			return true;
		}

		// check if quoted binary or quoted hexadecimal
		if(m_file.Check("'") == true)
		{
			// read until end of string
			extra_value = m_file.ReadUntil("'");

			// check if binary quoted string
			if( (m_file.CheckRange("Bb") == true) &&
				 (_check_binary(extra_value) == true) )
			{
				token = Tok_Binary;
				//DEBUG("Lexer %d: Binary token", (int)FileLine());
				return true;
			}

			// check if hexadecimal quoted string
			if( (m_file.CheckRange("Hh") == true) &&
				(_check_hexadecimal(extra_value) == true) )
			{
				token = Tok_Hexadecimal;
				//DEBUG("Lexer %d: Hexadecimal token", (int)FileLine());
				return true;
			}

			// unknown quoted string, don't know what to do
			throw ParserExceptionQuotedString(m_file.FileName(), m_file.FileLine());
			token = Tok_EndOfFile;
			return false;
		}

		// check if identifier starting with upper letter
		char carac;
		if(m_file.CheckRange('A', 'Z', &carac) == true)
		{
			// read until end of identifier
			extra_value += carac;
			_read_identifier(extra_value);
			token = Tok_Uppercase;
			//DEBUG("Lexer %d: Uppercase token", (int)FileLine());
			return true;
		}

		// check if identifier starting with lower letter
		if(m_file.CheckRange('a', 'z', &carac) == true)
		{
			// read until end of identifier
			extra_value += carac;
			_read_identifier(extra_value);
			token = Tok_Lowercase;
			//DEBUG("Lexer %d: Lowercase token", (int)FileLine());
			return true;
		}

		// check if integer starting with digit
		if( (m_file.CheckRange('0', '9', &carac) == true) ||
			 (m_file.CheckRange("+-", &carac) == true) )
		{
			extra_value += carac;
			_read_integer(extra_value);
			token = Tok_Integer;
			//DEBUG("Lexer %d: Integer token ='%s'", (int)FileLine(), extra_value.toLatin1().constData());
			return true;
		}

		// don't know what to do
		throw ParserExceptionSyntaxError(m_file.FileName(), m_file.FileLine());
		token = Tok_EndOfFile;
		return false;
	}
}

void MibLexer::_read_comment_1(void)
{
	for(;;)
	{
		if(m_file.IsEndOfFile() == true)
			return;

		if(m_file.Check("--") == true)
			return;
		if(m_file.CheckCrLf() == true)
			return;

		// advance 1 carac
		m_file.Advance();
	}
}

void MibLexer::_read_identifier(QString & str)
{
	for(;;)
	{
		// end of file ?
		if(m_file.IsEndOfFile() == true)
			return;

		char carac;
		// A à Z (LATIN CAPITAL LETTER A to LATIN CAPITAL LETTER Z)
		if(m_file.CheckRange('A', 'Z', &carac) == true)
		{
			str += carac;
			continue;
		}

		// a à z (LATIN SMALL LETTER A to LATIN SMALL LETTER Z)
		if(m_file.CheckRange('a', 'z', &carac) == true)
		{
			str += carac;
			continue;
		}

		// 0 à 9 (DIGIT ZERO to DIGIT 9)
		if(m_file.CheckRange('0', '9', &carac) == true)
		{
			str += carac;
			continue;
		}

		// - (HYPHEN-MINUS)
		if(m_file.Check("-") == true)
		{
			str += "-";
			continue;
		}

		// not allowed character, end of identifier
		return;
	}
}

void MibLexer::_read_integer(QString & str)
{
	for(;;)
	{
		// end of file ?
		if(m_file.IsEndOfFile() == true)
			return;

		// ignore white spaces
		if(m_file.CheckWs() == true)
			return;

		// is it a digit (ideally, first digit should not be '0'
		char carac;
		if(m_file.CheckRange('0', '9', &carac) == false)
			return;

		// add digit
		str += carac;
	}
}

bool MibLexer::_check_binary(const QString & input)
{
	for(int boucle = 0; boucle != input.length(); boucle++)
	{
		char carac = input.at(boucle).toLatin1();
		switch(carac)
		{
		case '0' : case '1' :
			break;

		// invalid binary string
		default : return false;
		}
	}
	return true;
}

bool MibLexer::_check_hexadecimal(const QString & input)
{
	for(int boucle = 0; boucle != input.length(); boucle++)
	{
		char carac = input.at(boucle).toLatin1();
		switch(carac)
		{
		case '0' : case '1' : case '2' : case '3' : case '4' :
		case '5' : case '6' : case '7' : case '8' : case '9' :
		case 'a' : case 'b' : case 'c' : case 'd' : case 'e' : case 'f' :
		case 'A' : case 'B' : case 'C' : case 'D' : case 'E' : case 'F' :
			break;

		// invalid hexadecimal string
		default : return false;
		}
	}
	return true;
}

bool MibLexer::_check_keyword(int & token)
{
	for(int boucle = 0; boucle != sizeof(m_table) / sizeof(m_table[0]); boucle++)
	{
		const Item_t & cur = m_table[boucle];
		if(m_file.Check(cur.Text) == true)
		{
			token = cur.Token;
			return true;
		}
	}
	return false;
}
