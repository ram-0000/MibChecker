#ifndef SNMPTREE_H
#define SNMPTREE_H

#include "ParserAction.h"
#include "SnmpImportManager.h"
#include "ParserException.h"
#include <QVector>
#include <QString>
#include "SnmpValue.h"
#include "SnmpType.h"
#include "MibFolder.h"

class SnmpTree : public ParserAction
{
public:
	SnmpTree(void);
	virtual ~SnmpTree(void);

	void clear(void);

	void Load(MibFolder & folder, const QString & mib_filename, bool user_choice = true);
	void Dump(void) const;

	QVector<SnmpType> m_global_types;
	QVector<SnmpValue> m_global_values;
	SnmpImportManager m_import_manager;

private:
	void _check_values(const QVector<SnmpValue> & values, const QVector<SnmpType> & types, bool check_macro);
	void _check_types(const QVector<SnmpType> & types, bool check_macro);
	void _check_type(const QVector<SnmpType> & types,
						  const SnmpType & type,
						  bool check_macro) const;
	bool _check_type_name(const QVector<SnmpType> & types, const QString & name) const;
	bool _check_value_name(const QVector<SnmpValue> & values, const QString & name) const;
	bool _resolve_oid(QVector<SnmpValue> & values);
	const SnmpOid & _get_oid_by_name(const QVector<SnmpValue> & values, const QString & name) const;

	virtual void OnAction(int MibLine,
								 const char * rule,
								 int order,
								 ParserValue & Value);

	typedef void (SnmpTree:: * PtrFct_t)(int order, ParserValue & Value);
	typedef struct { const char * name; PtrFct_t fct; } Jump_t;
	static Jump_t m_tbl_jump[];
	bool m_is_user_choice;
	QString m_mib_root_module;

	bool m_import;
	void On_importList(int order, ParserValue & Value);

	void On_symbol(int order, ParserValue & Value);

	inline void On_symbolsFromModule(int, ParserValue & Value) { m_import_manager.AddImportedModule(Value.get()); }

	QString m_tmp_name;

	int m_type_level;
	SnmpType m_tmp_type;
	SnmpType * m_tmp_type_ptr;
	QVector<SnmpType> m_module_types;

	SnmpValue m_tmp_value;
	SnmpValue m_tmp_integer_value;
	QVector<SnmpValue> m_module_values;

	QString m_mib_file;
	QString m_mib_filename;
	int m_mib_line;

	inline void On_moduleName(int, ParserValue & Value) { m_tmp_name = Value.get(); }
	inline void On_typeName(int, ParserValue & Value) { m_tmp_name = Value.get(); }
	inline void On_valueName(int, ParserValue & Value) { m_tmp_name = Value.get(); }
	inline void On_identifierName(int, ParserValue & Value) { m_tmp_name = Value.get(); }
	inline void On_anyName(int, ParserValue & Value) { m_tmp_name = Value.get(); }

	void On_mibModule(int, ParserValue & Value);
	inline void On_definedType(int, ParserValue & Value) { m_tmp_type_ptr->setTypeDefined(Value.get()); }
	inline void On_nullType(int, ParserValue &) { m_tmp_type_ptr->setTypeNull(); }
	inline void On_booleanType(int, ParserValue &) { m_tmp_type_ptr->setTypeBoolean(); }
	inline void On_realType(int, ParserValue &) { m_tmp_type_ptr->setTypeReal(); }
	inline void On_integerType(int, ParserValue &) { m_tmp_type_ptr->setTypeInteger(); }
	inline void On_objectIdentifierType(int, ParserValue &) { m_tmp_type_ptr->setTypeObjectIdentifier(); }
	inline void On_stringType(int, ParserValue &) { m_tmp_type_ptr->setTypeOctetString(); }
	inline void On_bitStringType(int, ParserValue &) { m_tmp_type_ptr->setTypeBitString(); }
	inline void On_bitsType(int, ParserValue &) { m_tmp_type_ptr->setTypeBits(); }
	inline void On_sequenceType(int, ParserValue &) { m_tmp_type_ptr->setTypeSequence(); }
	inline void On_sequenceOfType(int, ParserValue &) { m_tmp_type_ptr->setTypeSequenceOf(); }
	inline void On_setType(int, ParserValue &) { m_tmp_type_ptr->setTypeSet(); }
	inline void On_setOfType(int, ParserValue &) { m_tmp_type_ptr->setTypeSetOf(); }
	inline void On_choiceType(int, ParserValue &) { m_tmp_type_ptr->setTypeChoice(); }
	inline void On_enumeratedType(int, ParserValue &) { m_tmp_type_ptr->setTypeEnumerated(); }
	inline void On_selectionType(int, ParserValue &) { m_tmp_type_ptr->setTypeSelection(); }
	inline void On_anyType(int, ParserValue &) { m_tmp_type_ptr->setTypeAny(); }

