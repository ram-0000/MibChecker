#ifndef MIBPARSER_H
#define MIBPARSER_H

#include <QString>
#include "ParserGeneric.h"
#include "ParserException.h"

class MibParser : public ParserGeneric
{
public:
	MibParser(ParserAction & Callback, const QString & name) throw (ParserException);
	virtual ~MibParser(void);

	void Execute(void);

	virtual bool NextToken(LexToken_t & token);

private:

	MibLexer m_lexer;

	void _generate(void);

	void mibFile(void);
	void moduleName(void);
	void typeName(void);
	void valueName(void);
	void identifierName(void);
	void anyName(void);
	void mibModule(void);
	void tagDefault(void);
	void moduleBody(void);
	void exportList(void);
	void importList(void);
	void symbolsFromModule(void);
	void symbolList(void);
	void symbol(void);
	void assignmentList(void);
	void assignment(void);
	void macroDefinition(void);
	void macroReference(void);
	void macroBody(void);
	void typeAssignment(void);
	void type(void);
	void type1(void);
	void definedType(void);
	void builtinType(void);
	void nullType(void);
	void booleanType(void);
	void realType(void);
	void integerType(void);
	void objectIdentifierType(void);
	void stringType(void);
	void bitStringType(void);
	void bitsType(void);
	void sequence(void);
	void sequenceType(void);
	void sequenceOfType(void);
	void set(void);
	void setType(void);
	void setOfType(void);
	void choiceType(void);
	void enumeratedType(void);
	void selectionType(void);
	void tag(void);
	void classtag(void);
	void explicitOrImplicitTag(void);
	void anyType(void);
	void elementTypeList(void);
	void elementType(void);
	void elementType1(void);
	void elementType2(void);
	void optionalOrDefaultElement(void);
	void optionalOrDefaultElement1(void);
	void optionalOrDefaultElement2(void);
	void valueOrConstraintList(void);
	void namedNumberList(void);
	void namedNumber(void);
	void number(void);
	void constraintList(void);
	void constraint(void);
	void valueConstraintList(void);
	void valueConstraint(void);
	void valueRange(void);
	void lowerEndPoint(void);
	void upperEndPoint(void);
	void sizeConstraint(void);
	void alphabetConstraint(void);
	void containedTypeConstraint(void);
	void innerTypeConstraint(void);
	void innerTypeConstraint1(void);
	void innerTypeConstraint2(void);
	void componentsList(void);
	void componentsList1(void);
	void componentsList2(void);
	void componentsListTail(void);
	void componentConstraint(void);
	void componentConstraint1(void);
	void componentConstraint2(void);
	void componentValuePresence(void);
	void componentValuePresence1(void);
	void componentValuePresence2(void);
	void componentPresence(void);
	void valueAssignment(void);
	void value(void);
	void definedValue(void);
	void builtinValue(void);
	void nullValue(void);
	void booleanValue(void);
	void specialRealValue(void);
	void numberValue(void);
	void binaryValue(void);
	void hexadecimalValue(void);
	void stringValue(void);
	void bitOrObjectIdentifierValue(void);
	void nameOrNumber(void);
	void nameOrNumber1(void);
	void nameOrNumber2(void);
	void nameValueComponent(void);
	void nameAndNumber(void);
	void nameAndNumber1(void);
	void nameAndNumber2(void);
	void definedMacroType(void);
	void definedMacroName(void);
	void macroModuleIdentity(void);
	void macroObjectIdentity(void);
	void macroObjectType(void);
	void macroNotificationType(void);
	void macroTrapType(void);
	void macroTextualConvention(void);
	void macroObjectGroup(void);
	void macroNotificationGroup(void);
	void macroModuleCompliance(void);
	void macroAgentCapabilities(void);
	void snmpUpdatePart(void);
	void snmpOrganizationPart(void);
	void snmpContactPart(void);
	void snmpDescrPart(void);
	void snmpRevisionPart(void);
	void snmpStatusPart(void);
	void snmpReferPart(void);
	void snmpSyntaxPart(void);
	void snmpUnitsPart(void);
	void snmpAccessPart(void);
	void snmpIndexPart(void);
	void snmpIndexPart1(void);
	void snmpIndexPart2(void);
	void indexValueList(void);
	void indexValue(void);
	void indexValue1(void);
	void indexValue2(void);
	void indexType(void);
	void snmpDefValPart(void);
	void snmpObjectsPart(void);
	void valueList(void);
	void snmpEnterprisePart(void);
	void snmpVarPart(void);
	void snmpDisplayPart(void);
	void snmpNotificationsPart(void);
	void snmpModulePart(void);
	void snmpModuleImport(void);
	void snmpMandatoryPart(void);
	void snmpCompliancePart(void);
	void complianceGroup(void);
	void complianceObject(void);
	void snmpWriteSyntaxPart(void);
	void snmpProductReleasePart(void);
	void snmpModuleSupportPart(void);
	void snmpVariationPart(void);
	void snmpCreationPart(void);

};

#endif // MIBPARSER_H
