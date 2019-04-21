#include "SnmpTree.h"
#include "MibParser.h"
#include "MibFolder.h"
#include "Debug.h"
#include "Conf.h"

SnmpTree::SnmpTree(void)
{
	clear();
}

SnmpTree::~SnmpTree(void)
{
	clear();
}

void SnmpTree::clear(void)
{
	m_import_manager.clear();

	m_global_values.clear();
	m_module_values.clear();
	m_tmp_value.clear();
	m_import = false;
	m_mib_root_module.clear();

	m_global_types.clear();
	m_module_types.clear();
	m_tmp_type.Unlock();
	m_tmp_type.clear();
	m_tmp_type_ptr = nullptr;
	m_tmp_name.clear();
	m_type_level = 0;
}

void SnmpTree::Dump(void) const
{
	QString str;

	DEBUG("Imported symbols");
	str = "";
	for(const auto & item : m_import_manager.GetImportedSymbol(m_mib_root_module))
	{
		if(str.length() == 0) str += "   ";
		else str += ", ";
		str += item;
	}
	DEBUG("%s", str.toLatin1().constData());

	DEBUG("Modules loaded");
	str = "";
	for(const auto & item : m_import_manager.GetImportedModules(m_mib_root_module))
	{
		if(str.length() == 0) str += "   ";
		else str += ", ";
		str += item;
	}
	DEBUG("%s", str.toLatin1().constData());

	DEBUG("Defined types");
	for(const auto & item : m_global_types)
	{
		if(item.isUserChoice() == true)
			item.Dump(1);
	}

	DEBUG("Defined values");
	for(const auto & item : m_global_values)
	{
		if(item.isUserChoice() == true)
			item.Dump(1);
	}
}

void SnmpTree::Load(MibFolder & folder, const QString & mib_filename, bool user_choice)
{
	if(mib_filename.contains(".") == true)
		throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);

	m_mib_filename = mib_filename;

	DEBUG("Loading MIB %s...", m_mib_filename.toLatin1().constData());
	m_import_manager.SetCurrentModule(m_mib_filename);
	m_is_user_choice = user_choice;
	if(m_is_user_choice == true)
		m_mib_root_module = mib_filename;

	// create filename
	m_mib_file = folder.BuildMibFile(m_mib_filename);

	// create parser
	MibParser parser(*this, m_mib_file);

	// load primary module
	m_module_types.clear();
	m_module_values.clear();
	m_import = false;
	parser.Execute();

	// verify the module
	_check_values(m_module_values, m_module_types, true);
	_check_types(m_module_types, true);

	m_global_values += m_module_values;
	m_global_types += m_module_types;

	m_module_types.clear();
	m_module_values.clear();

	// load other mib files
	QString module = m_import_manager.GetModuleToLoad();
	if(module.length() != 0)
		Load(folder, module, false);

	if(user_choice == true)
	{
		DEBUG("Final pass...");

		// verify the complete tree
		_check_types(m_global_types, false);
		_check_values(m_global_values, m_global_types, false);

		// resolve all oid
		while(_resolve_oid(m_global_values) == true)
			;
	}
}