	inline void On_macroModuleIdentity(int, ParserValue &) { m_tmp_type_ptr->setTypeMacroModuleIdentity(); }
	inline void On_macroObjectIdentity(int, ParserValue &) { m_tmp_type_ptr->setTypeMacroObjectIdentity(); }
	inline void On_macroObjectType(int, ParserValue &) { m_tmp_type_ptr->setTypeMacroObjectType(); }
	inline void On_macroNotificationType(int, ParserValue &) { m_tmp_type_ptr->setTypeMacroNotificationType(); }
	inline void On_macroTextualConvention(int, ParserValue &) { m_tmp_type_ptr->setTypeMacroTextualConvention(); }
	inline void On_macroObjectGroup(int, ParserValue &) { m_tmp_type_ptr->setTypeMacroObjectGroup(); }
	inline void On_macroNotificationGroup(int, ParserValue &) { m_tmp_type_ptr->setTypeMacroNotificationGroup(); }
	void On_macroModuleCompliance(int order, ParserValue & Value);
	void On_macroAgentCapabilities(int order, ParserValue & Value);
	inline void On_macroTrapType(int, ParserValue &) { m_tmp_type_ptr->setTypeMacroTrapType(); }
	void On_macroModuleConformance(int order, ParserValue & Value);

	inline void On_nameOrNumber1(int, ParserValue & Value) { m_tmp_value.m_oid.add(Value.get().toInt()); }
	inline void On_nameOrNumber2(int, ParserValue & Value) { m_tmp_value.m_oid.add(Value.get()); }
	inline void On_nameAndNumber1(int, ParserValue & Value) { m_tmp_value.m_oid.set(Value.get().toInt()); }
	inline void On_nameAndNumber2(int, ParserValue & Value) { m_tmp_value.m_oid.set(Value.get()); }

	inline void On_snmpDescription(int, ParserValue & Value) { m_tmp_type_ptr->Description(Value.get()); }
	inline void On_snmpStatus(int, ParserValue & /*Value*/) { m_tmp_value.m_status.Set(m_tmp_name); }
	inline void On_snmpAccess(int, ParserValue & /*Value*/) { m_tmp_value.m_access.Set(m_tmp_name); }

	void On_type1(int order, ParserValue & Value);
	void On_namedNumber(int order, ParserValue & Value);
	void On_numberValue(int order, ParserValue & Value);
	void On_constraint(int order, ParserValue & Value);
	inline void On_lowerEndPoint(int, ParserValue & Value) { m_tmp_type_ptr->addTypeExtended(Value.get()); }
	inline void On_upperEndPoint(int, ParserValue & Value) { m_tmp_type_ptr->addTypeExtended(".."); m_tmp_type_ptr->addTypeExtended(Value.get()); }
	void On_sizeConstraint(int order, ParserValue & Value);
	void On_valueConstraintListOpt(int, ParserValue &) { m_tmp_type_ptr->addTypeExtended("|"); }

	void On_valueAssignment(int order, ParserValue & Value);
	void On_typeAssignment(int order, ParserValue & Value);

};

#endif // SNMPTREE_H
