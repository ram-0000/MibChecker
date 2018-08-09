grammar asn ;

tokens
{
	Tok_DOT						= '.' ;
	Tok_COMMA					= ',' ;
	Tok_SEMI_COLON				= ';' ;
	Tok_LEFT_PAREN				= '(' ;
	Tok_RIGHT_PAREN			= ')' ;
	Tok_LEFT_BRACE				= '{' ;
	Tok_RIGHT_BRACE			= '}' ;
	Tok_LEFT_BRACKET			= '[' ;
	Tok_RIGHT_BRACKET			= ']' ;
	Tok_LESS						= '<' ;
	Tok_VERTICAL_BAR			= '|' ;
	Tok_DOUBLE_DOT				= '..' ;
	Tok_ELLIPSIS				= '...' ;
	Tok_ASSIGN					= '::=' ;

	Tok_ABSENT					= 'ABSENT' ;
	Tok_ACCESS					= 'ACCESS' ;
	Tok_AGENT_CAPABILITIES	= 'AGENT-CAPABILITIES' ;
	Tok_ANY						= 'ANY' ;
	Tok_APPLICATION			= 'APPLICATION' ;
	Tok_AUGMENTS				= 'AUGMENTS' ;
	Tok_BEGIN					= 'BEGIN' ;
	Tok_BIT						= 'BIT' ;
	Tok_BITS						= 'BITS' ;
	Tok_BOOLEAN					= 'BOOLEAN' ;
	Tok_BY						= 'BY' ;
	Tok_CHOICE					= 'CHOICE' ;
	Tok_COMPONENTS				= 'COMPONENTS' ;
	Tok_COMPONENT				= 'COMPONENT' ;
	Tok_CONTACT_INFO			= 'CONTACT-INFO' ;
	Tok_CREATION_REQUIRES	= 'CREATION-REQUIRES' ;
	Tok_DEFAULT					= 'DEFAULT' ;
	Tok_DEFINED					= 'DEFINED' ;
	Tok_DEFINITIONS			= 'DEFINITIONS' ;
	Tok_DEFVAL					= 'DEFVAL' ;
	Tok_DESCRIPTION			= 'DESCRIPTION' ;
	Tok_DISPLAY_HINT			= 'DISPLAY-HINT' ;
	Tok_END						= 'END' ;
	Tok_ENTERPRISE				= 'ENTERPRISE' ;
	Tok_ENUMERATED				= 'ENUMERATED' ;
	Tok_EXPLICIT				= 'EXPLICIT' ;
	Tok_EXPORTS					= 'EXPORTS' ;
	Tok_FALSE					= 'FALSE' ;
	Tok_FROM						= 'FROM' ;
	Tok_GROUP					= 'GROUP' ;
	Tok_IDENTIFIER				= 'IDENTIFIER' ;
	Tok_IMPLICIT				= 'IMPLICIT' ;
	Tok_IMPLIED					= 'IMPLIED' ;
	Tok_IMPORTS					= 'IMPORTS' ;
	Tok_INCLUDES				= 'INCLUDES' ;
	Tok_INDEX					= 'INDEX' ;
	Tok_Integer32				= 'Integer32' ;
	Tok_INTEGER					= 'INTEGER' ;
	Tok_LAST_UPDATED			= 'LAST-UPDATED' ;
	Tok_MACRO					= 'MACRO' ;
	Tok_MANDATORY_GROUPS		= 'MANDATORY-GROUPS' ;
	Tok_MAX						= 'MAX' ;
	Tok_MAX_ACCESS				= 'MAX-ACCESS' ;
	Tok_MIN						= 'MIN' ;
	Tok_MIN_ACCESS				= 'MIN-ACCESS' ;
	Tok_MINUS_INFINITY		= 'MINUS-INFINITY' ;
	Tok_MODULE_COMPLIANCE	= 'MODULE-COMPLIANCE' ;
	Tok_MODULE_IDENTITY		= 'MODULE-IDENTITY' ;
	Tok_MODULE					= 'MODULE' ;
	Tok_NOTIFICATION_GROUP	= 'NOTIFICATION-GROUP' ;
	Tok_NOTIFICATION_TYPE	= 'NOTIFICATION-TYPE' ;
	Tok_NOTIFICATIONS			= 'NOTIFICATIONS' ;
	Tok_NULL						= 'NULL' ;
	Tok_OBJECT_GROUP			= 'OBJECT-GROUP' ;
	Tok_OBJECT_IDENTITY		= 'OBJECT-IDENTITY' ;
	Tok_OBJECT_TYPE			= 'OBJECT-TYPE' ;
	Tok_OBJECTS					= 'OBJECTS' ;
	Tok_OBJECT					= 'OBJECT' ;
	Tok_OCTET					= 'OCTET' ;
	Tok_OF						= 'OF' ;
	Tok_OPTIONAL				= 'OPTIONAL' ;
	Tok_ORGANIZATION			= 'ORGANIZATION' ;
	Tok_PLUS_INFINITY			= 'PLUS-INFINITY' ;
	Tok_PRESENT					= 'PRESENT' ;
	Tok_PRIVATE					= 'PRIVATE' ;
	Tok_PRODUCT_RELEASE		= 'PRODUCT-RELEASE' ;
	Tok_REAL						= 'REAL' ;
	Tok_REFERENCE				= 'REFERENCE' ;
	Tok_REVISION				= 'REVISION' ;
	Tok_SEQUENCE				= 'SEQUENCE' ;
	Tok_SET						= 'SET' ;
	Tok_SIZE						= 'SIZE' ;
	Tok_TEXTUAL_CONVENTION	= 'TEXTUAL-CONVENTION' ;
	Tok_STATUS					= 'STATUS' ;
	Tok_STRING					= 'STRING' ;
	Tok_SUPPORTS				= 'SUPPORTS' ;
	Tok_SYNTAX					= 'SYNTAX' ;
	Tok_TAGS						= 'TAGS' ;
	Tok_TRAP_TYPE				= 'TRAP-TYPE' ;
	Tok_TRUE						= 'TRUE' ;
	Tok_UNITS					= 'UNITS' ;
	Tok_UNIVERSAL				= 'UNIVERSAL' ;
	Tok_VARIABLES				= 'VARIABLES' ;
	Tok_VARIATION				= 'VARIATION' ;
	Tok_WITH						= 'WITH' ;
	Tok_WRITE_SYNTAX			= 'WRITE-SYNTAX' ;
}

