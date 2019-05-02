#include "MibParser.h"
#include "Debug.h"
#include "ParserPow.h"

MibParser::MibParser(ParserAction & Callback,
							const QString & name)
: ParserGeneric(Callback)
	, m_lexer(name)
{
	_generate();
	Finalize();
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
	valueConstraintListOpt();
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
	macroTextualConvention();
	macroObjectGroup();
	macroNotificationGroup();
	macroModuleCompliance();
	macroAgentCapabilities();
	macroTrapType();
	macroModuleConformance();
	snmpLastUpdated();
	snmpOrganization();
	snmpContactInfo();
	snmpDescription();
	snmpRevision();
	snmpStatus();
	snmpAccess();
	snmpReference();
	snmpSyntax();
	snmpWriteSyntax();
	snmpUnits();
	snmpDisplayHint();
	snmpProductRelease();
	snmpCommaValues();

	snmpIndex();
	snmpIndex1();
	snmpIndex2();
	snmpIndex3();
	snmpIndexTypes();
	snmpIndexType();
	snmpDefVal();
	snmpObjects();
	snmpNotifications();
	snmpComplianceModule();
	snmpModuleName();
	snmpModuleIdentifier();
	snmpComplianceMandatory();
	snmpCompliance();
	snmpComplianceGroup();
	snmpComplianceObject();
	snmpAgentModule();
	snmpAgentSupport();
	snmpAgentInclude();
	snmpAgentVariation();
	snmpAgentObjectVariation();
	snmpCreationRequire();
	snmpEnterprise();
	snmpVariables();

	// check parser completude
	CheckParser();
}

void MibParser::mibFile(void)
{
	ParserPow pow(this, ParserItem::Root());	// mibFile is the root of the parser
	Cardinal_1(pow, "mibModule", Tok_EndOfFile);
//mibFile: mibModule
}

void MibParser::moduleName(void)
{
	ParserPow pow(this, "moduleName");
	Cardinal_1(pow, Tok_Uppercase);
	Call(pow);
//moduleName: Tok_Uppercase
}

void MibParser::typeName(void)
{
	ParserPow pow(this, "typeName");
	Cardinal_1(pow, Tok_Uppercase);
	Call(pow);
//typeName: Tok_Uppercase
}

void MibParser::valueName(void)
{
	ParserPow pow(this, "valueName");
	Cardinal_1(pow, Tok_Lowercase);
	Call(pow);
//valueName: Tok_Lowercase
}

void MibParser::identifierName(void)
{
	ParserPow pow(this, "identifierName");
	Cardinal_1(pow, Tok_Lowercase);
	Call(pow);
//identifierName: Tok_Lowercase
}

void MibParser::anyName(void)
{
	ParserPow pow(this, "anyName");
	Branch(pow, Tok_Lowercase, Tok_Uppercase);
	Call(pow);
//anyName: Tok_Lowercase
//			| Tok_Uppercase
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
//mibModule: moduleName bitOrObjectIdentifierValue ? Tok_DEFINITIONS tagDefault ? Tok_ASSIGN Tok_BEGIN moduleBody Tok_END
}

void MibParser::tagDefault(void)
{
	ParserPow pow(this, "tagDefault");
	Branch(pow, Tok_EXPLICIT, Tok_IMPLICIT);
	Cardinal_1(pow, Tok_TAGS);
//tagDefault: ( Tok_EXPLICIT | Tok_IMPLICIT ) Tok_TAGS
}

void MibParser::moduleBody(void)
{
	ParserPow pow(this, "moduleBody");
	Cardinal_0_1(pow, "exportList");
	Cardinal_0_1(pow, "importList");
	Cardinal_1(pow, "assignmentList");
//moduleBody: exportList ? importList ? assignmentList
}

void MibParser::exportList(void)
{
	ParserPow pow(this, "exportList");
	Cardinal_1(pow, Tok_EXPORTS);
	Cardinal_1_n(pow, "symbolList");
	Cardinal_1(pow, Tok_SEMI_COLON);
//exportList: Tok_EXPORTS symbolList + Tok_SEMI_COLON
}

void MibParser::importList(void)
{
	ParserPow pow(this, "importList");
	Cardinal_1(pow, Tok_IMPORTS);
	Call(pow);
	Cardinal_1_n(pow, "symbolsFromModule");
	Cardinal_1(pow, Tok_SEMI_COLON);
	Call(pow);
//importList: Tok_IMPORTS symbolsFromModule + Tok_SEMI_COLON
}

void MibParser::symbolsFromModule(void)
{
	ParserPow pow(this, "symbolsFromModule");
	Cardinal_1(pow, "symbolList", Tok_FROM);
	Cardinal_1(pow, "moduleName");
	Call(pow);
//symbolsFromModule: symbolList Tok_FROM moduleName
}

void MibParser::symbolList(void)
{
	ParserPow pow(this, "symbolList");
	Cardinal_1(pow, "symbol");
	Cardinal_0_n(pow, Tok_COMMA, "symbol");
//symbolList: symbol ( Tok_COMMA symbol ) *
}

void MibParser::symbol(void)
{
	ParserPow pow(this, "symbol");
	Branch(pow, "anyName", "definedMacroName");
//symbol: anyName
//		  | definedMacroName
	Call(pow);
}

void MibParser::assignmentList(void)
{
	ParserPow pow(this, "assignmentList");
	Cardinal_0_n(pow, "assignment");
//assignmentList: assignment +
}

void MibParser::assignment(void)
{
	ParserPow pow(this, "assignment");
	Branch(pow, "typeAssignment", "valueAssignment", "macroDefinition");
//assignment: typeAssignment
//				| valueAssignment
//				| macroDefinition
}