SnmpTree::Jump_t SnmpTree::m_tbl_jump[] =
{
	{ "moduleName", &SnmpTree::On_moduleName },
	{ "typeName", &SnmpTree::On_typeName },
	{ "valueName", &SnmpTree::On_valueName },
	{ "identifierName", &SnmpTree::On_valueName },
	{ "anyName", &SnmpTree::On_anyName },

	{ "mibModule", &SnmpTree::On_mibModule },
	{ "importList", &SnmpTree::On_importList },
	{ "symbol", &SnmpTree::On_symbol },
	{ "symbolsFromModule", &SnmpTree::On_symbolsFromModule },
	{ "valueAssignment", &SnmpTree::On_valueAssignment },
	{ "definedType", &SnmpTree::On_definedType },
	{ "nullType", &SnmpTree::On_nullType },
	{ "booleanType", &SnmpTree::On_booleanType },
	{ "realType", &SnmpTree::On_realType },
	{ "integerType", &SnmpTree::On_integerType },
	{ "objectIdentifierType", &SnmpTree::On_objectIdentifierType },
	{ "stringType", &SnmpTree::On_stringType },
	{ "bitStringType", &SnmpTree::On_bitStringType },
	{ "bitsType", &SnmpTree::On_bitsType },
	{ "sequenceType", &SnmpTree::On_sequenceType },
	{ "sequenceOfType", &SnmpTree::On_sequenceOfType },
	{ "setType", &SnmpTree::On_setType },
	{ "setOfType", &SnmpTree::On_setOfType },
	{ "choiceType", &SnmpTree::On_choiceType },
	{ "enumeratedType", &SnmpTree::On_enumeratedType },
	{ "selectionType", &SnmpTree::On_selectionType },
	{ "anyType", &SnmpTree::On_anyType },
	{ "nameOrNumber1", &SnmpTree::On_nameOrNumber1 },
	{ "nameOrNumber2", &SnmpTree::On_nameOrNumber2 },
	{ "nameAndNumber1", &SnmpTree::On_nameAndNumber1 },
	{ "nameAndNumber2", &SnmpTree::On_nameAndNumber2 },

	{ "macroModuleIdentity", &SnmpTree::On_macroModuleIdentity },
	{ "macroObjectIdentity", &SnmpTree::On_macroObjectIdentity },
	{ "macroObjectType", &SnmpTree::On_macroObjectType },
	{ "macroNotificationType", &SnmpTree::On_macroNotificationType },
	{ "macroTrapType", &SnmpTree::On_macroTrapType },
	{ "macroTextualConvention", &SnmpTree::On_macroTextualConvention },
	{ "macroObjectGroup", &SnmpTree::On_macroObjectGroup },
	{ "macroNotificationGroup", &SnmpTree::On_macroNotificationGroup },
	{ "macroModuleCompliance", &SnmpTree::On_macroModuleCompliance },
	{ "macroModuleConformance", &SnmpTree::On_macroModuleConformance },
	{ "macroAgentCapabilities", &SnmpTree::On_macroAgentCapabilities },

	{ "snmpDescription", &SnmpTree::On_snmpDescription },
	{ "snmpStatus", &SnmpTree::On_snmpStatus },
	{ "snmpAccess", &SnmpTree::On_snmpAccess },

	{ "namedNumber", &SnmpTree::On_namedNumber },
	{ "numberValue", &SnmpTree::On_numberValue },
	{ "lowerEndPoint", &SnmpTree::On_lowerEndPoint },
	{ "upperEndPoint", &SnmpTree::On_upperEndPoint },
	{ "typeAssignment", &SnmpTree::On_typeAssignment },
	{ "type1", &SnmpTree::On_type1 },
	{ "sizeConstraint", &SnmpTree::On_sizeConstraint },
	{ "constraint", &SnmpTree::On_constraint },

};

void SnmpTree::OnAction(int MibLine,
								const char * rule,
								int order,
								ParserValue & Value)
{
	// store mibfile and mibline
	m_mib_line = MibLine;

	//DEBUG("DEBUG Parsing: OnAction %s(%d) ==> '%s'", rule, order, Value.get().toLatin1().constData());
	for(int boucle = 0; boucle != sizeof(m_tbl_jump) / sizeof(m_tbl_jump[0]); boucle++)
	{
		if(strcmp(rule, m_tbl_jump[boucle].name) != 0)
			continue;

		// execute function
		(this->*m_tbl_jump[boucle].fct)(order, Value);
		return;
	}

	// not found
	throw ParserExceptionCallbackNotFound(rule);
}

void SnmpTree::On_mibModule(int, ParserValue & Value)
{
	QString mib_name = m_mib_filename;
	if(mib_name.endsWith(Conf::MibExtension()) == true)
		mib_name = mib_name.left(mib_name.length() - Conf::MibExtension().length());

	if(mib_name != Value.get())
		throw ParserExceptionMibInvalidName(m_mib_filename, Value.get());
}

void SnmpTree::On_importList(int order, ParserValue &)
{
	if(order == 1)
		m_import = true;
	else if(order == 2)
		m_import = false;
}

void SnmpTree::On_symbol(int, ParserValue & Value)
{
	if(m_import == true)
		m_import_manager.AddImportedSymbol(Value.get());
}

void SnmpTree::On_constraint(int, ParserValue &)
{
	if(m_tmp_type_ptr->getTypeExtended().isEmpty() == false)
		m_tmp_type_ptr->addTypeExtended(" | ");
}

void SnmpTree::On_namedNumber(int order, ParserValue & Value)
{
	if(order == 1)
	{
		if(m_tmp_type_ptr->getTypeExtended().isEmpty() == false)
			m_tmp_type_ptr->addTypeExtended(", ");
		m_tmp_type_ptr->addTypeExtended(Value.get());

		// add an integer value
		m_tmp_integer_value.clear();
		m_tmp_integer_value.NameAndClear(m_tmp_name);
		m_tmp_integer_value.m_type.setTypeInteger();
	}
	else if(order == 2)
	{
		m_tmp_type_ptr->addTypeExtended("(");
		m_tmp_type_ptr->addTypeExtended(Value.get());
		m_tmp_type_ptr->addTypeExtended(")");

		// complete integer value
		m_tmp_integer_value.m_oid.add(Value.get().toInt());
		m_module_values.push_back(m_tmp_integer_value);
		m_tmp_integer_value.clear();
	}
}

