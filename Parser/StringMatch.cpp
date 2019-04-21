#include "StringMatch.h"
#include "ParserException.h"
#include "SnmpType.h"
#include "SnmpStatus.h"
#include "SnmpAccess.h"
#include "Debug.h"

StringMatch::Array_t StringMatch::m_snmp_access_array[]
{	{ SnmpAccess::AccessNotset					, "not-set" },
	{ SnmpAccess::AccessNotAccessible		, "not-accessible" },
	{ SnmpAccess::AccessAccessibleForNotify, "accessible-for-notify" },
	{ SnmpAccess::AccessReadOnly				, "read-only" },
	{ SnmpAccess::AccessReadWrite				, "read-write" },
	{ SnmpAccess::AccessReadCreate			, "read-create" },
	{ SnmpAccess::AccessNotImplemented		, "not-implemented" },
	{ SnmpAccess::AccessWriteOnly				, "write-only" },
	{ -1												, nullptr } };

StringMatch::Array_t StringMatch::m_snmp_status_array[]
{	{ SnmpStatus::StatusNotset					, "not-set" },
	{ SnmpStatus::StatusCurrent				, "current" },
	{ SnmpStatus::StatusDeprecated			, "deprecated" },
	{ SnmpStatus::StatusObsolete				, "obsolete" },
	{ SnmpStatus::StatusMandatory				, "mandatory" },
	{ SnmpStatus::StatusOptional				, "optional" },
	{ -1												, nullptr } };

StringMatch::Array_t StringMatch::m_snmp_type_array[] =
{	{ SnmpType::TypeNotSet						, "Not set" },
	{ SnmpType::TypeDefined						, "Defined" },
	{ SnmpType::TypeNull							, "NULL" },
	{ SnmpType::TypeBoolean						, "BOOLEAN" },
	{ SnmpType::TypeReal							, "REAL" },
	{ SnmpType::TypeInteger						, "INTEGER" },
	{ SnmpType::TypeObjectIdentifier			, "OBJECT IDENTIFIER" },
	{ SnmpType::TypeOctetString				, "OCTET STRING" },
	{ SnmpType::TypeBitString					, "BIT STRING" },
	{ SnmpType::TypeBits							, "BITS" },
	{ SnmpType::TypeSequence					, "SEQUENCE" },
	{ SnmpType::TypeSequenceOf					, "SEQUENCE OF" },
	{ SnmpType::TypeSet							, "SET" },
	{ SnmpType::TypeSetOf						, "SET OF" },
	{ SnmpType::TypeChoice						, "CHOICE" },
	{ SnmpType::TypeEnumerated					, "ENUMERATED" },
	{ SnmpType::TypeSelection					, "SELECTION" },
	{ SnmpType::TypeAny							, "ANY" },
	{ SnmpType::TypeMacroModuleIdentity		, "MODULE-IDENTITY" },
	{ SnmpType::TypeMacroObjectIdentity		, "OBJECT-IDENTITY" },
	{ SnmpType::TypeMacroObjectType			, "OBJECT-TYPE" },
	{ SnmpType::TypeMacroNotificationType	, "NOTIFICATION-TYPE" },
	{ SnmpType::TypeMacroTextualConvention	, "TEXTUAL-CONVENTION" },
	{ SnmpType::TypeMacroObjectGroup			, "OBJECT-GROUP" },
	{ SnmpType::TypeMacroNotificationGroup	, "NOTIFICATION-GROUP" },
	{ SnmpType::TypeMacroModuleCompliance	, "MODULE-COMPLIANCE" },
	{ SnmpType::TypeMacroAgentCapabilities	, "AGENT-CAPABILITIES" },
	{ SnmpType::TypeMacroTrapType				, "TRAP-TYPE" },
	{ SnmpType::TypeMacroModuleConformance	, "MODULE-CONFORMANCE" },
	{ -1												, nullptr } };

const char * StringMatch::translate(int idx,
												const Array_t * array,
												bool raise_exception)
{
	for(const Array_t * p = array; ; ++p)
	{
		// check if end of table
		if(p->str == nullptr)
		{
			WARNING("Translation for id '%d' not found", idx);
			if(raise_exception == true)
				throw ParserExceptionTextNotFound(idx);
			return nullptr;
		}

		// check if found
		if(p->idx == idx)
			return p->str;
	}
}

int StringMatch::translate(const char * str,
									const Array_t * array,
									bool raise_exception)
{
	for(const Array_t * p = array; ; ++p)
	{
		// check if end of table
		if(p->str == nullptr)
		{
			WARNING("Translation for string '%s' not found", str);
			if(raise_exception == true)
				throw ParserExceptionTextNotFound(str);
			return -1;
		}

		// check if found
		if(strcmp(p->str, str) == 0)
			return p->idx;
	}
}


