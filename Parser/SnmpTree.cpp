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
	m_tmp_type.clear();
	m_tmp_type_ptr = NULL;
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
		// only dump OID values
		if(item.m_type.isTypeObjectIdentifier() == false)
			continue;
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
	{ "moduleName", On_moduleName },
	{ "typeName", On_typeName },
	{ "valueName", On_valueName },
	{ "identifierName", On_valueName },
	{ "anyName", On_anyName },

	{ "mibModule", On_mibModule },
	{ "importList", On_importList },
	{ "symbol", On_symbol },
	{ "symbolsFromModule", On_symbolsFromModule },
	{ "valueAssignment", On_valueAssignment },
	{ "definedType", On_definedType },
	{ "nullType", On_nullType },
	{ "booleanType", On_booleanType },
	{ "realType", On_realType },
	{ "integerType", On_integerType },
	{ "objectIdentifierType", On_objectIdentifierType },
	{ "stringType", On_stringType },
	{ "bitStringType", On_bitStringType },
	{ "bitsType", On_bitsType },
	{ "sequenceType", On_sequenceType },
	{ "sequenceOfType", On_sequenceOfType },
	{ "setType", On_setType },
	{ "setOfType", On_setOfType },
	{ "choiceType", On_choiceType },
	{ "enumeratedType", On_enumeratedType },
	{ "selectionType", On_selectionType },
	{ "anyType", On_anyType },
	{ "nameOrNumber1", On_nameOrNumber1 },
	{ "nameOrNumber2", On_nameOrNumber2 },
	{ "nameAndNumber1", On_nameAndNumber1 },
	{ "nameAndNumber2", On_nameAndNumber2 },
	{ "nameAndNumber2", On_nameAndNumber2 },

	{ "macroModuleIdentity", On_macroModuleIdentity },
	{ "macroObjectIdentity", On_macroObjectIdentity },
	{ "macroObjectType", On_macroObjectType },
	{ "macroNotificationType", On_macroNotificationType },
	{ "macroTrapType", On_macroTrapType },
	{ "macroTextualConvention", On_macroTextualConvention },
	{ "macroObjectGroup", On_macroObjectGroup },
	{ "macroNotificationGroup", On_macroNotificationGroup },
	{ "macroModuleCompliance", On_macroModuleCompliance },
	{ "macroAgentCapabilities", On_macroAgentCapabilities },

	{ "snmpDescrPart", On_snmpDescrPart },
	{ "snmpStatusPart", On_snmpStatusPart },
	{ "snmpAccessPart", On_snmpAccessPart },

	{ "namedNumber", On_namedNumber },
	{ "lowerEndPoint", On_lowerEndPoint },
	{ "upperEndPoint", On_upperEndPoint },
	{ "typeAssignment", On_typeAssignment },
	{ "type1", On_type1 },
	{ "sizeConstraint", On_sizeConstraint },
	{ "constraint", On_constraint },

};

void SnmpTree::OnAction(int MibLine,
								const char * rule,
								int order,
								ParserValue & Value)
{
	// store mibfile and mibline
	m_mib_line = MibLine;

	//DEBUG("%s - %d ==> '%s'", rule, order, Value.get().toLatin1().constData());
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
	if(m_tmp_type_ptr->is_type_extended_empty() == false)
		m_tmp_type_ptr->add_type_extended(" | ");
}

void SnmpTree::On_namedNumber(int order, ParserValue & Value)
{
	if(order == 1)
	{
		if(m_tmp_type_ptr->is_type_extended_empty() == false)
			m_tmp_type_ptr->add_type_extended(", ");
		m_tmp_type_ptr->add_type_extended(Value.get());

		// add an integer value
		m_tmp_integer_value.clear();
		m_tmp_integer_value.setName(Value.get());
		m_tmp_integer_value.m_type.setTypeInteger();
	}
	else if(order == 2)
	{
		m_tmp_type_ptr->add_type_extended("(");
		m_tmp_type_ptr->add_type_extended(Value.get());
		m_tmp_type_ptr->add_type_extended(")");

		// complete integer value
		m_tmp_integer_value.m_oid.add(Value.get().toInt());
		m_module_values.push_back(m_tmp_integer_value);
		m_tmp_integer_value.clear();
	}
}

void SnmpTree::On_type1(int order, ParserValue &)
{
	if(order == 1) m_type_level++;
	else if(order == 2) m_type_level--;
	else throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);

	//DEBUG("Type Order=%d, Level=%d", order, m_type_level);
	if(m_type_level == 0)
	{
		if(order == 1)
			throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);
		else
			m_tmp_type_ptr = NULL;
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

void SnmpTree::On_typeAssignment(int, ParserValue &)
{
	//m_tmp_type.Dump(0);

	m_tmp_type.UserChoice(m_is_user_choice);
	m_module_types.push_back(m_tmp_type);
	m_tmp_type.clear();
}

void SnmpTree::On_valueAssignment(int order, ParserValue &)
{
	if(order == 1)
	{
		m_tmp_value.clear();
		m_tmp_value.Name(m_tmp_name);
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
		m_tmp_type_ptr->add_type_extended("Size(");
	else if(order == 2)
		m_tmp_type_ptr->add_type_extended(")");
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
		else if(defined != SnmpType::NameMacroType())
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
		if(value.getName() == name)
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
		if(value.getName() == name)
			return value.m_oid;
	}

	// not found
	throw ParserExceptionSnmpValueNotFound(name);
}