fragment UPPER					: 'A'..'Z' ;
fragment LOWER					: 'a'..'z' ;
fragment DIGIT					: '0'..'9' ;
fragment HT						: '\u0009' ;
fragment LF						: '\u000a' ;
fragment VT						: '\u000b' ;
fragment FF						: '\u000c' ;
fragment CR						: '\u000d' ;
fragment SPACE					: ' ' ;

Tok_Lowercase					: UPPER ( UPPER | LOWER | DIGIT | '-' | '_' ) *
									;
									
Tok_Uppercase					: LOWER ( UPPER | LOWER | DIGIT | '-' | '_' ) *
									;

Tok_Binary						: '\'' ( '0'..'1' ) + '\'' ( 'B' | 'b' )
									;

Tok_Hexadecimal				: '\'' ( DIGIT | 'A'..'F' | 'a'..'f' ) + '\'' ( 'H' | 'h' )
									;

Tok_String						: '"' .* '"'
									;

Tok_Integer						: '0'
									| ( '+' | '-' ) ? '1'..'9' DIGIT *
									;

WHITE_SPACE						: ( HT | LF | VT | FF | CR | SPACE ) +
									;

COMMENT							: '/*' .* '*/'
									;

LINE_COMMENT					: '--' .* (( '--' ) | CR | LF )
									;

mibFile							: mibModule
									;

moduleName						: Tok_Uppercase
									;

typeName							: Tok_Uppercase
									;

valueName						: Tok_Lowercase
									;

identifierName					: Tok_Lowercase
									;

anyName							: Tok_Lowercase
									| Tok_Uppercase
									;

mibModule						: moduleName bitOrObjectIdentifierValue ? Tok_DEFINITIONS tagDefault ? Tok_ASSIGN Tok_BEGIN moduleBody Tok_END
									;

tagDefault						: ( Tok_EXPLICIT | Tok_IMPLICIT ) Tok_TAGS 
									;

moduleBody						: exportList ? importList ? assignmentList
									;

exportList						: Tok_EXPORTS symbolList + Tok_SEMI_COLON
									;

importList						: Tok_IMPORTS symbolsFromModule + Tok_SEMI_COLON
									;

symbolsFromModule				: symbolList Tok_FROM moduleName
									;

symbolList						: symbol ( Tok_COMMA symbol ) *
									;

symbol							: anyName
									| definedMacroName
									;

assignmentList					: assignment +
									;

assignment						: typeAssignment
									| valueAssignment
									| macroDefinition
									;
									