void MibParser::macroDefinition(void)
{
	ParserPow pow(this, "macroDefinition");
	Cardinal_1(pow, "macroReference", Tok_MACRO, Tok_ASSIGN, "macroBody");
//macroDefinition: macroReference Tok_MACRO Tok_ASSIGN macroBody
}

void MibParser::macroReference(void)
{
	ParserPow pow(this, "macroReference");
	Cardinal_1(pow, "definedMacroName");
//macroReference: definedMacroName
}

void MibParser::macroBody(void)
{
	ParserPow pow(this, "macroBody");
	Cardinal_1(pow, Tok_BEGIN);
	Greedy(pow, Tok_END);
	Cardinal_1(pow, Tok_END);
//macroBody: Tok_BEGIN .* Tok_END
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
//typeAssignment: typeName Tok_ASSIGN type
}

void MibParser::type(void)
{
	ParserPow pow(this, "type");
	Cardinal_0_1(pow, "tag");
	Cardinal_0_1(pow, "explicitOrImplicitTag");
	Cardinal_1(pow, "type1");
//type: tag ? explicitOrImplicitTag ? type1
}

void MibParser::type1(void)
{
	ParserPow pow(this, "type1");
	Call(pow);
	Branch(pow, "builtinType", "definedType", "definedMacroType");
//type: builtinType
//		| definedType
//		| definedMacroType
	Call(pow);
}

void MibParser::definedType(void)
{
	ParserPow pow(this, "definedType");
	Cardinal_1(pow, "typeName");
	Call(pow);
	Cardinal_0_1(pow, "valueOrConstraintList");
//definedType: typeName valueOrConstraintList ?
}

void MibParser::builtinType(void)
{
	ParserPow pow(this, "builtinType");
	Branch(pow, "nullType", "booleanType", "realType", "integerType", "objectIdentifierType", "stringType", "bitStringType", "bitsType", "sequence", "set", "choiceType", "enumeratedType", "selectionType", "anyType");
//builtinType: nullType
//				 | booleanType
//				 | realType
//				 | integerType
//				 | objectIdentifierType
//				 | stringType
//				 | bitStringType
//				 | bitsType
//				 | sequence
//				 | set
//				 | choiceType
//				 | enumeratedType
//				 | selectionType
//				 | anyType
}

void MibParser::nullType(void)
{
	ParserPow pow(this, "nullType");
	Cardinal_1(pow, Tok_NULL);
	Call(pow);
//nullType: Tok_NULL
}

void MibParser::booleanType(void)
{
	ParserPow pow(this, "booleanType");
	Cardinal_1(pow, Tok_BOOLEAN);
	Call(pow);
//booleanType: Tok_BOOLEAN
}

void MibParser::realType(void)
{
	ParserPow pow(this, "realType");
	Cardinal_1(pow, Tok_REAL);
	Call(pow);
//realType: Tok_REAL
}

void MibParser::integerType(void)
{
	ParserPow pow(this, "integerType");
	Cardinal_1(pow, Tok_INTEGER);
	Call(pow);
	Cardinal_0_1(pow, "valueOrConstraintList");
//integerType: Tok_INTEGER valueOrConstraintList ?
}

void MibParser::objectIdentifierType(void)
{
	ParserPow pow(this, "objectIdentifierType");
	Cardinal_1(pow, Tok_OBJECT, Tok_IDENTIFIER);
	Call(pow);
//objectIdentifierType: Tok_OBJECT Tok_IDENTIFIER
}

void MibParser::stringType(void)
{
	ParserPow pow(this, "stringType");
	Cardinal_1(pow, Tok_OCTET, Tok_STRING);
	Call(pow);
	Cardinal_0_1(pow, "constraintList");
//stringType: Tok_OCTET Tok_STRING constraintList ?
}

void MibParser::bitStringType(void)
{
	ParserPow pow(this, "bitStringType");
	Cardinal_1(pow, Tok_BIT, Tok_STRING);
	Call(pow);
	Cardinal_0_1(pow, "valueOrConstraintList");
//bitStringType: Tok_BIT Tok_STRING valueOrConstraintList ?
}

void MibParser::bitsType(void)
{
	ParserPow pow(this, "bitsType");
	Cardinal_1(pow, Tok_BITS);
	Call(pow);
	Cardinal_0_1(pow, "valueOrConstraintList");
//bitsType: Tok_BITS valueOrConstraintList ?
}

void MibParser::sequence(void)
{
	ParserPow pow(this, "sequence");
	Cardinal_1(pow, Tok_SEQUENCE);
	Branch(pow, "sequenceType", "sequenceOfType");
//sequence: Tok_SEQUENCE ( sequenceType | sequenceOfType )
}

void MibParser::sequenceType(void)
{
	ParserPow pow(this, "sequenceType");
	Cardinal_1(pow, Tok_LEFT_BRACE);
	Call(pow);
	Cardinal_0_1(pow, "elementTypeList");
	Cardinal_1(pow, Tok_RIGHT_BRACE);
//sequenceType: Tok_LEFT_BRACE elementTypeList ? Tok_RIGHT_BRACE
}

void MibParser::sequenceOfType(void)
{
	ParserPow pow(this, "sequenceOfType");
	Cardinal_0_1(pow, "constraintList");
	Cardinal_1(pow, Tok_OF);
	Call(pow);
	Cardinal_1(pow, "type");
//sequenceOfType: constraintList ? Tok_OF type
}

