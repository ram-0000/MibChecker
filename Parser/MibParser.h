#ifndef MIBPARSER_H
#define MIBPARSER_H

#include <QString>
#include "ParserGeneric.h"
#include "ParserException.h"

class MibParser : public ParserGeneric
{
public:
	MibParser(ParserAction & Callback, const QString & name);
	virtual ~MibParser(void);

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
	void valueConstraintListOpt(void);
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

	void macroTextualConvention(void);

	void macroObjectGroup(void);
	void macroNotificationGroup(void);
	void macroModuleCompliance(void);
	void macroAgentCapabilities(void);

	void macroTrapType(void);
	void macroModuleConformance(void);

	void snmpLastUpdated(void);
	void snmpOrganization(void);
	void snmpContactInfo(void);
	void snmpDescription(void);
	void snmpRevision(void);
	void snmpStatus(void);
	void snmpReference(void);
	void snmpAccess(void);
	void snmpSyntax(void);
	void snmpWriteSyntax(void);
	void snmpUnits(void);
	void snmpDisplayHint(void);
	void snmpProductRelease(void);
	void snmpCommaValues(void);

	void snmpIndex(void);
	void snmpIndex1(void);
	void snmpIndex2(void);
	void snmpIndex3(void);
	void snmpIndexTypes(void);
	void snmpIndexType(void);
	void snmpDefVal(void);
	void snmpObjects(void);
	void snmpNotifications(void);
	void snmpComplianceModule(void);
	void snmpModuleName(void);
	void snmpModuleIdentifier(void);
	void snmpComplianceMandatory(void);
	void snmpCompliance(void);
	void snmpComplianceGroup(void);
	void snmpComplianceObject(void);
	void snmpAgentModule(void);
	void snmpAgentSupport(void);
	void snmpAgentInclude(void);
	void snmpAgentVariation(void);
	void snmpAgentObjectVariation(void);
	void snmpCreationRequire(void);
	void snmpEnterprise(void);
	void snmpVariables(void);

};

#endif // MIBPARSER_H