macroDefinition				: macroReference Tok_MACRO Tok_ASSIGN macroBody
									;

macroReference					: definedMacroName
									;

macroBody						: Tok_BEGIN .* Tok_END
									;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// * Type notation productions
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
typeAssignment					: typeName Tok_ASSIGN type
									;

type								: tag ? explicitOrImplicitTag ? type1
									;
									
type1								: builtinType
									| definedType
									| definedMacroType
									;

definedType						: typeName valueOrConstraintList ?
									;

builtinType						: nullType
									| booleanType
									| realType
									| integerType
									| objectIdentifierType
									| stringType
									| bitStringType
									| bitsType
									| sequence
									| set
									| choiceType
									| enumeratedType
									| selectionType
									| anyType
									;

nullType							: Tok_NULL
									;

booleanType						: Tok_BOOLEAN
									;

realType							: Tok_REAL
									;

integerType						: Tok_INTEGER valueOrConstraintList ?
									;

objectIdentifierType			: Tok_OBJECT Tok_IDENTIFIER
									;

stringType						: Tok_OCTET Tok_STRING constraintList ?
									;

bitStringType					: Tok_BIT Tok_STRING valueOrConstraintList ?
									;

bitsType							: Tok_BITS valueOrConstraintList ?
									;
									
sequence							: Tok_SEQUENCE ( sequenceType | sequenceOfType )
									;

sequenceType					: Tok_LEFT_BRACE elementTypeList ? Tok_RIGHT_BRACE
									;

sequenceOfType					: constraintList ? Tok_OF type
									;
									
set								: Tok_SET ( setType | setOfType )
									;

setType							: Tok_LEFT_BRACE elementTypeList ? Tok_RIGHT_BRACE
									;

setOfType						: sizeConstraint ? Tok_OF type
									;

choiceType						: Tok_CHOICE Tok_LEFT_BRACE elementTypeList Tok_RIGHT_BRACE
									;

enumeratedType					: Tok_ENUMERATED namedNumberList
									;

selectionType					: identifierName Tok_LESS type
									;

tag								: Tok_LEFT_BRACKET classtag ? Tok_Integer Tok_RIGHT_BRACKET
									;

classtag							: Tok_UNIVERSAL
									| Tok_APPLICATION
									| Tok_PRIVATE
									;

explicitOrImplicitTag		: Tok_EXPLICIT
									| Tok_IMPLICIT
									;

anyType							: Tok_ANY ( Tok_DEFINED Tok_BY anyName ) ?
									;

elementTypeList				: elementType ( Tok_COMMA elementType ) *
									;

elementType						: identifierName ( elementType1 | elementType2 )
									;

elementType1					: type optionalOrDefaultElement ?
									;

elementType2					: Tok_COMPONENTS Tok_OF type
									;

optionalOrDefaultElement	: optionalOrDefaultElement1 | optionalOrDefaultElement2
									;

optionalOrDefaultElement1	: Tok_OPTIONAL
									;

optionalOrDefaultElement2	: Tok_DEFAULT identifierName ? value
									;

valueOrConstraintList		: namedNumberList
									| constraintList
									;

namedNumberList				: Tok_LEFT_BRACE namedNumber ( Tok_COMMA namedNumber ) * Tok_RIGHT_BRACE
									;

namedNumber						: identifierName Tok_LEFT_PAREN number Tok_RIGHT_PAREN
									;

number							: numberValue
									| binaryValue
									| hexadecimalValue
									| definedValue
									;

constraintList					: Tok_LEFT_PAREN constraint ( Tok_VERTICAL_BAR constraint ) * Tok_RIGHT_PAREN
									;

constraint						: valueConstraint
									| sizeConstraint
									| alphabetConstraint
									| containedTypeConstraint
									| innerTypeConstraint
									;

sizeConstraint					: Tok_SIZE valueConstraintList
									;

alphabetConstraint			: Tok_FROM valueConstraintList
									;

valueConstraintList			: Tok_LEFT_PAREN valueConstraint ( Tok_VERTICAL_BAR valueConstraint ) * Tok_RIGHT_PAREN
									;

valueConstraint				: lowerEndPoint valueRange ?
									;

valueRange						: Tok_LESS ? Tok_DOUBLE_DOT Tok_LESS ? upperEndPoint
									;

lowerEndPoint					: value
									| Tok_MIN
									;

upperEndPoint					: value
									| Tok_MAX
									;

containedTypeConstraint		: Tok_INCLUDES type
									;

innerTypeConstraint			: innerTypeConstraint1 | innerTypeConstraint2
									;

