#include "MibParser.h"
#include "Debug.h"
#include "ParserPow.h"

MibParser::MibParser(ParserAction & Callback,
							const QString & name) throw (ParserException)
	: ParserGeneric(Callback)
	, m_lexer(name)
{
	_generate();
	Finalize();
	Check();
	//Dump();
}

MibParser::~MibParser(void)
{
}

bool MibParser::NextToken(LexToken_t & token)
{
	// read next token
	bool ret = m_lexer.Next(token.Token, token.ExtraValue);
	if(ret == false)
		return false;

	// add more information
	token.MibFileLine = m_lexer.FileLine();
	token.TokenStr = MibLexer::Decode(token.Token);
	token.MibFileName = m_lexer.FileName();
	return true;
}

void MibParser::Execute(void)
{
	// entry point of mib perser
	ParserGeneric::Execute("mibFile");
}

void MibParser::_generate(void)
{
	mibFile();
	moduleName();
	typeName();
	valueName();
	identifierName();
	anyName();
	mibModule();
	tagDefault();
	moduleBody();
	exportList();
	importList();
	symbolsFromModule();
	symbolList();
	symbol();
	assignmentList();
	assignment();
	macroDefinition();
	macroReference();
	macroBody();
	typeAssignment();
	type();
	type1();
	definedType();
	builtinType();
	nullType();
	booleanType();
	realType();
	integerType();
	objectIdentifierType();
	stringType();
	bitStringType();
	bitsType();
	sequence();
	sequenceType();
	sequenceOfType();
	set();
	setType();
	setOfType();
	choiceType();
	enumeratedType();
	selectionType();
	tag();
	classtag();
	explicitOrImplicitTag();
	anyType();
	elementTypeList();
	elementType();
	elementType1();
	elementType2();
	optionalOrDefaultElement();
	optionalOrDefaultElement1();
	optionalOrDefaultElement2();
	valueOrConstraintList();
	namedNumberList();
	namedNumber();
	number();
	constraintList();
	constraint();
	valueConstraintList();
	valueConstraint();
	valueRange();
	lowerEndPoint();
	upperEndPoint();
	sizeConstraint();
	alphabetConstraint();
	containedTypeConstraint();
	innerTypeConstraint();
	innerTypeConstraint1();
	innerTypeConstraint2();
	componentsList();
	componentsList1();
	componentsList2();
	componentsListTail();
	componentConstraint();
	componentConstraint1();
	componentConstraint2();
	componentValuePresence();
	componentValuePresence1();
	componentValuePresence2();
	componentPresence();
	valueAssignment();
	value();
	definedValue();
	builtinValue();
	nullValue();
	booleanValue();
	specialRealValue();
	numberValue();
	binaryValue();
	hexadecimalValue();
	stringValue();
	bitOrObjectIdentifierValue();
	nameOrNumber();
	nameOrNumber1();
	nameOrNumber2();
	nameValueComponent();
	nameAndNumber();
	nameAndNumber1();
	nameAndNumber2();
	definedMacroType();
	definedMacroName();
	macroModuleIdentity();
	macroObjectIdentity();
	macroObjectType();
	macroNotificationType();
	macroTrapType();
	macroTextualConvention();
	macroObjectGroup();
	macroNotificationGroup();
	macroModuleCompliance();
	macroAgentCapabilities();
	snmpUpdatePart();
	snmpOrganizationPart();
	snmpContactPart();
	snmpDescrPart();
	snmpRevisionPart();
	snmpStatusPart();
	snmpReferPart();
	snmpSyntaxPart();
	snmpUnitsPart();
	snmpAccessPart();
	snmpIndexPart();
	snmpIndexPart1();
	snmpIndexPart2();
	indexValueList();
	indexValue();
	indexValue1();
	indexValue2();
	indexType();
	snmpDefValPart();
	snmpObjectsPart();
	valueList();
	snmpEnterprisePart();
	snmpVarPart();
	snmpDisplayPart();
	snmpNotificationsPart();
	snmpModulePart();
	snmpModuleImport();
	snmpMandatoryPart();
	snmpCompliancePart();
	complianceGroup();
	complianceObject();
	snmpWriteSyntaxPart();
	snmpProductReleasePart();
	snmpModuleSupportPart();
	snmpVariationPart();
	snmpCreationPart();
}

void MibParser::mibFile(void)
{
	ParserPow pow(this, "mibFile");
	Cardinal_1(pow, "mibModule", Tok_EndOfFile);
//mibFile							: mibModule
}

void MibParser::moduleName(void)
{
	ParserPow pow(this, "moduleName");
	Cardinal_1(pow, Tok_Uppercase);
	Call(pow);
//moduleName						: Tok_Uppercase
}

void MibParser::typeName(void)
{
	ParserPow pow(this, "typeName");
	Cardinal_1(pow, Tok_Uppercase);
	Call(pow);
//typeName							: Tok_Uppercase
}

void MibParser::valueName(void)
{
	ParserPow pow(this, "valueName");
	Cardinal_1(pow, Tok_Lowercase);
	Call(pow);
//valueName						: Tok_Lowercase
}

void MibParser::identifierName(void)
{
	ParserPow pow(this, "identifierName");
	Cardinal_1(pow, Tok_Lowercase);
	Call(pow);
//identifierName					: Tok_Lowercase
}

void MibParser::anyName(void)
{
	ParserPow pow(this, "anyName");
	Branch(pow, Tok_Lowercase, Tok_Uppercase);
	Call(pow);
//anyName							: Tok_Lowercase
//									| Tok_Uppercase
}

void MibParser::mibModule(void)
{
	ParserPow pow(this, "mibModule");
	Cardinal_1(pow, "moduleName");
	Call(pow);
	Cardinal_0_1(pow, "bitOrObjectIdentifierValue");
	Cardinal_1(pow, Tok_DEFINITIONS);
	Cardinal_0_1(pow, "tagDefault");
	Cardinal_1(pow, Tok_ASSIGN, Tok_BEGIN, "moduleBody", Tok_END);
//mibModule						: moduleName bitOrObjectIdentifierValue ? Tok_DEFINITIONS tagDefault ? Tok_ASSIGN Tok_BEGIN moduleBody Tok_END
}

void MibParser::tagDefault(void)
{
	ParserPow pow(this, "tagDefault");
	Branch(pow, Tok_EXPLICIT, Tok_IMPLICIT);
	Cardinal_1(pow, Tok_TAGS);
//tagDefault						: ( Tok_EXPLICIT | Tok_IMPLICIT ) Tok_TAGS
}

void MibParser::moduleBody(void)
{
	ParserPow pow(this, "moduleBody");
	Cardinal_0_1(pow, "exportList");
	Cardinal_0_1(pow, "importList");
	Cardinal_1(pow, "assignmentList");
//moduleBody						: exportList ? importList ? assignmentList
}

