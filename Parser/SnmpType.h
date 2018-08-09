#ifndef SNMPTYPE_H
#define SNMPTYPE_H

#include <QString>
#include <QVector>

class SnmpType
{
public:
	SnmpType(void);
	~SnmpType(void);

	void clear(void);

	inline void Name(QString & val) { m_name = val; val.clear(); }
	inline const QString & Name(void) const { return m_name; }

	const QString & MibModule(void) const { return m_mib_module; }
	void MibModule(const QString & val) { m_mib_module = val; }

	int MibLine(void) const { return m_mib_line; }
	void MibLine(int val) { m_mib_line = val; }

	bool isSet(void) const { return m_type != TypeNotSet; }
	void setType(SnmpType & type);

	void Dump(int level) const;

	inline void UserChoice(bool user_choice) { m_user_choice = user_choice; }
	inline bool isUserChoice(void) const { return m_user_choice; }

	inline QString TypeStr(void) const { return type_str(m_type); }
	inline const QString & ConstraintStr(void) const { return m_type_extended; }

	typedef enum { TypeNotSet, TypeDefined,
						TypeNull, TypeBoolean, TypeReal,
						TypeInteger, TypeObjectIdentifier, TypeString,
						TypeBitString, TypeBits, TypeSequence,
						TypeSequenceOf, TypeSet, TypeSetOf,
						TypeChoice, TypeEnumerated, TypeSelection,
						TypeAny,
						TypeMacroModuleIdentity, TypeMacroObjectIdentity, TypeMacroObjectType,
						TypeMacroNotificationType, TypeMacroTrapType, TypeMacroTextualConvention,
						TypeMacroObjectGroup, TypeMacroNotificationGroup, TypeMacroModuleCompliance,
						TypeMacroAgentCapabilities
					 } BaseType_t;

	inline void setTypeDefined(const QString & name) { m_type = TypeDefined;  m_type_name = name; }
	inline void setTypeNull(void) { m_type = TypeNull; }
	inline void setTypeBoolean(void) { m_type = TypeBoolean; }
	inline void setTypeReal(void) { m_type = TypeReal; }
	inline void setTypeInteger(void) { m_type = TypeInteger; }
	inline void setTypeObjectIdentifier(void) { m_type = TypeObjectIdentifier; }
	inline void setTypeString(void) { m_type = TypeString; }
	inline void setTypeBitString(void) { m_type = TypeBitString; }
	inline void setTypeBits(void) { m_type = TypeBits; }
	inline void setTypeSequence(void) { m_type = TypeSequence; }
	inline void setTypeSequenceOf(void) { m_type = TypeSequenceOf; }
	inline void setTypeSet(void) { m_type = TypeSet; }
	inline void setTypeSetOf(void) { m_type = TypeSetOf; }
	inline void setTypeChoice(void) { m_type = TypeChoice; }
	inline void setTypeEnumerated(void) { m_type = TypeEnumerated; }
	inline void setTypeSelection(void) { m_type = TypeSelection; }
	inline void setTypeAny(void) { m_type = TypeAny; }

	inline void setTypeMacroModuleIdentity(void) { m_type = TypeMacroModuleIdentity; }
	inline void setTypeMacroObjectIdentity(void) { m_type = TypeMacroObjectIdentity; }
	inline void setTypeMacroObjectType(void) { setTypeDefined(NameMacroType()); }
	inline void setTypeMacroNotificationType(void) { m_type = TypeMacroNotificationType; }
	inline void setTypeMacroTrapType(void) { m_type =TypeMacroTrapType ; }
	inline void setTypeMacroTextualConvention(void) { m_type = TypeMacroTextualConvention; }
	inline void setTypeMacroObjectGroup(void) { m_type = TypeMacroObjectGroup; }
	inline void setTypeMacroNotificationGroup(void) { m_type = TypeMacroNotificationGroup; }
	inline void setTypeMacroModuleCompliance(void) { m_type = TypeMacroModuleCompliance; }
	inline void setTypeMacroAgentCapabilities(void) { m_type = TypeMacroAgentCapabilities; }

	inline void add_type_extended(const QString & str) { m_type_extended += str; }
	inline bool is_type_extended_empty(void) const { return m_type_extended.length() == 0; }

	inline bool isTypeObjectIdentifier(void) const { return m_type == TypeObjectIdentifier; }
	inline bool isTypeDefined(void) const { return m_type == TypeDefined; }
	inline const QString & getTypeDefined(void) const { return m_type_name; }

	SnmpType & addCompound(void);
	SnmpType & lastCompound(void);
	void addCompound(SnmpType & type);
	inline const QVector<SnmpType> & getCompound(void) const { return m_compound; }

	inline bool isMacro(void) const { return is_macro(m_type); }

	static inline const char * NameMacroType(void) { return "OBJECT-TYPE"; }

private:
	QString m_mib_module;
	int m_mib_line;
	bool m_user_choice;

	QString m_name;						// name of the type
	BaseType_t m_type;
	QString m_type_name;					// used when type is 'Defined'
	QString m_type_extended;			// extension to the type (constraints, size, ...
	QVector<SnmpType> m_compound;

	static const char * type_str(BaseType_t type);
	static bool is_macro(BaseType_t type);

};

#endif // SNMPTYPE_H
