#ifndef SNMPTYPE_H
#define SNMPTYPE_H

#include <QString>
#include <QVector>
#include "StringMatch.h"

class SnmpType
{
public:
		typedef enum { TypeNotSet, TypeDefined,
							TypeNull, TypeBoolean, TypeReal,
							TypeInteger, TypeObjectIdentifier, TypeOctetString,
							TypeBitString, TypeBits, TypeSequence,
							TypeSequenceOf, TypeSet, TypeSetOf,
							TypeChoice, TypeEnumerated, TypeSelection,
							TypeAny,
							TypeMacroModuleIdentity, TypeMacroObjectIdentity, TypeMacroObjectType, TypeMacroNotificationType,
							TypeMacroTextualConvention,
							TypeMacroObjectGroup, TypeMacroNotificationGroup, TypeMacroModuleCompliance, TypeMacroAgentCapabilities,
							TypeMacroTrapType, TypeMacroModuleConformance,
						 } BaseType_t;

	SnmpType(void);

	void clear(void);

	inline const QString & Name(void) const { return m_name; }
	void Name(QString & val);

	inline const QString & MibModule(void) const { return m_mib_module; }
	void MibModule(const QString & val);

	inline int MibLine(void) const { return m_mib_line; }
	void MibLine(int val);

	inline const QString & Description(void) const { return m_description; }
	void Description(const QString & str, bool override = false);

	inline bool isSet(void) const { return m_type != TypeNotSet; }
	void setType(SnmpType & type);

	inline bool isUserChoice(void) const { return m_user_choice; }
	void UserChoice(bool user_choice);

	void Dump(int level) const;

	inline const char * TypeStr(void) const { return StringMatch::SnmpTypeTranslate(m_type); }

	inline const QString & getTypeExtended(void) const { return m_type_extended; }
	void addTypeExtended(const QString & str);

	inline bool isTypeDefined(void) const { return m_type == TypeDefined; }
	inline const QString & getTypeDefined(void) const { return m_type_name; }

	inline const QVector<SnmpType> & getCompound(void) const { return m_compound; }
	SnmpType & addCompound(void);
	SnmpType & lastCompound(void);

	bool isLocked(void) const { return m_lock; }
	void Lock(void) { m_lock = true; }
	void Unlock(void) { m_lock = false; }

	inline void setTypeDefined(const QString & name)	{ set(TypeDefined, name); }
	inline void setTypeNull(void)								{ set(TypeNull); }
	inline void setTypeBoolean(void)							{ set(TypeBoolean); }
	inline void setTypeReal(void)								{ set(TypeReal); }
	inline void setTypeInteger(void)							{ set(TypeInteger); }
	inline void setTypeObjectIdentifier(void)				{ set(TypeObjectIdentifier); }
	inline void setTypeOctetString(void)					{ set(TypeOctetString); }
	inline void setTypeBitString(void)						{ set(TypeBitString); }
	inline void setTypeBits(void)								{ set(TypeBits); }
	inline void setTypeSequence(void)						{ set(TypeSequence); }
	inline void setTypeSequenceOf(void)						{ set(TypeSequenceOf); }
	inline void setTypeSet(void)								{ set(TypeSet); }
	inline void setTypeSetOf(void)							{ set(TypeSetOf); }
	inline void setTypeChoice(void)							{ set(TypeChoice); }
	inline void setTypeEnumerated(void)						{ set(TypeEnumerated); }
	inline void setTypeSelection(void)						{ set(TypeSelection); }
	inline void setTypeAny(void)								{ set(TypeAny); }

	inline void setTypeMacroModuleIdentity(void)			{ set(TypeMacroModuleIdentity); }
	inline void setTypeMacroObjectIdentity(void)			{ set(TypeMacroObjectIdentity); }
	inline void setTypeMacroObjectType(void)				{ set(TypeMacroObjectType); }
	inline void setTypeMacroNotificationType(void)		{ set(TypeMacroNotificationType); }
	inline void setTypeMacroTextualConvention(void)		{ set(TypeMacroTextualConvention); }
	inline void setTypeMacroObjectGroup(void)				{ set(TypeMacroObjectGroup); }
	inline void setTypeMacroNotificationGroup(void)		{ set(TypeMacroNotificationGroup); }
	inline void setTypeMacroModuleCompliance(void)		{ set(TypeMacroModuleCompliance); }
	inline void setTypeMacroAgentCapabilities(void)		{ set(TypeMacroAgentCapabilities); }
	inline void setTypeMacroTrapType(void)					{ set(TypeMacroTrapType); }
	inline void setTypeMacroModuleConformance(void)		{ set(TypeMacroModuleConformance); }

	inline bool isMacroTrapType(void) const { return m_type == TypeMacroTrapType; }

private:

	void set(BaseType_t type, const QString & name = "");

	QString m_mib_module;
	int m_mib_line;
	bool m_user_choice;

	QString m_name;						// name of the type
	BaseType_t m_type;
	QString m_type_name;					// used when type is 'Defined'
	QString m_type_extended;			// extension to the type (constraints, size, ...
	QVector<SnmpType> m_compound;
	QString m_description;

	static bool is_macro(BaseType_t type);

	bool m_lock;

};

#endif // SNMPTYPE_H