void MibParser::set(void)
{
	ParserPow pow(this, "set");
	Cardinal_1(pow, Tok_SET);
	Branch(pow, "setType", "setOfType");
//set: Tok_SET ( setType | setOfType )
}

void MibParser::setType(void)
{
	ParserPow pow(this, "setType");
	Cardinal_1(pow, Tok_LEFT_BRACE);
	Call(pow);
	Cardinal_0_1(pow, "elementTypeList");
	Cardinal_1(pow, Tok_RIGHT_BRACE);
//setType: Tok_LEFT_BRACE elementTypeList ? Tok_RIGHT_BRACE
}

void MibParser::setOfType(void)
{
	ParserPow pow(this, "setOfType");
	Cardinal_0_1(pow, "sizeConstraint");
	Cardinal_1(pow, Tok_OF);
	Call(pow);
	Cardinal_1(pow, "type");
//setOfType: sizeConstraint ? Tok_OF type
}

void MibParser::choiceType(void)
{
	ParserPow pow(this, "choiceType");
	Cardinal_1(pow, Tok_CHOICE);
	Call(pow);
	Cardinal_1(pow, Tok_LEFT_BRACE, "elementTypeList", Tok_RIGHT_BRACE);
//choiceType: Tok_CHOICE Tok_LEFT_BRACE elementTypeList Tok_RIGHT_BRACE
}

void MibParser::enumeratedType(void)
{
	ParserPow pow(this, "enumeratedType");
	Cardinal_1(pow, Tok_ENUMERATED);
	Call(pow);
	Cardinal_1(pow, "namedNumberList");
//enumeratedType: Tok_ENUMERATED namedNumberList
}

void MibParser::selectionType(void)
{
	ParserPow pow(this, "selectionType");
	Cardinal_1(pow, "identifierName");
	Call(pow);
	Cardinal_1(pow, Tok_LESS, "type");
//selectionType: identifierName Tok_LESS type
}

void MibParser::tag(void)
{
	ParserPow pow(this, "tag");
	Cardinal_1(pow, Tok_LEFT_BRACKET);
	Cardinal_0_1(pow, "classtag");
	Cardinal_1(pow, Tok_Integer, Tok_RIGHT_BRACKET);
//tag: Tok_LEFT_BRACKET class ? Tok_Integer Tok_RIGHT_BRACKET
}

void MibParser::classtag(void)
{
	ParserPow pow(this, "classtag");
	Branch(pow, Tok_UNIVERSAL, Tok_APPLICATION, Tok_PRIVATE);
//classtag: Tok_UNIVERSAL
//			 | Tok_APPLICATION
//			 | Tok_PRIVATE
}

void MibParser::explicitOrImplicitTag(void)
{
	ParserPow pow(this, "explicitOrImplicitTag");
	Branch(pow, Tok_EXPLICIT, Tok_IMPLICIT);
//explicitOrImplicitTag: Tok_EXPLICIT
//							  | Tok_IMPLICIT
}

void MibParser::anyType(void)
{
	ParserPow pow(this, "anyType");
	Cardinal_1(pow, Tok_ANY);
	Call(pow);
	Cardinal_0_1(pow, Tok_DEFINED, Tok_BY, "anyName");
//anyType: Tok_ANY ( Tok_DEFINED Tok_BY anyName ) ?
}

void MibParser::elementTypeList(void)
{
	ParserPow pow(this, "elementTypeList");
	Cardinal_1(pow, "elementType");
	Cardinal_0_n(pow, Tok_COMMA, "elementType");
//elementTypeList: elementType ( Tok_COMMA elementType ) *
}

void MibParser::elementType(void)
{
	ParserPow pow(this, "elementType");
	Cardinal_1(pow, "identifierName");
	Branch(pow, "elementType1", "elementType2");
//elementType: identifierName ( elementType1 | elementType2 )
}

void MibParser::elementType1(void)
{
	ParserPow pow(this, "elementType1");
	Cardinal_1(pow, "type");
	Cardinal_0_1(pow, "optionalOrDefaultElement");
//elementType1: type optionalOrDefaultElement ?
}

void MibParser::elementType2(void)
{
	ParserPow pow(this, "elementType2");
	Cardinal_1(pow, Tok_COMPONENTS, Tok_OF, "type");
//elementType2: Tok_COMPONENTS Tok_OF type
}

void MibParser::optionalOrDefaultElement(void)
{
	ParserPow pow(this, "optionalOrDefaultElement");
	Branch(pow, "optionalOrDefaultElement1", "optionalOrDefaultElement2");
//optionalOrDefaultElement: optionalOrDefaultElement1 | optionalOrDefaultElement2
}

void MibParser::optionalOrDefaultElement1(void)
{
	ParserPow pow(this, "optionalOrDefaultElement1");
	Cardinal_1(pow, Tok_OPTIONAL);
//optionalOrDefaultElement: Tok_OPTIONAL
}

void MibParser::optionalOrDefaultElement2(void)
{
	ParserPow pow(this, "optionalOrDefaultElement2");
	Cardinal_1(pow, Tok_DEFAULT);
	Cardinal_0_1(pow, "identifierName");
	Cardinal_1(pow, "value");
//optionalOrDefaultElement: Tok_DEFAULT identifierName ? value
}

void MibParser::valueOrConstraintList(void)
{
	ParserPow pow(this, "valueOrConstraintList");
	Branch(pow, "namedNumberList", "constraintList");
//valueOrConstraintList: namedNumberList
//							  | constraintList
}