innerTypeConstraint1			: Tok_WITH Tok_COMPONENT valueOrConstraintList
									;

innerTypeConstraint2			: Tok_WITH Tok_COMPONENTS componentsList
									;

componentsList					: componentsList1 | componentsList2
									;

componentsList1				: Tok_LEFT_BRACE componentConstraint componentsListTail * Tok_RIGHT_BRACE
									;

componentsList2				: Tok_LEFT_BRACE Tok_ELLIPSIS componentsListTail + Tok_RIGHT_BRACE
									;

componentsListTail			: Tok_COMMA componentConstraint ?
									;

componentConstraint			: componentConstraint1 | componentConstraint2
									;

componentConstraint1			: identifierName componentValuePresence ?
									;

componentConstraint2			: componentValuePresence
									;

componentValuePresence		: componentValuePresence1 | componentValuePresence2
									;

componentValuePresence1		: valueOrConstraintList componentPresence ?
									;

componentValuePresence2		: componentPresence
									;

componentPresence				: Tok_PRESENT
									| Tok_ABSENT
									| Tok_OPTIONAL
									;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// * Value notation productions
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
valueAssignment				: valueName type Tok_ASSIGN value
									;

value								: builtinValue
									| definedValue
									;

definedValue					: valueName
									;

builtinValue					: nullValue
									| booleanValue
									| specialRealValue
									| numberValue
									| binaryValue
									| hexadecimalValue
									| stringValue
									| bitOrObjectIdentifierValue
									;

nullValue						: Tok_NULL
									;

booleanValue					: Tok_TRUE
									| Tok_FALSE
									;

specialRealValue				: Tok_PLUS_INFINITY
									| Tok_MINUS_INFINITY
									;

numberValue						: Tok_Integer
									;

binaryValue						: Tok_Binary
									;

hexadecimalValue				: Tok_Hexadecimal
									;

stringValue						: Tok_String
									;

bitOrObjectIdentifierValue	: Tok_LEFT_BRACE nameValueComponent * Tok_RIGHT_BRACE
									;

nameValueComponent			: nameOrNumber ( Tok_COMMA nameOrNumber ) *
									;

nameOrNumber					: nameOrNumber1 | nameOrNumber2
									;

nameOrNumber1					: Tok_Integer
									;

nameOrNumber2					: identifierName nameAndNumber ?
									;

nameAndNumber					: Tok_LEFT_PAREN ( nameAndNumber1 | nameAndNumber2 ) Tok_RIGHT_PAREN
									;

nameAndNumber1					: Tok_Integer
									;

nameAndNumber2					: definedValue
									;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// * Macro Syntax definitions
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
definedMacroType				: macroModuleIdentity
									| macroObjectIdentity
									| macroObjectType
									| macroNotificationType
									| macroTrapType
									| macroTextualConvention
									| macroObjectGroup
									| macroNotificationGroup
									| macroModuleCompliance
									| macroAgentCapabilities
									;

definedMacroName				: Tok_MODULE_IDENTITY
									| Tok_OBJECT_IDENTITY
									| Tok_OBJECT_TYPE
									| Tok_NOTIFICATION_TYPE
									| Tok_TRAP_TYPE
									| Tok_TEXTUAL_CONVENTION
									| Tok_OBJECT_GROUP
									| Tok_NOTIFICATION_GROUP
									| Tok_MODULE_COMPLIANCE
									| Tok_AGENT_CAPABILITIES
									;

macroModuleIdentity			: Tok_MODULE_IDENTITY snmpUpdatePart snmpOrganizationPart snmpContactPart snmpDescrPart snmpRevisionPart *
									;

macroObjectIdentity			: Tok_OBJECT_IDENTITY snmpStatusPart snmpDescrPart snmpReferPart ?
									;

macroObjectType				: Tok_OBJECT_TYPE snmpSyntaxPart snmpUnitsPart ? snmpAccessPart snmpStatusPart snmpDescrPart ? snmpReferPart ? snmpIndexPart ? snmpDefValPart ?
									;

macroNotificationType		: Tok_NOTIFICATION_TYPE snmpObjectsPart ? snmpStatusPart snmpDescrPart snmpReferPart ?
									;

macroTrapType					: Tok_TRAP_TYPE snmpEnterprisePart snmpVarPart ? snmpDescrPart ? snmpReferPart ?
									;

macroTextualConvention		: Tok_TEXTUAL_CONVENTION snmpDisplayPart ? snmpStatusPart snmpDescrPart snmpReferPart ? snmpSyntaxPart
									;