void SnmpTree::On_numberValue(int order, ParserValue & Value)
{
	if( (order == 1) && (m_tmp_type.isMacroTrapType() == true) )
	{
		// set value for the trap
		int val = Value.get().toInt();
		//DEBUG("Value %d for trap '%s'", val, m_tmp_value.getName().toLatin1().constData());
		m_tmp_value.m_oid.add(val);
	}
}

void SnmpTree::On_type1(int order, ParserValue & /*Value*/)
{
	if(order == 1)
	{
		// first call is the init of the type
		m_type_level++;
	}
	else if(order == 2)
	{
		// second call is the close of the type
		m_type_level--;
	}
	else
	{
		// not possible
		throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);
	}

	// some debug message
	//DEBUG("On_Type1(): Level %d, order=%d, Line=%d, value=%s", m_type_level, order, m_mib_line, Value.get().toLatin1().constData());

	if(m_type_level == 0)
	{
		// level = 0 is uniquely possible when closing the type
		if(order == 1)
			throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);

		// reset pointer
		m_tmp_type_ptr = nullptr;
		return;
	}

	if(m_type_level == 1)
	{
		if(order == 1)
		{
			m_tmp_type_ptr = &m_tmp_type;
			m_tmp_type_ptr->clear();
			m_tmp_type_ptr->Name(m_tmp_name);
			m_tmp_type_ptr->MibLine(m_mib_line);
			m_tmp_type_ptr->MibModule(m_mib_filename);
		}
		else
			m_tmp_type_ptr = &m_tmp_type;
		return;
	}

	if(m_type_level == 2)
	{
		if(order == 1)
		{
			m_tmp_type_ptr = &m_tmp_type.addCompound();
			m_tmp_type_ptr->clear();
			m_tmp_type_ptr->Name(m_tmp_name);
			m_tmp_type_ptr->MibLine(m_mib_line);
			m_tmp_type_ptr->MibModule(m_mib_filename);
		}
		else
			m_tmp_type_ptr = &m_tmp_type.lastCompound();
		return;
	}

	if(m_type_level == 3)
	{
		if(order == 1)
		{
			m_tmp_type_ptr = &m_tmp_type.lastCompound().addCompound();
			m_tmp_type_ptr->clear();
			m_tmp_type_ptr->Name(m_tmp_name);
			m_tmp_type_ptr->MibLine(m_mib_line);
			m_tmp_type_ptr->MibModule(m_mib_filename);
		}
		else
			throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);
		return;
	}

	throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);
}

void SnmpTree::On_typeAssignment(int /*order*/, ParserValue & /*Value*/)
{
	//DEBUG("On_typeAssignment(): Line %d, order=%d, Level=%d, value=%s", m_mib_line, order, m_type_level, Value.get().toLatin1().constData());
	//m_tmp_type.Dump(0);

	m_tmp_type.UserChoice(m_is_user_choice);
	m_module_types.push_back(m_tmp_type);
	m_tmp_type.clear();
}

void SnmpTree::On_valueAssignment(int order, ParserValue & /*Value*/)
{
	//DEBUG("On_valueAssignment(): Line %d, order=%d, Level=%d, value=%s", m_mib_line, order, m_type_level, Value.get().toLatin1().constData());

	if(order == 1)
	{
		m_tmp_value.clear();
		m_tmp_value.NameAndClear(m_tmp_name);
		m_tmp_value.MibLine(m_mib_line);
		m_tmp_value.MibModule(m_mib_filename);
	}
	else if(order == 2)
	{
		m_tmp_value.UserChoice(m_is_user_choice);
		m_tmp_value.m_type.setType(m_tmp_type);
		m_module_values.push_back(m_tmp_value);
		m_tmp_value.clear();
	}
}

void SnmpTree::On_sizeConstraint(int order, ParserValue &)
{
	if(order == 1)
		m_tmp_type_ptr->addTypeExtended("Size(");
	else if(order == 2)
		m_tmp_type_ptr->addTypeExtended(")");
}

void SnmpTree::_check_values(const QVector<SnmpValue> & values,
									  const QVector<SnmpType> & types,
									  bool check_macro)
{
	// verify that unknown names are imported
	for(const auto & value : values)
	{
		// check type
		_check_type(types, value.m_type, check_macro);

		// check oid
		const SnmpOid & oid = value.m_oid;
		for(const auto & member : oid.getVal())
		{
			if(member.Name().length() != 0)
			{
				if(member.isString() == true)
				{
					// name and string values are defined, don't know what to do
					throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);
				}

				if(member.isInteger() == true)
				{
					// name and integer values (like org(3) are defined, skip it
					continue;
				}

				// check that name exists
				if(_check_value_name(values, member.Name()) == false)
					throw ParserExceptionSnmpValueNotFound(member.Name(), value.MibModule(), value.MibLine());
			}
		}
	}
}