void MibParser::exportList(void)
{
	ParserPow pow(this, "exportList");
	Cardinal_1(pow, Tok_EXPORTS);
	Cardinal_1_n(pow, "symbolList");
	Cardinal_1(pow, Tok_SEMI_COLON);
//exportList						: Tok_EXPORTS symbolList + Tok_SEMI_COLON
}

void MibParser::importList(void)
{
	ParserPow pow(this, "importList");
	Cardinal_1(pow, Tok_IMPORTS);
	Call(pow);
	Cardinal_1_n(pow, "symbolsFromModule");
	Cardinal_1(pow, Tok_SEMI_COLON);
	Call(pow);
//importList						: Tok_IMPORTS symbolsFromModule + Tok_SEMI_COLON
}

void MibParser::symbolsFromModule(void)
{
	ParserPow pow(this, "symbolsFromModule");
	Cardinal_1(pow, "symbolList", Tok_FROM);
	Cardinal_1(pow, "moduleName");
//symbolsFromModule				: symbolList Tok_FROM moduleName
	Call(pow);
}

void MibParser::symbolList(void)
{
	ParserPow pow(this, "symbolList");
	Cardinal_1(pow, "symbol");
	Cardinal_0_n(pow, Tok_COMMA, "symbol");
//symbolList						: symbol ( Tok_COMMA symbol ) *
}

void MibParser::symbol(void)
{
	ParserPow pow(this, "symbol");
	Branch(pow, "anyName", "definedMacroName");
//symbol							: anyName
//									| definedMacroName
	Call(pow);
}

void MibParser::assignmentList(void)
{
	ParserPow pow(this, "assignmentList");
	Cardinal_0_n(pow, "assignment");
//assignmentList					: assignment +
}

void MibParser::assignment(void)
{
	ParserPow pow(this, "assignment");
	Branch(pow, "typeAssignment", "valueAssignment", "macroDefinition");
//assignment						: typeAssignment
//									| valueAssignment
//									| macroDefinition
}

void MibParser::macroDefinition(void)
{
	ParserPow pow(this, "macroDefinition");
	Cardinal_1(pow, "macroReference", Tok_MACRO, Tok_ASSIGN, "macroBody");
//macroDefinition				: macroReference Tok_MACRO Tok_ASSIGN macroBody
}

void MibParser::macroReference(void)
{
	ParserPow pow(this, "macroReference");
	Cardinal_1(pow, "definedMacroName");
//macroReference					: definedMacroName
}