void MibParser::namedNumberList(void)
{
	ParserPow pow(this, "namedNumberList");
	Cardinal_1(pow, Tok_LEFT_BRACE, "namedNumber");
	Cardinal_0_n(pow, Tok_COMMA, "namedNumber");
	Cardinal_1(pow, Tok_RIGHT_BRACE);
//namedNumberList: Tok_LEFT_BRACE namedNumber ( Tok_COMMA namedNumber ) * Tok_RIGHT_BRACE
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
//namedNumber: identifierName Tok_LEFT_PAREN number Tok_RIGHT_PAREN
}

void MibParser::number(void)
{
	ParserPow pow(this, "number");
	Branch(pow, "numberValue", "binaryValue", "hexadecimalValue", "definedValue");
//number: numberValue
//		 | binaryValue
//		 | hexadecimalValue
//		 | definedValue
}

void MibParser::constraintList(void)
{
	ParserPow pow(this, "constraintList");
	Cardinal_1(pow, Tok_LEFT_PAREN, "constraint");
	Cardinal_0_n(pow, Tok_VERTICAL_BAR, "constraint");
	Cardinal_1(pow, Tok_RIGHT_PAREN);
//constraintList: Tok_LEFT_PAREN constraint ( Tok_VERTICAL_BAR constraint ) * Tok_RIGHT_PAREN
}

void MibParser::constraint(void)
{
	ParserPow pow(this, "constraint");
	Call(pow);
	Branch(pow, "valueConstraint", "sizeConstraint", "alphabetConstraint", "containedTypeConstraint", "innerTypeConstraint");
//constraint: valueConstraint
//				| sizeConstraint
//				| alphabetConstraint
//				| containedTypeConstraint
//				| innerTypeConstraint
}

void MibParser::valueConstraintList(void)
{
	ParserPow pow(this, "valueConstraintList");
	Cardinal_1(pow, Tok_LEFT_PAREN, "valueConstraint");
	Cardinal_0_n(pow, "valueConstraintListOpt");
	Cardinal_1(pow, Tok_RIGHT_PAREN);
//valueConstraintList: Tok_LEFT_PAREN valueConstraint ( valueConstraintList ) * Tok_RIGHT_PAREN
}

void MibParser::valueConstraintListOpt(void)
{
	ParserPow pow(this, "valueConstraintListOpt");
	Cardinal_1(pow, Tok_VERTICAL_BAR);
	Call(pow);
	Cardinal_1(pow, "valueConstraint");
//valueConstraintListOpt: Tok_VERTICAL_BAR valueConstraint
}


void MibParser::valueConstraint(void)
{
	ParserPow pow(this, "valueConstraint");
	Cardinal_1(pow, "lowerEndPoint");
	Cardinal_0_1(pow, "valueRange");
//valueConstraint: lowerEndPoint valueRange ?
}

void MibParser::valueRange(void)
{
	ParserPow pow(this, "valueRange");
	Cardinal_0_1(pow, Tok_LESS);
	Cardinal_1(pow, Tok_DOUBLE_DOT);
	Cardinal_0_1(pow, Tok_LESS);
	Cardinal_1(pow, "upperEndPoint");
//valueRange: Tok_LESS ? Tok_DOUBLE_DOT Tok_LESS ? upperEndPoint
}

void MibParser::lowerEndPoint(void)
{
	ParserPow pow(this, "lowerEndPoint");
	Branch(pow, "value", Tok_MIN);
	Call(pow);
//lowerEndPoint: value
//					| Tok_MIN
}

void MibParser::upperEndPoint(void)
{
	ParserPow pow(this, "upperEndPoint");
	Branch(pow, "value", Tok_MAX);
	Call(pow);
//upperEndPoint: value
//					| Tok_MAX
}

void MibParser::sizeConstraint(void)
{
	ParserPow pow(this, "sizeConstraint");
	Cardinal_1(pow, Tok_SIZE);
	Call(pow);
	Cardinal_1(pow, "valueConstraintList");
	Call(pow);
//sizeConstraint: Tok_SIZE valueConstraintList
}

void MibParser::alphabetConstraint(void)
{
	ParserPow pow(this, "alphabetConstraint");
	Cardinal_1(pow, Tok_FROM, "valueConstraintList");
//alphabetConstraint: Tok_FROM valueConstraintList
}

void MibParser::containedTypeConstraint(void)
{
	ParserPow pow(this, "containedTypeConstraint");
	Cardinal_1(pow, Tok_INCLUDES, "type");
//containedTypeConstraint: Tok_INCLUDES type
}

void MibParser::innerTypeConstraint(void)
{
	ParserPow pow(this, "innerTypeConstraint");
	Branch(pow, "innerTypeConstraint1", "innerTypeConstraint2");
//innerTypeConstraint: innerTypeConstraint1 | innerTypeConstraint2
}

void MibParser::innerTypeConstraint1(void)
{
	ParserPow pow(this, "innerTypeConstraint1");
	Cardinal_1(pow, Tok_WITH, Tok_COMPONENT, "valueOrConstraintList");
//innerTypeConstraint1: Tok_WITH Tok_COMPONENT valueOrConstraintList
}

void MibParser::innerTypeConstraint2(void)
{
	ParserPow pow(this, "innerTypeConstraint2");
	Cardinal_1(pow, Tok_WITH, Tok_COMPONENTS, "componentsList");
//innerTypeConstraint2: Tok_WITH Tok_COMPONENTS componentsList
}

void MibParser::componentsList(void)
{
	ParserPow pow(this, "componentsList");
	Branch(pow, "componentsList1", "componentsList2");
//componentsList: componentsList1 | componentsList2
}