void SnmpTree::_check_types(const QVector<SnmpType> & types, bool check_macro)
{
	// verify that unknown names are imported
	for(const auto & type : types)
		_check_type(types, type, check_macro);
}

void SnmpTree::_check_type(const QVector<SnmpType> & types,
									const SnmpType & type,
									bool check_macro) const
{
	// 1st, works on base type
	if(type.isTypeDefined() == true)
	{
		// get name of the defined type
		const QString & defined = type.getTypeDefined();
		if(check_macro == true)
		{
			if(_check_type_name(types, defined) == false)
				throw ParserExceptionSnmpTypeNotFound(defined, type.MibModule(), type.MibLine());
		}
		else if(defined != StringMatch::SnmpTypeTranslate(SnmpType::TypeMacroObjectType))
		{
			if(_check_type_name(types, defined) == false)
				throw ParserExceptionSnmpTypeNotFound(defined, type.MibModule(), type.MibLine());
		}
	}

	// 2nd, works on compound types
	for(const auto & compound_type : type.getCompound())
	{
		if(compound_type.isTypeDefined() == true)
		{
			// get name of the defined type
			const QString & defined = compound_type.getTypeDefined();
			if(_check_type_name(types, defined) == false)
				throw ParserExceptionSnmpTypeNotFound(defined, compound_type.MibModule(), compound_type.MibLine());
		}
	}
}

bool SnmpTree::_check_type_name(const QVector<SnmpType> & types, const QString & name) const
{
	// check with names in module types
	for(const auto & type : types)
	{
		if(type.Name() == name)
			return true;
	}

	// then check in imported symbol
	if(m_import_manager.CheckSymbolImported(m_mib_filename, name) == true)
		return true;

	// symbol not found
	return false;
}

bool SnmpTree::_check_value_name(const QVector<SnmpValue> & values, const QString & name) const
{
	// check with names in module types
	for(const auto & value : values)
	{
		if(value.Name() == name)
			return true;
	}

	// then check in imported symbol
	if(m_import_manager.CheckSymbolImported(m_mib_filename, name) == true)
		return true;

	// symbol not found
	return false;
}

bool SnmpTree::_resolve_oid(QVector<SnmpValue> & values)
{
	bool change = false;
	for(auto & value : values)
	{
		QVector<SnmpOidMember> & oid_members = value.m_oid.getVal();
		for(int pos = 0; pos != oid_members.size(); pos++)
		{
			const SnmpOidMember & oid_member = oid_members[pos];

			// check if oid member is defined
			if(oid_member.isDefined() == true)
				continue;

			// not defined, a new pass will be mandatory
			change = true;

			// find the correct value in values
			const QVector<SnmpOidMember> & vals = _get_oid_by_name(values, oid_member.Name()).getVal();

			// remove non defined value
			oid_members.removeAt(pos);

			// insert replacement values
			for(const auto & mem : vals)
			{
				oid_members.insert(pos, mem);
				pos++;
			}

			// the oid has been modified, stop to analyse it, it will be re-analized next pass
			break;
		}
	}

	return change;
}

const SnmpOid & SnmpTree::_get_oid_by_name(const QVector<SnmpValue> & values, const QString & name) const
{
	for(const auto & value : values)
	{
		if(value.Name() == name)
			return value.m_oid;
	}

	// not found
	throw ParserExceptionSnmpValueNotFound(name);
}

void SnmpTree::On_macroModuleCompliance(int order, ParserValue & /*Value*/)
{
	if(order == 1)
	{
		m_tmp_type_ptr->setTypeMacroModuleCompliance();
	}
	else if(order == 2)
	{
		m_tmp_type_ptr->Lock();
	}
	else if(order == 3)
	{
		m_tmp_type_ptr->Unlock();
	}
}

void SnmpTree::On_macroAgentCapabilities(int order, ParserValue & /*Value*/)
{
	if(order == 1)
	{
		m_tmp_type_ptr->setTypeMacroAgentCapabilities();
	}
	else if(order == 2)
	{
		m_tmp_type_ptr->Lock();
	}
	else if(order == 3)
	{
		m_tmp_type_ptr->Unlock();
	}
}

inline void SnmpTree::On_macroModuleConformance(int order, ParserValue & /*Value*/)
{
	if(order == 1)
	{
		m_tmp_type_ptr->setTypeMacroModuleConformance();
	}
	else if(order == 2)
	{
		m_tmp_type_ptr->Lock();
	}
	else if(order == 3)
	{
		m_tmp_type_ptr->Unlock();
	}
}