void MibParser::macroBody(void)
{
	ParserPow pow(this, "macroBody");
	Cardinal_1(pow, Tok_BEGIN);
	Greedy(pow, Tok_END);
	Cardinal_1(pow, Tok_END);
//macroBody						: Tok_BEGIN .* Tok_END
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// * Type notation productions
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void MibParser::typeAssignment(void)
{
	ParserPow pow(this, "typeAssignment");
	Cardinal_1(pow, "typeName");
	Cardinal_1(pow, Tok_ASSIGN);
	Cardinal_1(pow, "type");
	Call(pow);
//typeAssignment					: typeName Tok_ASSIGN type
}

void MibParser::type(void)
{
	ParserPow pow(this, "type");
	Cardinal_0_1(pow, "tag");
	Cardinal_0_1(pow, "explicitOrImplicitTag");
	Cardinal_1(pow, "type1");
//type								: tag ? explicitOrImplicitTag ? type1
}

void MibParser::type1(void)
{
	ParserPow pow(this, "type1");
	Call(pow);
	Branch(pow, "builtinType", "definedType", "definedMacroType");
//type								: builtinType
//									| definedType
//									| definedMacroType
	Call(pow);
}

void MibParser::definedType(void)
{
	ParserPow pow(this, "definedType");
	Cardinal_1(pow, "typeName");
	Call(pow);
	Cardinal_0_1(pow, "valueOrConstraintList");
//definedType						: typeName valueOrConstraintList ?
}

void MibParser::builtinType(void)
{
	ParserPow pow(this, "builtinType");
	Branch(pow, "nullType", "booleanType", "realType", "integerType", "objectIdentifierType", "stringType", "bitStringType", "bitsType", "sequence", "set", "choiceType", "enumeratedType", "selectionType", "anyType");
//builtinType						: nullType
//									| booleanType
//									| realType
//									| integerType
//									| objectIdentifierType
//									| stringType
//									| bitStringType
//									| bitsType
//									| sequence
//									| set
//									| choiceType
//									| enumeratedType
//									| selectionType
//									| anyType
}

void MibParser::nullType(void)
{
	ParserPow pow(this, "nullType");
	Cardinal_1(pow, Tok_NULL);
	Call(pow);
//nullType							: Tok_NULL
}

void MibParser::booleanType(void)
{
	ParserPow pow(this, "booleanType");
	Cardinal_1(pow, Tok_BOOLEAN);
	Call(pow);
//booleanType						: Tok_BOOLEAN
}

void MibParser::realType(void)
{
	ParserPow pow(this, "realType");
	Cardinal_1(pow, Tok_REAL);
	Call(pow);
//realType							: Tok_REAL
}

void MibParser::integerType(void)
{
	ParserPow pow(this, "integerType");
	Cardinal_1(pow, Tok_INTEGER);
	Call(pow);
	Cardinal_0_1(pow, "valueOrConstraintList");
//integerType						: Tok_INTEGER valueOrConstraintList ?
}

void MibParser::objectIdentifierType(void)
{
	ParserPow pow(this, "objectIdentifierType");
	Cardinal_1(pow, Tok_OBJECT, Tok_IDENTIFIER);
	Call(pow);
//objectIdentifierType			: Tok_OBJECT Tok_IDENTIFIER
}

void MibParser::stringType(void)
{
	ParserPow pow(this, "stringType");
	Cardinal_1(pow, Tok_OCTET, Tok_STRING);
	Call(pow);
	Cardinal_0_1(pow, "constraintList");
//stringType						: Tok_OCTET Tok_STRING constraintList ?
}

void MibParser::bitStringType(void)
{
	ParserPow pow(this, "bitStringType");
	Cardinal_1(pow, Tok_BIT, Tok_STRING);
	Call(pow);
	Cardinal_0_1(pow, "valueOrConstraintList");
//bitStringType					: Tok_BIT Tok_STRING valueOrConstraintList ?
}

void MibParser::bitsType(void)
{
	ParserPow pow(this, "bitsType");
	Cardinal_1(pow, Tok_BITS);
	Call(pow);
	Cardinal_0_1(pow, "valueOrConstraintList");
//bitsType							: Tok_BITS valueOrConstraintList ?
}

void MibParser::sequence(void)
{
	ParserPow pow(this, "sequence");
	Cardinal_1(pow, Tok_SEQUENCE);
	Branch(pow, "sequenceType", "sequenceOfType");
//sequence					: Tok_SEQUENCE ( sequenceType | sequenceOfType )
}

void MibParser::sequenceType(void)
{
	ParserPow pow(this, "sequenceType");
	Cardinal_1(pow, Tok_LEFT_BRACE);
	Call(pow);
	Cardinal_0_1(pow, "elementTypeList");
	Cardinal_1(pow, Tok_RIGHT_BRACE);
//sequenceType					: Tok_LEFT_BRACE elementTypeList ? Tok_RIGHT_BRACE
}

void MibParser::sequenceOfType(void)
{
	ParserPow pow(this, "sequenceOfType");
	Cardinal_0_1(pow, "constraintList");
	Cardinal_1(pow, Tok_OF);
	Call(pow);
	Cardinal_1(pow, "type");
//sequenceOfType					: constraintList ? Tok_OF type
}

void MibParser::set(void)
{
	ParserPow pow(this, "set");
	Cardinal_1(pow, Tok_SET);
	Branch(pow, "setType", "setOfType");
//set					: Tok_SET ( setType | setOfType )
}

void MibParser::setType(void)
{
	ParserPow pow(this, "setType");
	Cardinal_1(pow, Tok_LEFT_BRACE);
	Call(pow);
	Cardinal_0_1(pow, "elementTypeList");
	Cardinal_1(pow, Tok_RIGHT_BRACE);
//setType							: Tok_LEFT_BRACE elementTypeList ? Tok_RIGHT_BRACE
}

void MibParser::setOfType(void)
{
	ParserPow pow(this, "setOfType");
	Cardinal_0_1(pow, "sizeConstraint");
	Cardinal_1(pow, Tok_OF);
	Call(pow);
	Cardinal_1(pow, "type");
//setOfType						: sizeConstraint ? Tok_OF type
}

void MibParser::choiceType(void)
{
	ParserPow pow(this, "choiceType");
	Cardinal_1(pow, Tok_CHOICE);
	Call(pow);
	Cardinal_1(pow, Tok_LEFT_BRACE, "elementTypeList", Tok_RIGHT_BRACE);
//choiceType						: Tok_CHOICE Tok_LEFT_BRACE elementTypeList Tok_RIGHT_BRACE
}

void MibParser::enumeratedType(void)
{
	ParserPow pow(this, "enumeratedType");
	Cardinal_1(pow, Tok_ENUMERATED);
	Call(pow);
	Cardinal_1(pow, "namedNumberList");
//enumeratedType					: Tok_ENUMERATED namedNumberList
}

void MibParser::selectionType(void)
{
	ParserPow pow(this, "selectionType");
	Cardinal_1(pow, "identifierName");
	Call(pow);
	Cardinal_1(pow, Tok_LESS, "type");
//selectionType					: identifierName Tok_LESS type
}

void MibParser::tag(void)
{
	ParserPow pow(this, "tag");
	Cardinal_1(pow, Tok_LEFT_BRACKET);
	Cardinal_0_1(pow, "classtag");
	Cardinal_1(pow, Tok_Integer, Tok_RIGHT_BRACKET);
//tag								: Tok_LEFT_BRACKET class ? Tok_Integer Tok_RIGHT_BRACKET
}

void MibParser::classtag(void)
{
	ParserPow pow(this, "classtag");
	Branch(pow, Tok_UNIVERSAL, Tok_APPLICATION, Tok_PRIVATE);
//classtag						: Tok_UNIVERSAL
//									| Tok_APPLICATION
//									| Tok_PRIVATE
}

void MibParser::explicitOrImplicitTag(void)
{
	ParserPow pow(this, "explicitOrImplicitTag");
	Branch(pow, Tok_EXPLICIT, Tok_IMPLICIT);
//explicitOrImplicitTag		: Tok_EXPLICIT
//									| Tok_IMPLICIT
}

void MibParser::anyType(void)
{
	ParserPow pow(this, "anyType");
	Cardinal_1(pow, Tok_ANY);
	Call(pow);
	Cardinal_0_1(pow, Tok_DEFINED, Tok_BY, "anyName");
//anyType							: Tok_ANY ( Tok_DEFINED Tok_BY anyName ) ?
}

void MibParser::elementTypeList(void)
{
	ParserPow pow(this, "elementTypeList");
	Cardinal_1(pow, "elementType");
	Cardinal_0_n(pow, Tok_COMMA, "elementType");
//elementTypeList				: elementType ( Tok_COMMA elementType ) *
}

void MibParser::elementType(void)
{
	ParserPow pow(this, "elementType");
	Cardinal_1(pow, "identifierName");
	Branch(pow, "elementType1", "elementType2");
//elementType						: identifierName ( elementType1 | elementType2 )
}

void MibParser::elementType1(void)
{
	ParserPow pow(this, "elementType1");
	Cardinal_1(pow, "type");
	Cardinal_0_1(pow, "optionalOrDefaultElement");
//elementType1					: type optionalOrDefaultElement ?
}

void MibParser::elementType2(void)
{
	ParserPow pow(this, "elementType2");
	Cardinal_1(pow, Tok_COMPONENTS, Tok_OF, "type");
//elementType2					: Tok_COMPONENTS Tok_OF type
}

void MibParser::optionalOrDefaultElement(void)
{
	ParserPow pow(this, "optionalOrDefaultElement");
	Branch(pow, "optionalOrDefaultElement1", "optionalOrDefaultElement2");
//optionalOrDefaultElement	: optionalOrDefaultElement1 | optionalOrDefaultElement2
}

void MibParser::optionalOrDefaultElement1(void)
{
	ParserPow pow(this, "optionalOrDefaultElement1");
	Cardinal_1(pow, Tok_OPTIONAL);
//optionalOrDefaultElement	: Tok_OPTIONAL
}

void MibParser::optionalOrDefaultElement2(void)
{
	ParserPow pow(this, "optionalOrDefaultElement2");
	Cardinal_1(pow, Tok_DEFAULT);
	Cardinal_0_1(pow, "identifierName");
	Cardinal_1(pow, "value");
//optionalOrDefaultElement	: Tok_DEFAULT identifierName ? value
}

void MibParser::valueOrConstraintList(void)
{
	ParserPow pow(this, "valueOrConstraintList");
	Branch(pow, "namedNumberList", "constraintList");
//valueOrConstraintList		: namedNumberList
//									| constraintList
}

void MibParser::namedNumberList(void)
{
	ParserPow pow(this, "namedNumberList");
	Cardinal_1(pow, Tok_LEFT_BRACE, "namedNumber");
	Cardinal_0_n(pow, Tok_COMMA, "namedNumber");
	Cardinal_1(pow, Tok_RIGHT_BRACE);
//namedNumberList				: Tok_LEFT_BRACE namedNumber ( Tok_COMMA namedNumber ) * Tok_RIGHT_BRACE
}

void MibParser::namedNumber(void)
{
	ParserPow pow(this, "namedNumber");
	Cardinal_1(pow, "identifierName");
	Call(pow);
	Cardinal_1(pow, Tok_LEFT_PAREN);
	Cardinal_1(pow, "number");
	Call(pow);
	Cardinal_1(pow, Tok_RIGHT_PAREN);
//namedNumber						: identifierName Tok_LEFT_PAREN number Tok_RIGHT_PAREN
}

void MibParser::number(void)
{
	ParserPow pow(this, "number");
	Branch(pow, "numberValue", "binaryValue", "hexadecimalValue", "definedValue");
//number							: numberValue
//									| binaryValue
//									| hexadecimalValue
//									| definedValue
}

void MibParser::constraintList(void)
{
	ParserPow pow(this, "constraintList");
	Cardinal_1(pow, Tok_LEFT_PAREN, "constraint");
	Cardinal_0_n(pow, Tok_VERTICAL_BAR, "constraint");
	Cardinal_1(pow, Tok_RIGHT_PAREN);
//constraintList					: Tok_LEFT_PAREN constraint ( Tok_VERTICAL_BAR constraint ) * Tok_RIGHT_PAREN
}

void MibParser::constraint(void)
{
	ParserPow pow(this, "constraint");
	Call(pow);
	Branch(pow, "valueConstraint", "sizeConstraint", "alphabetConstraint", "containedTypeConstraint", "innerTypeConstraint");
//constraint						: valueConstraint
//									| sizeConstraint
//									| alphabetConstraint
//									| containedTypeConstraint
//									| innerTypeConstraint
}

void MibParser::valueConstraintList(void)
{
	ParserPow pow(this, "valueConstraintList");
	Cardinal_1(pow, Tok_LEFT_PAREN, "valueConstraint");
	Cardinal_0_n(pow, Tok_VERTICAL_BAR, "valueConstraint");
	Cardinal_1(pow, Tok_RIGHT_PAREN);
//valueConstraintList			: Tok_LEFT_PAREN valueConstraint ( Tok_VERTICAL_BAR valueConstraint ) * Tok_RIGHT_PAREN
}

void MibParser::valueConstraint(void)
{
	ParserPow pow(this, "valueConstraint");
	Cardinal_1(pow, "lowerEndPoint");
	Cardinal_0_1(pow, "valueRange");
//valueConstraint				: lowerEndPoint valueRange ?
}

void MibParser::valueRange(void)
{
	ParserPow pow(this, "valueRange");
	Cardinal_0_1(pow, Tok_LESS);
	Cardinal_1(pow, Tok_DOUBLE_DOT);
	Cardinal_0_1(pow, Tok_LESS);
	Cardinal_1(pow, "upperEndPoint");
//valueRange						: Tok_LESS ? Tok_DOUBLE_DOT Tok_LESS ? upperEndPoint
}

void MibParser::lowerEndPoint(void)
{
	ParserPow pow(this, "lowerEndPoint");
	Branch(pow, "value", Tok_MIN);
	Call(pow);
//lowerEndPoint					: value
//									| Tok_MIN
}

void MibParser::upperEndPoint(void)
{
	ParserPow pow(this, "upperEndPoint");
	Branch(pow, "value", Tok_MAX);
	Call(pow);
//upperEndPoint					: value
//									| Tok_MAX
}

void MibParser::sizeConstraint(void)
{
	ParserPow pow(this, "sizeConstraint");
	Cardinal_1(pow, Tok_SIZE);
	Call(pow);
	Cardinal_1(pow, "valueConstraintList");
	Call(pow);
//sizeConstraint					: Tok_SIZE valueConstraintList
}

void MibParser::alphabetConstraint(void)
{
	ParserPow pow(this, "alphabetConstraint");
	Cardinal_1(pow, Tok_FROM, "valueConstraintList");
//alphabetConstraint			: Tok_FROM valueConstraintList
}

void MibParser::containedTypeConstraint(void)
{
	ParserPow pow(this, "containedTypeConstraint");
	Cardinal_1(pow, Tok_INCLUDES, "type");
//containedTypeConstraint		: Tok_INCLUDES type
}

void MibParser::innerTypeConstraint(void)
{
	ParserPow pow(this, "innerTypeConstraint");
	Branch(pow, "innerTypeConstraint1", "innerTypeConstraint2");
//innerTypeConstraint			: innerTypeConstraint1 | innerTypeConstraint2
}

void MibParser::innerTypeConstraint1(void)
{
	ParserPow pow(this, "innerTypeConstraint1");
	Cardinal_1(pow, Tok_WITH, Tok_COMPONENT, "valueOrConstraintList");
//innerTypeConstraint1			: Tok_WITH Tok_COMPONENT valueOrConstraintList
}

void MibParser::innerTypeConstraint2(void)
{
	ParserPow pow(this, "innerTypeConstraint2");
	Cardinal_1(pow, Tok_WITH, Tok_COMPONENTS, "componentsList");
//innerTypeConstraint2			: Tok_WITH Tok_COMPONENTS componentsList
}

void MibParser::componentsList(void)
{
	ParserPow pow(this, "componentsList");
	Branch(pow, "componentsList1", "componentsList2");
//componentsList					: componentsList1 | componentsList2
}

void MibParser::componentsList1(void)
{
	ParserPow pow(this, "componentsList1");
	Cardinal_1(pow, Tok_LEFT_BRACE, "componentConstraint");
	Cardinal_0_n(pow, "componentsListTail");
	Cardinal_1(pow, Tok_RIGHT_BRACE);
//componentsList1				: Tok_LEFT_BRACE componentConstraint componentsListTail * Tok_RIGHT_BRACE
}

void MibParser::componentsList2(void)
{
	ParserPow pow(this, "componentsList2");
	Cardinal_1(pow, Tok_LEFT_BRACE, Tok_ELLIPSIS);
	Cardinal_1_n(pow, "componentsListTail");
	Cardinal_1(pow, Tok_RIGHT_BRACE);
//componentsList2				: Tok_LEFT_BRACE Tok_ELLIPSIS componentsListTail + Tok_RIGHT_BRACE
}

void MibParser::componentsListTail(void)
{
	ParserPow pow(this, "componentsListTail");
	Cardinal_1(pow, Tok_COMMA);
	Cardinal_0_1(pow, "componentConstraint");
//componentsListTail			: Tok_COMMA componentConstraint ?
}

void MibParser::componentConstraint(void)
{
	ParserPow pow(this, "componentConstraint");
	Branch(pow, "componentConstraint1", "componentConstraint2");
//componentConstraint			: componentConstraint1 | componentConstraint2
}

void MibParser::componentConstraint1(void)
{
	ParserPow pow(this, "componentConstraint1");
	Cardinal_1(pow, "identifierName");
	Cardinal_0_1(pow, "componentValuePresence");
//componentConstraint1			: identifierName componentValuePresence ?
}

void MibParser::componentConstraint2(void)
{
	ParserPow pow(this, "componentConstraint2");
	Cardinal_1(pow, "componentValuePresence");
//componentConstraint2			: componentValuePresence
}

void MibParser::componentValuePresence(void)
{
	ParserPow pow(this, "componentValuePresence");
	Branch(pow, "componentValuePresence1", "componentValuePresence2");
//componentValuePresence		: componentValuePresence1 | componentValuePresence2
}

void MibParser::componentValuePresence1(void)
{
	ParserPow pow(this, "componentValuePresence1");
	Cardinal_1(pow, "valueOrConstraintList");
	Cardinal_0_1(pow, "componentPresence");
//componentValuePresence1		: valueOrConstraintList componentPresence ?
}

void MibParser::componentValuePresence2(void)
{
	ParserPow pow(this, "componentValuePresence2");
	Cardinal_1(pow, "componentPresence");
//componentValuePresence2		: componentPresence
}

void MibParser::componentPresence(void)
{
	ParserPow pow(this, "componentPresence");
	Branch(pow, Tok_PRESENT, Tok_ABSENT, Tok_OPTIONAL);
//componentPresence				: Tok_PRESENT
//									| Tok_ABSENT
//									| Tok_OPTIONAL
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// * Value notation productions
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void MibParser::valueAssignment(void)
{
	ParserPow pow(this, "valueAssignment");
	Cardinal_1(pow, "valueName");
	Call(pow);
	Cardinal_1(pow, "type");
	Cardinal_1(pow, Tok_ASSIGN, "value");
//valueAssignment				: valueName type Tok_ASSIGN value
	Call(pow);
}

void MibParser::value(void)
{
	ParserPow pow(this, "value");
	Branch(pow, "builtinValue", "definedValue");
//value								: builtinValue
//									| definedValue
}

void MibParser::definedValue(void)
{
	ParserPow pow(this, "definedValue");
	Cardinal_1(pow, "valueName");
//definedValue					: valueName
}

void MibParser::builtinValue(void)
{
	ParserPow pow(this, "builtinValue");
	Branch(pow, "nullValue", "booleanValue", "specialRealValue", "numberValue", "binaryValue", "hexadecimalValue", "stringValue", "bitOrObjectIdentifierValue");
//builtinValue					: nullValue
//									| booleanValue
//									| specialRealValue
//									| numberValue
//									| binaryValue
//									| hexadecimalValue
//									| stringValue
//									| bitOrObjectIdentifierValue
}

void MibParser::nullValue(void)
{
	ParserPow pow(this, "nullValue");
	Cardinal_1(pow, Tok_NULL);
//nullValue						: Tok_NULL
}

void MibParser::booleanValue(void)
{
	ParserPow pow(this, "booleanValue");
	Branch(pow, Tok_TRUE, Tok_FALSE);
//booleanValue					: Tok_TRUE
//									| Tok_FALSE
}

void MibParser::specialRealValue(void)
{
	ParserPow pow(this, "specialRealValue");
	Branch(pow, Tok_PLUS_INFINITY, Tok_MINUS_INFINITY);
//specialRealValue				: Tok_PLUS_INFINITY
//									| Tok_MINUS_INFINITY
}

void MibParser::numberValue(void)
{
	ParserPow pow(this, "numberValue");
	Cardinal_1(pow, Tok_Integer);
//numberValue						: Tok_Integer
}

void MibParser::binaryValue(void)
{
	ParserPow pow(this, "binaryValue");
	Cardinal_1(pow, Tok_Binary);
//binaryValue						: Tok_Binary
}

void MibParser::hexadecimalValue(void)
{
	ParserPow pow(this, "hexadecimalValue");
	Cardinal_1(pow, Tok_Hexadecimal);
//hexadecimalValue				: Tok_Hexadecimal
}

void MibParser::stringValue(void)
{
	ParserPow pow(this, "stringValue");
	Cardinal_1(pow, Tok_String);
//stringValue						: Tok_String
}

void MibParser::bitOrObjectIdentifierValue(void)
{
	ParserPow pow(this, "bitOrObjectIdentifierValue");
	Cardinal_1(pow, Tok_LEFT_BRACE);
	Cardinal_0_n(pow, "nameValueComponent");
	Cardinal_1(pow, Tok_RIGHT_BRACE);
//bitOrObjectIdentifierValue					: Tok_LEFT_BRACE nameValueComponent * Tok_RIGHT_BRACE
}

void MibParser::nameValueComponent(void)
{
	ParserPow pow(this, "nameValueComponent");
	Cardinal_1(pow, "nameOrNumber");
	Cardinal_0_n(pow, Tok_COMMA, "nameOrNumber");
//nameValueComponent			: nameOrNumber ( Tok_COMMA nameOrNumber ) *
}

void MibParser::nameOrNumber(void)
{
	ParserPow pow(this, "nameOrNumber");
	Branch(pow, "nameOrNumber1", "nameOrNumber2");
//nameOrNumber					: nameOrNumber1 | nameOrNumber2
}

void MibParser::nameOrNumber1(void)
{
	ParserPow pow(this, "nameOrNumber1");
	Cardinal_1(pow, Tok_Integer);
	Call(pow);
//nameOrNumber					: Tok_Integer
}

void MibParser::nameOrNumber2(void)
{
	ParserPow pow(this, "nameOrNumber2");
	Cardinal_1(pow, "identifierName");
	Call(pow);
	Cardinal_0_1(pow, "nameAndNumber");
//nameOrNumber					: identifierName nameAndNumber ?
}

void MibParser::nameAndNumber(void)
{
	ParserPow pow(this, "nameAndNumber");
	Cardinal_1(pow, Tok_LEFT_PAREN);
	Branch(pow, "nameAndNumber1", "nameAndNumber2");
	Cardinal_1(pow, Tok_RIGHT_PAREN);
//nameAndNumber					: Tok_LEFT_PAREN ( Tok_Integer | definedValue ) Tok_RIGHT_PAREN
}

void MibParser::nameAndNumber1(void)
{
	ParserPow pow(this, "nameAndNumber1");
	Cardinal_1(pow, Tok_Integer);
	Call(pow);
//nameAndNumber1					: Tok_Integer
}

void MibParser::nameAndNumber2(void)
{
	ParserPow pow(this, "nameAndNumber2");
	Cardinal_1(pow, "definedValue");
	Call(pow);
//nameAndNumber2					: definedValue
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// * Macro Syntax definitions
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void MibParser::definedMacroType(void)
{
	ParserPow pow(this, "definedMacroType");
	Branch(pow, "macroModuleIdentity", "macroObjectIdentity", "macroObjectType", "macroNotificationType", "macroTrapType", "macroTextualConvention", "macroObjectGroup", "macroNotificationGroup", "macroModuleCompliance", "macroAgentCapabilities");
//definedMacroType				: macroModuleIdentity
//									| macroObjectIdentity
//									| macroObjectType
//									| macroNotificationType
//									| macroTrapType
//									| macroTextualConvention
//									| macroObjectGroup
//									| macroNotificationGroup
//									| macroModuleCompliance
//									| macroAgentCapabilities
}

void MibParser::definedMacroName(void)
{
	ParserPow pow(this, "definedMacroName");
	Branch(pow, Tok_MODULE_IDENTITY, Tok_OBJECT_IDENTITY, Tok_OBJECT_TYPE, Tok_NOTIFICATION_TYPE, Tok_TRAP_TYPE, Tok_TEXTUAL_CONVENTION, Tok_OBJECT_GROUP, Tok_NOTIFICATION_GROUP, Tok_MODULE_COMPLIANCE, Tok_AGENT_CAPABILITIES);
//definedMacroName				: Tok_MODULE_IDENTITY
//									| Tok_OBJECT_IDENTITY
//									| Tok_OBJECT_TYPE
//									| Tok_NOTIFICATION_TYPE
//									| Tok_TRAP_TYPE
//									| Tok_TEXTUAL_CONVENTION
//									| Tok_OBJECT_GROUP
//									| Tok_NOTIFICATION_GROUP
//									| Tok_MODULE_COMPLIANCE
//									| Tok_AGENT_CAPABILITIES
}

void MibParser::macroModuleIdentity(void)
{
	ParserPow pow(this, "macroModuleIdentity");
	Cardinal_1(pow, Tok_MODULE_IDENTITY);
	Call(pow);
	Cardinal_1(pow, "snmpUpdatePart", "snmpOrganizationPart", "snmpContactPart", "snmpDescrPart");
	Cardinal_0_n(pow, "snmpRevisionPart");
//macroModuleIdentity: Tok_MODULE_IDENTITY snmpUpdatePart snmpOrganizationPart snmpContactPart snmpDescrPart snmpRevisionPart *
}

void MibParser::macroObjectIdentity(void)
{
	ParserPow pow(this, "macroObjectIdentity");
	Cardinal_1(pow, Tok_OBJECT_IDENTITY);
	Call(pow);
	Cardinal_1(pow, "snmpStatusPart", "snmpDescrPart");
	Cardinal_0_1(pow, "snmpReferPart");
//macroObjectIdentity: Tok_OBJECT_IDENTITY snmpStatusPart snmpDescrPart snmpReferPart ?
}

void MibParser::macroObjectType(void)
{
	ParserPow pow(this, "macroObjectType");
	Cardinal_1(pow, Tok_OBJECT_TYPE);
	Call(pow);
	Cardinal_1(pow, "snmpSyntaxPart");
	Cardinal_0_1(pow, "snmpUnitsPart");
	Cardinal_1(pow, "snmpAccessPart", "snmpStatusPart");
	Cardinal_0_1(pow, "snmpDescrPart");
	Cardinal_0_1(pow, "snmpReferPart");
	Cardinal_0_1(pow, "snmpIndexPart");
	Cardinal_0_1(pow, "snmpDefValPart");
//macroObjectType		: Tok_OBJECT_TYPE snmpSyntaxPart snmpUnitsPart ? snmpAccessPart snmpStatusPart snmpDescrPart ? snmpReferPart ? snmpIndexPart ? snmpDefValPart ?
}

void MibParser::macroNotificationType(void)
{
	ParserPow pow(this, "macroNotificationType");
	Cardinal_1(pow, Tok_NOTIFICATION_TYPE);
	Call(pow);
	Cardinal_0_1(pow, "snmpObjectsPart");
	Cardinal_1(pow, "snmpStatusPart", "snmpDescrPart");
	Cardinal_0_1(pow, "snmpReferPart");
//macroNotificationType: Tok_NOTIFICATION_TYPE snmpObjectsPart ? snmpStatusPart snmpDescrPart snmpReferPart ?
}

void MibParser::macroTrapType(void)
{
	ParserPow pow(this, "macroTrapType");
	Cardinal_1(pow, Tok_TRAP_TYPE);
	Call(pow);
	Cardinal_1(pow, "snmpEnterprisePart");
	Cardinal_0_1(pow, "snmpVarPart");
	Cardinal_0_1(pow, "snmpDescrPart");
	Cardinal_0_1(pow, "snmpReferPart");
//macroTrapType		: Tok_TRAP_TYPE snmpEnterprisePart snmpVarPart ? snmpDescrPart ? snmpReferPart ?
}

void MibParser::macroTextualConvention(void)
{
	ParserPow pow(this, "macroTextualConvention");
	Cardinal_1(pow, Tok_TEXTUAL_CONVENTION);
	Call(pow);
	Cardinal_0_1(pow, "snmpDisplayPart");
	Cardinal_1(pow, "snmpStatusPart", "snmpDescrPart");
	Cardinal_0_1(pow, "snmpReferPart");
	Cardinal_1(pow, "snmpSyntaxPart");
//macroTextualConvention: Tok_TEXTUAL_CONVENTION snmpDisplayPart ? snmpStatusPart snmpDescrPart snmpReferPart ? snmpSyntaxPart
}

void MibParser::macroObjectGroup(void)
{
	ParserPow pow(this, "macroObjectGroup");
	Cardinal_1(pow, Tok_OBJECT_GROUP);
	Call(pow);
	Cardinal_1(pow, "snmpObjectsPart", "snmpStatusPart", "snmpDescrPart");
	Cardinal_0_1(pow, "snmpReferPart");
//macroObjectGroup	: Tok_OBJECT_GROUP snmpObjectsPart snmpStatusPart snmpDescrPart snmpReferPart ?
}

void MibParser::macroNotificationGroup(void)
{
	ParserPow pow(this, "macroNotificationGroup");
	Cardinal_1(pow, Tok_NOTIFICATION_GROUP);
	Call(pow);
	Cardinal_1(pow, "snmpNotificationsPart", "snmpStatusPart", "snmpDescrPart");
	Cardinal_0_1(pow, "snmpReferPart");
//macroNotificationGroup: Tok_NOTIFICATION_GROUP snmpNotificationsPart snmpStatusPart snmpDescrPart snmpReferPart ?
}

void MibParser::macroModuleCompliance(void)
{
	ParserPow pow(this, "macroModuleCompliance");
	Cardinal_1(pow, Tok_MODULE_COMPLIANCE);
	Call(pow);
	Cardinal_1(pow, "snmpStatusPart", "snmpDescrPart");
	Cardinal_0_1(pow, "snmpReferPart");
	Cardinal_1_n(pow, "snmpModulePart");
//macroModuleCompliance: Tok_MODULE_COMPLIANCE snmpStatusPart snmpDescrPart snmpReferPart ? snmpModulePart +
}

void MibParser::macroAgentCapabilities(void)
{
	ParserPow pow(this, "macroAgentCapabilities");
	Cardinal_1(pow, Tok_AGENT_CAPABILITIES);
	Call(pow);
	Cardinal_1(pow, "snmpProductReleasePart", "snmpStatusPart", "snmpDescrPart");
	Cardinal_0_1(pow, "snmpReferPart");
	Cardinal_0_n(pow, "snmpModuleSupportPart");
//macroAgentCapabilities: Tok_AGENT_CAPABILITIES snmpProductReleasePart snmpStatusPart snmpDescrPart snmpReferPart ? snmpModuleSupportPart *
}

void MibParser::snmpUpdatePart(void)
{
	ParserPow pow(this, "snmpUpdatePart");
	Cardinal_1(pow, Tok_LAST_UPDATED, Tok_String);
//snmpUpdatePart					: Tok_LAST_UPDATED Tok_String
}

void MibParser::snmpOrganizationPart(void)
{
	ParserPow pow(this, "snmpOrganizationPart");
	Cardinal_1(pow, Tok_ORGANIZATION, Tok_String);
//snmpOrganizationPart			: Tok_ORGANIZATION Tok_String
}

void MibParser::snmpContactPart(void)
{
	ParserPow pow(this, "snmpContactPart");
	Cardinal_1(pow, Tok_CONTACT_INFO, Tok_String);
//snmpContactPart				: Tok_CONTACT_INFO Tok_String
}

void MibParser::snmpDescrPart(void)
{
	ParserPow pow(this, "snmpDescrPart");
	Cardinal_1(pow, Tok_DESCRIPTION, Tok_String);
	Call(pow);
//snmpDescrPart					: Tok_DESCRIPTION Tok_String
}

void MibParser::snmpRevisionPart(void)
{
	ParserPow pow(this, "snmpRevisionPart");
	Cardinal_1(pow, Tok_REVISION, "value", Tok_DESCRIPTION, Tok_String);
//snmpRevisionPart				: Tok_REVISION value Tok_DESCRIPTION Tok_String
}

void MibParser::snmpStatusPart(void)
{
	ParserPow pow(this, "snmpStatusPart");
	Cardinal_1(pow, Tok_STATUS, "identifierName");
	Call(pow);
//snmpStatusPart					: Tok_STATUS identifierName
}

void MibParser::snmpReferPart(void)
{
	ParserPow pow(this, "snmpReferPart");
	Cardinal_1(pow, Tok_REFERENCE, Tok_String);
//snmpReferPart					: Tok_REFERENCE Tok_String
}

void MibParser::snmpSyntaxPart(void)
{
	ParserPow pow(this, "snmpSyntaxPart");
	Cardinal_1(pow, Tok_SYNTAX);
	Cardinal_1(pow, "type");
//snmpSyntaxPart					: Tok_SYNTAX type
}

void MibParser::snmpUnitsPart(void)
{
	ParserPow pow(this, "snmpUnitsPart");
	Cardinal_1(pow, Tok_UNITS, Tok_String);
//snmpUnitsPart					: Tok_UNITS Tok_String
}

void MibParser::snmpAccessPart(void)
{
	ParserPow pow(this, "snmpAccessPart");
	Branch(pow, Tok_ACCESS, Tok_MAX_ACCESS, Tok_MIN_ACCESS);
	Cardinal_1(pow, "identifierName");
	Call(pow);
//snmpAccessPart					: Tok_ACCESS identifierName
//									| Tok_MAX_ACCESS identifierName
//									| Tok_MIN_ACCESS identifierName
}

void MibParser::snmpIndexPart(void)
{
	ParserPow pow(this, "snmpIndexPart");
	Branch(pow, "snmpIndexPart1", "snmpIndexPart2");
//snmpIndexPart					: snmpIndexPart1 | snmpIndexPart2
}

void MibParser::snmpIndexPart1(void)
{
	ParserPow pow(this, "snmpIndexPart1");
	Cardinal_1(pow, Tok_INDEX, Tok_LEFT_BRACE, "indexValueList", Tok_RIGHT_BRACE);
//snmpIndexPart1					: Tok_INDEX Tok_LEFT_BRACE indexValueList Tok_RIGHT_BRACE
}

void MibParser::snmpIndexPart2(void)
{
	ParserPow pow(this, "snmpIndexPart2");
	Cardinal_1(pow, Tok_AUGMENTS, Tok_LEFT_BRACE, "value", Tok_RIGHT_BRACE);
//snmpIndexPart2					: Tok_AUGMENTS Tok_LEFT_BRACE value Tok_RIGHT_BRACE
}

void MibParser::indexValueList(void)
{
	ParserPow pow(this, "indexValueList");
	Cardinal_1(pow, "indexValue");
	Cardinal_0_n(pow, Tok_COMMA, "indexValue");
//indexValueList					: indexValue ( Tok_COMMA indexValue ) *
}

void MibParser::indexValue(void)
{
	ParserPow pow(this, "indexValue");
	Branch(pow, "indexValue1", "indexValue2");
//indexValue						: indexValue1 | indexValue2
}

void MibParser::indexValue1(void)
{
	ParserPow pow(this, "indexValue1");
	Cardinal_0_1(pow, Tok_IMPLIED);
	Cardinal_1(pow, "value");
//indexValue						: Tok_IMPLIED ? value
}

void MibParser::indexValue2(void)
{
	ParserPow pow(this, "indexValue2");
	Cardinal_1(pow, "indexType");
//indexValue						: indexType
}

void MibParser::indexType(void)
{
	ParserPow pow(this, "indexType");
	Branch(pow, "integerType", "stringType", "objectIdentifierType", "typeName");
//indexType						: integerType
//									| stringType
//									| objectIdentifierType
//									| typeName
}

void MibParser::snmpDefValPart(void)
{
	ParserPow pow(this, "snmpDefValPart");
	Cardinal_1(pow, Tok_DEFVAL, Tok_LEFT_BRACE, "value", Tok_RIGHT_BRACE);
//snmpDefValPart					: Tok_DEFVAL Tok_LEFT_BRACE value Tok_RIGHT_BRACE
}

void MibParser::snmpObjectsPart(void)
{
	ParserPow pow(this, "snmpObjectsPart");
	Cardinal_1(pow, Tok_OBJECTS, Tok_LEFT_BRACE, "valueList", Tok_RIGHT_BRACE);
//snmpObjectsPart				: Tok_OBJECTS Tok_LEFT_BRACE valueList Tok_RIGHT_BRACE
}

void MibParser::valueList(void)
{
	ParserPow pow(this, "valueList");
	Cardinal_1(pow, "value");
	Cardinal_0_n(pow, Tok_COMMA, "value");
//valueList						: value ( Tok_COMMA value ) *
}

void MibParser::snmpEnterprisePart(void)
{
	ParserPow pow(this, "snmpEnterprisePart");
	Cardinal_1(pow, Tok_ENTERPRISE, "value");
//snmpEnterprisePart			: Tok_ENTERPRISE value
}

void MibParser::snmpVarPart(void)
{
	ParserPow pow(this, "snmpVarPart");
	Cardinal_1(pow, Tok_VARIABLES, Tok_LEFT_BRACE, "valueList", Tok_RIGHT_BRACE);
//snmpVarPart						: Tok_VARIABLES Tok_LEFT_BRACE valueList Tok_RIGHT_BRACE
}

void MibParser::snmpDisplayPart(void)
{
	ParserPow pow(this, "snmpDisplayPart");
	Cardinal_1(pow, Tok_DISPLAY_HINT, Tok_String);
//snmpDisplayPart				: Tok_DISPLAY_HINT Tok_String
}

void MibParser::snmpNotificationsPart(void)
{
	ParserPow pow(this, "snmpNotificationsPart");
	Cardinal_1(pow, Tok_NOTIFICATIONS, Tok_LEFT_BRACE, "valueList", Tok_RIGHT_BRACE);
//snmpNotificationsPart		: Tok_NOTIFICATIONS Tok_LEFT_BRACE valueList Tok_RIGHT_BRACE
}

void MibParser::snmpModulePart(void)
{
	ParserPow pow(this, "snmpModulePart");
	Cardinal_1(pow, Tok_MODULE);
	Cardinal_0_1(pow, "snmpModuleImport");
	Cardinal_0_1(pow, "snmpMandatoryPart");
	Cardinal_0_n(pow, "snmpCompliancePart");
//snmpModulePart					: Tok_MODULE snmpModuleImport ? snmpMandatoryPart ? snmpCompliancePart *
}

void MibParser::snmpModuleImport(void)
{
	ParserPow pow(this, "snmpModuleImport");
	Cardinal_1(pow, "moduleName");
//snmpModuleImport				: moduleName
}

void MibParser::snmpMandatoryPart(void)
{
	ParserPow pow(this, "snmpMandatoryPart");
	Cardinal_1(pow, Tok_MANDATORY_GROUPS, Tok_LEFT_BRACE, "valueList", Tok_RIGHT_BRACE);
//snmpMandatoryPart				: Tok_MANDATORY_GROUPS Tok_LEFT_BRACE valueList Tok_RIGHT_BRACE
}

void MibParser::snmpCompliancePart(void)
{
	ParserPow pow(this, "snmpCompliancePart");
	Branch(pow, "complianceGroup", "complianceObject");
//snmpCompliancePart			: complianceGroup
//									| complianceObject
}

void MibParser::complianceGroup(void)
{
	ParserPow pow(this, "complianceGroup");
	Cardinal_1(pow, Tok_GROUP, "value", "snmpDescrPart");
//complianceGroup				: Tok_GROUP value snmpDescrPart
}

void MibParser::complianceObject(void)
{
	ParserPow pow(this, "complianceObject");
	Cardinal_1(pow, Tok_OBJECT, "value");
	Cardinal_0_1(pow, "snmpSyntaxPart");
	Cardinal_0_1(pow, "snmpWriteSyntaxPart");
	Cardinal_0_1(pow, "snmpAccessPart");
	Cardinal_1(pow, "snmpDescrPart");
//complianceObject				: Tok_OBJECT value snmpSyntaxPart ? snmpWriteSyntaxPart ? snmpAccessPart ? snmpDescrPart
}

void MibParser::snmpWriteSyntaxPart(void)
{
	ParserPow pow(this, "snmpWriteSyntaxPart");
	Cardinal_1(pow, Tok_WRITE_SYNTAX, "type");
//snmpWriteSyntaxPart			: Tok_WRITE_SYNTAX type
}

void MibParser::snmpProductReleasePart(void)
{
	ParserPow pow(this, "snmpProductReleasePart");
	Cardinal_1(pow, Tok_PRODUCT_RELEASE, Tok_String);
//snmpProductReleasePart		: Tok_PRODUCT_RELEASE Tok_String
}

void MibParser::snmpModuleSupportPart(void)
{
	ParserPow pow(this, "snmpModuleSupportPart");
	Cardinal_1(pow, Tok_SUPPORTS, "snmpModuleImport", Tok_INCLUDES, Tok_LEFT_BRACE, "valueList", Tok_RIGHT_BRACE);
	Cardinal_0_n(pow, "snmpVariationPart");
//snmpModuleSupportPart		: Tok_SUPPORTS snmpModuleImport Tok_INCLUDES Tok_LEFT_BRACE valueList Tok_RIGHT_BRACE snmpVariationPart *
}

void MibParser::snmpVariationPart(void)
{
	ParserPow pow(this, "snmpVariationPart");
	Cardinal_1(pow, Tok_VARIATION, "value");
	Cardinal_0_1(pow, "snmpSyntaxPart");
	Cardinal_0_1(pow, "snmpWriteSyntaxPart");
	Cardinal_0_1(pow, "snmpAccessPart");
	Cardinal_0_1(pow, "snmpCreationPart");
	Cardinal_0_1(pow, "snmpDefValPart");
	Cardinal_1(pow, "snmpDescrPart");
//snmpVariationPart				: Tok_VARIATION value snmpSyntaxPart ? snmpWriteSyntaxPart ? snmpAccessPart ? snmpCreationPart ? snmpDefValPart ? snmpDescrPart
}

void MibParser::snmpCreationPart(void)
{
	ParserPow pow(this, "snmpCreationPart");
	Cardinal_1(pow, Tok_CREATION_REQUIRES, Tok_LEFT_BRACE);
	Cardinal_0_1(pow, "valueList");
	Cardinal_1(pow, Tok_RIGHT_BRACE);
//snmpCreationPart				: Tok_CREATION_REQUIRES Tok_LEFT_BRACE valueList ? Tok_RIGHT_BRACE
}