void MibParser::componentsList1(void)
{
	ParserPow pow(this, "componentsList1");
	Cardinal_1(pow, Tok_LEFT_BRACE, "componentConstraint");
	Cardinal_0_n(pow, "componentsListTail");
	Cardinal_1(pow, Tok_RIGHT_BRACE);
//componentsList1: Tok_LEFT_BRACE componentConstraint componentsListTail * Tok_RIGHT_BRACE
}

void MibParser::componentsList2(void)
{
	ParserPow pow(this, "componentsList2");
	Cardinal_1(pow, Tok_LEFT_BRACE, Tok_ELLIPSIS);
	Cardinal_1_n(pow, "componentsListTail");
	Cardinal_1(pow, Tok_RIGHT_BRACE);
//componentsList2: Tok_LEFT_BRACE Tok_ELLIPSIS componentsListTail + Tok_RIGHT_BRACE
}

void MibParser::componentsListTail(void)
{
	ParserPow pow(this, "componentsListTail");
	Cardinal_1(pow, Tok_COMMA);
	Cardinal_0_1(pow, "componentConstraint");
//componentsListTail: Tok_COMMA componentConstraint ?
}

void MibParser::componentConstraint(void)
{
	ParserPow pow(this, "componentConstraint");
	Branch(pow, "componentConstraint1", "componentConstraint2");
//componentConstraint: componentConstraint1 | componentConstraint2
}

void MibParser::componentConstraint1(void)
{
	ParserPow pow(this, "componentConstraint1");
	Cardinal_1(pow, "identifierName");
	Cardinal_0_1(pow, "componentValuePresence");
//componentConstraint1: identifierName componentValuePresence ?
}

void MibParser::componentConstraint2(void)
{
	ParserPow pow(this, "componentConstraint2");
	Cardinal_1(pow, "componentValuePresence");
//componentConstraint2: componentValuePresence
}

void MibParser::componentValuePresence(void)
{
	ParserPow pow(this, "componentValuePresence");
	Branch(pow, "componentValuePresence1", "componentValuePresence2");
//componentValuePresence: componentValuePresence1 | componentValuePresence2
}

void MibParser::componentValuePresence1(void)
{
	ParserPow pow(this, "componentValuePresence1");
	Cardinal_1(pow, "valueOrConstraintList");
	Cardinal_0_1(pow, "componentPresence");
//componentValuePresence1: valueOrConstraintList componentPresence ?
}

void MibParser::componentValuePresence2(void)
{
	ParserPow pow(this, "componentValuePresence2");
	Cardinal_1(pow, "componentPresence");
//componentValuePresence2: componentPresence
}

void MibParser::componentPresence(void)
{
	ParserPow pow(this, "componentPresence");
	Branch(pow, Tok_PRESENT, Tok_ABSENT, Tok_OPTIONAL);
//componentPresence: Tok_PRESENT
//						 | Tok_ABSENT
//						 | Tok_OPTIONAL
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
	Call(pow);
	//valueAssignment: valueName type Tok_ASSIGN value
}

void MibParser::value(void)
{
	ParserPow pow(this, "value");
	Branch(pow, "builtinValue", "definedValue");
//value: builtinValue
//		 | definedValue
}

void MibParser::definedValue(void)
{
	ParserPow pow(this, "definedValue");
	Cardinal_1(pow, "valueName");
//definedValue: valueName
}

void MibParser::builtinValue(void)
{
	ParserPow pow(this, "builtinValue");
	Branch(pow, "nullValue", "booleanValue", "specialRealValue", "numberValue", "binaryValue", "hexadecimalValue", "stringValue", "bitOrObjectIdentifierValue");
//builtinValue: nullValue
//				  | booleanValue
//				  | specialRealValue
//				  | numberValue
//				  | binaryValue
//				  | hexadecimalValue
//				  | stringValue
//				  | bitOrObjectIdentifierValue
}

void MibParser::nullValue(void)
{
	ParserPow pow(this, "nullValue");
	Cardinal_1(pow, Tok_NULL);
//nullValue: Tok_NULL
}

void MibParser::booleanValue(void)
{
	ParserPow pow(this, "booleanValue");
	Branch(pow, Tok_TRUE, Tok_FALSE);
//booleanValue: Tok_TRUE
//				  | Tok_FALSE
}

void MibParser::specialRealValue(void)
{
	ParserPow pow(this, "specialRealValue");
	Branch(pow, Tok_PLUS_INFINITY, Tok_MINUS_INFINITY);
//specialRealValue: Tok_PLUS_INFINITY
//						| Tok_MINUS_INFINITY
}

void MibParser::numberValue(void)
{
	ParserPow pow(this, "numberValue");
	Cardinal_1(pow, Tok_Integer);
	Call(pow);
//numberValue: Tok_Integer
}

void MibParser::binaryValue(void)
{
	ParserPow pow(this, "binaryValue");
	Cardinal_1(pow, Tok_Binary);
//binaryValue: Tok_Binary
}

void MibParser::hexadecimalValue(void)
{
	ParserPow pow(this, "hexadecimalValue");
	Cardinal_1(pow, Tok_Hexadecimal);
//hexadecimalValue: Tok_Hexadecimal
}

void MibParser::stringValue(void)
{
	ParserPow pow(this, "stringValue");
	Cardinal_1(pow, Tok_String);
//stringValue: Tok_String
}

void MibParser::bitOrObjectIdentifierValue(void)
{
	ParserPow pow(this, "bitOrObjectIdentifierValue");
	Cardinal_1(pow, Tok_LEFT_BRACE);
	Cardinal_0_n(pow, "nameValueComponent");
	Cardinal_1(pow, Tok_RIGHT_BRACE);
//bitOrObjectIdentifierValue: Tok_LEFT_BRACE nameValueComponent * Tok_RIGHT_BRACE
}