macroObjectGroup				: Tok_OBJECT_GROUP snmpObjectsPart snmpStatusPart snmpDescrPart snmpReferPart ?
									;

macroNotificationGroup		: Tok_NOTIFICATION_GROUP snmpNotificationsPart snmpStatusPart snmpDescrPart snmpReferPart ?
									;

macroModuleCompliance		: Tok_MODULE_COMPLIANCE snmpStatusPart snmpDescrPart snmpReferPart ? snmpModulePart +
									;

macroAgentCapabilities		: Tok_AGENT_CAPABILITIES snmpProductReleasePart snmpStatusPart snmpDescrPart snmpReferPart ? snmpModuleSupportPart *
									;

snmpUpdatePart					: Tok_LAST_UPDATED Tok_String
									;

snmpOrganizationPart			: Tok_ORGANIZATION Tok_String
									;

snmpContactPart				: Tok_CONTACT_INFO Tok_String
									;

snmpDescrPart					: Tok_DESCRIPTION Tok_String
									;

snmpRevisionPart				: Tok_REVISION value Tok_DESCRIPTION Tok_String
									;

snmpStatusPart					: Tok_STATUS identifierName
									;

snmpReferPart					: Tok_REFERENCE Tok_String
									;

snmpSyntaxPart					: Tok_SYNTAX type
									;

snmpUnitsPart					: Tok_UNITS Tok_String
									;

snmpAccessPart					: Tok_ACCESS identifierName
									| Tok_MAX_ACCESS identifierName
									| Tok_MIN_ACCESS identifierName
									;

snmpIndexPart					: snmpIndexPart1 | snmpIndexPart2
									;

snmpIndexPart1					: Tok_INDEX Tok_LEFT_BRACE indexValueList Tok_RIGHT_BRACE
									;

snmpIndexPart2					: Tok_AUGMENTS Tok_LEFT_BRACE value Tok_RIGHT_BRACE
									;

indexValueList					: indexValue ( Tok_COMMA indexValue ) *
									;

indexValue						: indexValue1 | indexValue2
									;

indexValue1						: Tok_IMPLIED ? value
									;

indexValue2						: indexType
									;

indexType						: integerType
									| stringType
									| objectIdentifierType
									| typeName
									;

snmpDefValPart					: Tok_DEFVAL Tok_LEFT_BRACE value Tok_RIGHT_BRACE
									;

snmpObjectsPart				: Tok_OBJECTS Tok_LEFT_BRACE valueList Tok_RIGHT_BRACE
									;

valueList						: value ( Tok_COMMA value ) *
									;

snmpEnterprisePart			: Tok_ENTERPRISE value
									;

snmpVarPart						: Tok_VARIABLES Tok_LEFT_BRACE valueList Tok_RIGHT_BRACE
									;

snmpDisplayPart				: Tok_DISPLAY_HINT Tok_String
									;

snmpNotificationsPart		: Tok_NOTIFICATIONS Tok_LEFT_BRACE valueList Tok_RIGHT_BRACE
									;

snmpModulePart					: Tok_MODULE snmpModuleImport ? snmpMandatoryPart ? snmpCompliancePart *
									;

snmpModuleImport				: moduleName
									;

snmpMandatoryPart				: Tok_MANDATORY_GROUPS Tok_LEFT_BRACE valueList Tok_RIGHT_BRACE
									;

snmpCompliancePart			: complianceGroup
									| complianceObject
									;

complianceGroup				: Tok_GROUP value snmpDescrPart
									;

complianceObject				: Tok_OBJECT value snmpSyntaxPart ? snmpWriteSyntaxPart ? snmpAccessPart ? snmpDescrPart
									;

snmpWriteSyntaxPart			: Tok_WRITE_SYNTAX type
									;

snmpProductReleasePart		: Tok_PRODUCT_RELEASE Tok_String
									;

snmpModuleSupportPart		: Tok_SUPPORTS snmpModuleImport Tok_INCLUDES Tok_LEFT_BRACE valueList Tok_RIGHT_BRACE snmpVariationPart *
									;

snmpVariationPart				: Tok_VARIATION value snmpSyntaxPart ? snmpWriteSyntaxPart ? snmpAccessPart ? snmpCreationPart ? snmpDefValPart ? snmpDescrPart
									;

snmpCreationPart				: Tok_CREATION_REQUIRES Tok_LEFT_BRACE valueList ? Tok_RIGHT_BRACE
									;