void MibParser::nameValueComponent(void)
{
	ParserPow pow(this, "nameValueComponent");
	Cardinal_1(pow, "nameOrNumber");
	Cardinal_0_n(pow, Tok_COMMA, "nameOrNumber");
//nameValueComponent: nameOrNumber ( Tok_COMMA nameOrNumber ) *
}

void MibParser::nameOrNumber(void)
{
	ParserPow pow(this, "nameOrNumber");
	Branch(pow, "nameOrNumber1", "nameOrNumber2");
//nameOrNumber: nameOrNumber1 | nameOrNumber2
}

void MibParser::nameOrNumber1(void)
{
	ParserPow pow(this, "nameOrNumber1");
	Cardinal_1(pow, Tok_Integer);
	Call(pow);
//nameOrNumber: Tok_Integer
}

void MibParser::nameOrNumber2(void)
{
	ParserPow pow(this, "nameOrNumber2");
	Cardinal_1(pow, "identifierName");
	Call(pow);
	Cardinal_0_1(pow, "nameAndNumber");
//nameOrNumber: identifierName nameAndNumber ?
}

void MibParser::nameAndNumber(void)
{
	ParserPow pow(this, "nameAndNumber");
	Cardinal_1(pow, Tok_LEFT_PAREN);
	Branch(pow, "nameAndNumber1", "nameAndNumber2");
	Cardinal_1(pow, Tok_RIGHT_PAREN);
//nameAndNumber: Tok_LEFT_PAREN ( Tok_Integer | definedValue ) Tok_RIGHT_PAREN
}

void MibParser::nameAndNumber1(void)
{
	ParserPow pow(this, "nameAndNumber1");
	Cardinal_1(pow, Tok_Integer);
	Call(pow);
//nameAndNumber1: Tok_Integer
}

void MibParser::nameAndNumber2(void)
{
	ParserPow pow(this, "nameAndNumber2");
	Cardinal_1(pow, "definedValue");
	Call(pow);
//nameAndNumber2: definedValue
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// * Macro Syntax definitions
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void MibParser::definedMacroType(void)
{
	ParserPow pow(this, "definedMacroType");
	Branch(pow
			 , "macroModuleIdentity"
			 , "macroObjectIdentity"
			 , "macroObjectType"
			 , "macroNotificationType"
			 , "macroTextualConvention"
			 , "macroObjectGroup"
			 , "macroNotificationGroup"
			 , "macroModuleCompliance"
			 , "macroAgentCapabilities"
			 , "macroTrapType"
			 , "macroModuleConformance"
			 );
}

void MibParser::definedMacroName(void)
{
	ParserPow pow(this, "definedMacroName");
	Branch(pow,
			 Tok_MODULE_IDENTITY,
			 Tok_OBJECT_IDENTITY,
			 Tok_OBJECT_TYPE,
			 Tok_NOTIFICATION_TYPE,
			 Tok_TEXTUAL_CONVENTION,
			 Tok_OBJECT_GROUP,
			 Tok_NOTIFICATION_GROUP,
			 Tok_MODULE_COMPLIANCE,
			 Tok_AGENT_CAPABILITIES,
			 Tok_TRAP_TYPE,
			 Tok_MODULE_CONFORMANCE);
}

void MibParser::macroModuleIdentity(void)
{
	ParserPow pow(this, "macroModuleIdentity");
	Cardinal_1(pow, Tok_MODULE_IDENTITY);
	Call(pow);
	Cardinal_1(pow, "snmpLastUpdated", "snmpOrganization", "snmpContactInfo", "snmpDescription");
	Cardinal_0_n(pow, "snmpRevision");
}

void MibParser::macroObjectIdentity(void)
{
	ParserPow pow(this, "macroObjectIdentity");
	Cardinal_1(pow, Tok_OBJECT_IDENTITY);
	Call(pow);
	Cardinal_1(pow, "snmpStatus", "snmpDescription");
	Cardinal_0_1(pow, "snmpReference");
}

void MibParser::macroObjectType(void)
{
	ParserPow pow(this, "macroObjectType");
	Cardinal_1(pow, Tok_OBJECT_TYPE);
	Call(pow);
	Cardinal_1(pow, "snmpSyntax");
	Cardinal_0_1(pow, "snmpUnits");
	Cardinal_1(pow, "snmpAccess", "snmpStatus", "snmpDescription");
	Cardinal_0_1(pow, "snmpReference");
	Cardinal_1(pow, "snmpIndex");
	Cardinal_0_1(pow, "snmpDefVal");
}

void MibParser::macroNotificationType(void)
{
	ParserPow pow(this, "macroNotificationType");
	Cardinal_1(pow, Tok_NOTIFICATION_TYPE);
	Call(pow);
	Cardinal_1(pow, "snmpObjects", "snmpStatus", "snmpDescription");
	Cardinal_0_1(pow, "snmpReference");
}

void MibParser::macroTextualConvention(void)
{
	ParserPow pow(this, "macroTextualConvention");
	Cardinal_1(pow, Tok_TEXTUAL_CONVENTION);
	Call(pow);
	Cardinal_0_1(pow, "snmpDisplayHint");
	Cardinal_1(pow, "snmpStatus", "snmpDescription");
	Cardinal_0_1(pow, "snmpReference");
	Cardinal_1(pow, "snmpSyntax");
}

void MibParser::macroObjectGroup(void)
{
	ParserPow pow(this, "macroObjectGroup");
	Cardinal_1(pow, Tok_OBJECT_GROUP);
	Call(pow);
	Cardinal_1(pow, "snmpObjects", "snmpStatus", "snmpDescription");
	Cardinal_0_1(pow, "snmpReference");
}

void MibParser::macroNotificationGroup(void)
{
	ParserPow pow(this, "macroNotificationGroup");
	Cardinal_1(pow, Tok_NOTIFICATION_GROUP);
	Call(pow);
	Cardinal_1(pow, "snmpNotifications", "snmpStatus", "snmpDescription");
	Cardinal_0_1(pow, "snmpReference");
}

void MibParser::macroModuleCompliance(void)
{
	ParserPow pow(this, "macroModuleCompliance");
	Cardinal_1(pow, Tok_MODULE_COMPLIANCE);
	Call(pow);
	Cardinal_1(pow, "snmpStatus", "snmpDescription");
	Cardinal_0_1(pow, "snmpReference");
	Call(pow);
	Cardinal_1_n(pow, "snmpComplianceModule");
	Call(pow);
}

void MibParser::macroAgentCapabilities(void)
{
	ParserPow pow(this, "macroAgentCapabilities");
	Cardinal_1(pow, Tok_AGENT_CAPABILITIES);
	Call(pow);
	Cardinal_1(pow, "snmpProductRelease", "snmpStatus", "snmpDescription");
	Cardinal_0_1(pow, "snmpReference");
	Call(pow);
	Cardinal_0_n(pow, "snmpAgentModule");
	Call(pow);
}

void MibParser::macroTrapType(void)
{
	ParserPow pow(this, "macroTrapType");
	Cardinal_1(pow, Tok_TRAP_TYPE);
	Call(pow);
	Cardinal_1(pow, "snmpEnterprise");
	Cardinal_0_1(pow, "snmpVariables");
	Cardinal_0_1(pow, "snmpDescription");
	Cardinal_0_1(pow, "snmpReference");
}

void MibParser::macroModuleConformance(void)
{
	ParserPow pow(this, "macroModuleConformance");
	Cardinal_1(pow, Tok_MODULE_CONFORMANCE);
	Call(pow);
	Cardinal_1(pow, "snmpLastUpdated", "snmpProductRelease", "snmpDescription");
	Call(pow);
	Cardinal_0_n(pow, "snmpAgentModule");
	Call(pow);
}

void MibParser::snmpLastUpdated(void)
{
	ParserPow pow(this, "snmpLastUpdated");
	Cardinal_1(pow, Tok_LAST_UPDATED, Tok_String);
}

void MibParser::snmpOrganization(void)
{
	ParserPow pow(this, "snmpOrganization");
	Cardinal_1(pow, Tok_ORGANIZATION, Tok_String);
}

void MibParser::snmpContactInfo(void)
{
	ParserPow pow(this, "snmpContactInfo");
	Cardinal_1(pow, Tok_CONTACT_INFO, Tok_String);
}

void MibParser::snmpDescription(void)
{
	ParserPow pow(this, "snmpDescription");
	Cardinal_1(pow, Tok_DESCRIPTION, Tok_String);
	Call(pow);
}

void MibParser::snmpRevision(void)
{
	ParserPow pow(this, "snmpRevision");
	Cardinal_1(pow, Tok_REVISION, Tok_String, Tok_DESCRIPTION, Tok_String);
}

void MibParser::snmpStatus(void)
{
	ParserPow pow(this, "snmpStatus");
	Cardinal_1(pow, Tok_STATUS, "identifierName");
	Call(pow);
}

void MibParser::snmpAccess(void)
{
	ParserPow pow(this, "snmpAccess");
	Branch(pow, Tok_ACCESS, Tok_MAX_ACCESS, Tok_MIN_ACCESS);
	Cardinal_1(pow, "identifierName");
	Call(pow);
}

void MibParser::snmpReference(void)
{
	ParserPow pow(this, "snmpReference");
	Cardinal_1(pow, Tok_REFERENCE, Tok_String);
}

void MibParser::snmpSyntax(void)
{
	ParserPow pow(this, "snmpSyntax");
	Cardinal_1(pow, Tok_SYNTAX, "type");
}

void MibParser::snmpWriteSyntax(void)
{
	ParserPow pow(this, "snmpWriteSyntax");
	Cardinal_1(pow, Tok_WRITE_SYNTAX, "type");
}

void MibParser::snmpUnits(void)
{
	ParserPow pow(this, "snmpUnits");
	Cardinal_1(pow, Tok_UNITS, Tok_String);
}

void MibParser::snmpDisplayHint(void)
{
	ParserPow pow(this, "snmpDisplayHint");
	Cardinal_1(pow, Tok_DISPLAY_HINT, Tok_String);
}

void MibParser::snmpProductRelease(void)
{
	ParserPow pow(this, "snmpProductRelease");
	Cardinal_1(pow, Tok_PRODUCT_RELEASE, Tok_String);
}

void MibParser::snmpCommaValues(void)
{
	ParserPow pow(this, "snmpCommaValues");
	Cardinal_1(pow, "value");
	Cardinal_0_n(pow, Tok_COMMA, "value");
}

void MibParser::snmpIndex(void)
{
	ParserPow pow(this, "snmpIndex");
	Cardinal_0_1(pow, "snmpIndex1");
}

void MibParser::snmpIndex1(void)
{
	ParserPow pow(this, "snmpIndex1");
	Branch(pow, "snmpIndex2", "snmpIndex3");
}

void MibParser::snmpIndex2(void)
{
	ParserPow pow(this, "snmpIndex2");
	Cardinal_1(pow, Tok_INDEX, Tok_LEFT_BRACE, "snmpIndexTypes", Tok_RIGHT_BRACE);
}

void MibParser::snmpIndex3(void)
{
	ParserPow pow(this, "snmpIndex3");
	Cardinal_1(pow, Tok_AUGMENTS, Tok_LEFT_BRACE, "value", Tok_RIGHT_BRACE);
}

void MibParser::snmpIndexTypes(void)
{
	ParserPow pow(this, "snmpIndexTypes");
	Cardinal_1(pow, "snmpIndexType");
	Cardinal_0_n(pow, Tok_COMMA, "snmpIndexType");
}

void MibParser::snmpIndexType(void)
{
	ParserPow pow(this, "snmpIndexType");
	Cardinal_0_1(pow, Tok_IMPLIED);
	Cardinal_1(pow, "value");
}

void MibParser::snmpDefVal(void)
{
	ParserPow pow(this, "snmpDefVal");
	Cardinal_1(pow, Tok_DEFVAL, Tok_LEFT_BRACE, "value", Tok_RIGHT_BRACE);
}

void MibParser::snmpObjects(void)
{
	ParserPow pow(this, "snmpObjects");
	Cardinal_0_1(pow, Tok_OBJECTS, Tok_LEFT_BRACE, "snmpCommaValues", Tok_RIGHT_BRACE);
}

void MibParser::snmpNotifications(void)
{
	ParserPow pow(this, "snmpNotifications");
	Cardinal_1(pow, Tok_NOTIFICATIONS, Tok_LEFT_BRACE, "snmpCommaValues", Tok_RIGHT_BRACE);
}

void MibParser::snmpComplianceModule(void)
{
	ParserPow pow(this, "snmpComplianceModule");
	Cardinal_1(pow, Tok_MODULE, "snmpModuleName", "snmpComplianceMandatory");
	Cardinal_0_n(pow, "snmpCompliance");
}

void MibParser::snmpModuleName(void)
{
	ParserPow pow(this, "snmpModuleName");
	Cardinal_0_1(pow, Tok_Uppercase, "snmpModuleIdentifier");
}

void MibParser::snmpModuleIdentifier(void)
{
	ParserPow pow(this, "snmpModuleIdentifier");
	Cardinal_0_1(pow, "value");
}

void MibParser::snmpComplianceMandatory(void)
{
	ParserPow pow(this, "snmpComplianceMandatory");
	Cardinal_0_1(pow, Tok_MANDATORY_GROUPS, Tok_LEFT_BRACE, "snmpCommaValues", Tok_RIGHT_BRACE);
}

void MibParser::snmpCompliance(void)
{
	ParserPow pow(this, "snmpCompliance");
	Branch(pow, "snmpComplianceGroup", "snmpComplianceObject");
}

void MibParser::snmpComplianceGroup(void)
{
	ParserPow pow(this, "snmpComplianceGroup");
	Cardinal_1(pow, Tok_GROUP, "value", "snmpDescription");
}

void MibParser::snmpComplianceObject(void)
{
	ParserPow pow(this, "snmpComplianceObject");
	Cardinal_1(pow, Tok_OBJECT, "value");
	Cardinal_0_1(pow, "snmpSyntax");
	Cardinal_0_1(pow, "snmpWriteSyntax");
	Cardinal_0_1(pow, "snmpAccess");
	Cardinal_1(pow, "snmpDescription");
}

void MibParser::snmpAgentModule(void)
{
	ParserPow pow(this, "snmpAgentModule");
	Cardinal_1(pow, "snmpAgentSupport", "snmpAgentInclude", "snmpAgentVariation");
}

void MibParser::snmpAgentSupport(void)
{
	ParserPow pow(this, "snmpAgentSupport");
	Cardinal_1(pow, Tok_SUPPORTS, "snmpModuleName");
}

void MibParser::snmpAgentInclude(void)
{
	ParserPow pow(this, "snmpAgentInclude");
	Cardinal_1(pow, Tok_INCLUDES, Tok_LEFT_BRACE, "snmpCommaValues", Tok_RIGHT_BRACE);
}

void MibParser::snmpAgentVariation(void)
{
	ParserPow pow(this, "snmpAgentVariation");
	Cardinal_0_n(pow, "snmpAgentObjectVariation");
}

void MibParser::snmpAgentObjectVariation(void)
{
	ParserPow pow(this, "snmpAgentObjectVariation");
	Cardinal_1(pow, Tok_VARIATION, "value");
	Cardinal_0_1(pow, "snmpSyntax");
	Cardinal_0_1(pow, "snmpWriteSyntax");
	Cardinal_0_1(pow, "snmpAccess");
	Cardinal_0_1(pow, "snmpCreationRequire");
	Cardinal_0_1(pow, "snmpDefVal");
	Cardinal_1(pow, "snmpDescription");
}

void MibParser::snmpCreationRequire(void)
{
	ParserPow pow(this, "snmpCreationRequire");
	Cardinal_1(pow, Tok_CREATION_REQUIRES, Tok_LEFT_BRACE, "snmpCommaValues", Tok_RIGHT_BRACE);
}

void MibParser::snmpEnterprise(void)
{
	ParserPow pow(this, "snmpEnterprise");
	Cardinal_1(pow, Tok_ENTERPRISE, "value");
}

void MibParser::snmpVariables(void)
{
	ParserPow pow(this, "snmpVariables");
	Cardinal_1(pow, Tok_VARIABLES, Tok_LEFT_BRACE, "snmpCommaValues", Tok_RIGHT_BRACE);
}


