#include "Conf.h"
#include "Debug.h"
#include <QSettings>
#include "Ifs/IfsImageFile.h"
#include "Ifs/IfsResVersion.h"

Conf::ConfElt_t * Conf::Find(ConfId_t id)
{
	static Conf::ConfElt_t t[] =
	{
		{ ConfIdCompany,					ConfTypeString,  ConfSrcVersion,	"CompanyName",				"", 0, "", QByteArray(), false },
		{ ConfIdApplication,				ConfTypeString,  ConfSrcVersion,	"ProductName",				"", 0, "", QByteArray(), false },
		{ ConfIdCopyright,				ConfTypeString,  ConfSrcVersion,	"LegalCopyright",			"", 0, "", QByteArray(), false },
		{ ConfIdVersion,					ConfTypeString,  ConfSrcVersion,	"ProductVersion",			"1", 0, "", QByteArray(), false },
		{ ConfIdFileDate,					ConfTypeString,  ConfSrcVersion,	"FileDate", "1",			0, "", QByteArray(), false },
		{ ConfIdAuthorName,				ConfTypeString,  ConfSrcHard,		"",							"Raymond Mercier",							0, "", QByteArray(), false },
		{ ConfIdAuthorMail,				ConfTypeString,  ConfSrcHard,		"",							"mailto:Raymond.Mercier@circitor.fr",	0, "", QByteArray(), false },
		{ ConfIdSiteName,					ConfTypeString,  ConfSrcHard,		"",							"www.circitor.fr",							0, "", QByteArray(), false },
		{ ConfIdSiteUrl,					ConfTypeString,  ConfSrcHard,		"",							"http://www.circitor.fr",					0, "", QByteArray(), false },

		{ ConfIdInputFolder,				ConfTypeString,  ConfSrcRegUser,	"InputFolder",				"C:\\Users\\Raymond\\Documents\\Qt\\Mib\\ToBeChecked", 0, "", QByteArray(), false },
		{ ConfIdBadFolder,				ConfTypeString,  ConfSrcRegUser,	"BadFolder",				"C:\\Users\\Raymond\\Documents\\Qt\\Mib\\Bad", 0, "", QByteArray(), false },
		{ ConfIdOutputDefFolder,		ConfTypeString,  ConfSrcRegUser,	"DefFolder",				"C:\\Users\\Raymond\\Documents\\Qt\\Mib\\www\\Def", 0, "", QByteArray(), false },
		{ ConfIdOutputIncFolder,		ConfTypeString,  ConfSrcRegUser,	"IncFolder",				"C:\\Users\\Raymond\\Documents\\Qt\\Mib\\www\\Inc", 0, "", QByteArray(), false },
		{ ConfIdOutputHtmlFolder,		ConfTypeString,  ConfSrcRegUser,	"HtmlFolder",				"C:\\Users\\Raymond\\Documents\\Qt\\Mib\\www\\Html", 0, "", QByteArray(), false },
		{ ConfIdOutputMibFolder,		ConfTypeString,  ConfSrcRegUser,	"MibFolder",				"C:\\Users\\Raymond\\Documents\\Qt\\Mib\\www\\Mib", 0, "", QByteArray(), false },
		{ ConfIdWndMainGeometry,		ConfTypeArray,   ConfSrcRegUser,	"WndMainGeometry",		"", 0, "", QByteArray(), false },
		{ ConfIdWndMainState,			ConfTypeArray,   ConfSrcRegUser,	"WndMainState",			"", 0, "", QByteArray(), false },
		{ ConfIdEditorPath,				ConfTypeString,  ConfSrcRegUser,	"EditorPath",				"notepad.exe", 0, "", QByteArray(), false },
		{ ConfIdCmdPath,					ConfTypeString,  ConfSrcRegUser,	"CmdPath",					"cmd.exe", 0, "", QByteArray(), false },
		{ ConfIdMibExtension,			ConfTypeString,  ConfSrcRegUser,	"MibExtension",			".mib", 0, "", QByteArray(), false },
		{ ConfIdIncExtension,			ConfTypeString,  ConfSrcRegUser,	"IncExtension",			".inc", 0, "", QByteArray(), false },
		{ ConfIdDefExtension,			ConfTypeString,  ConfSrcRegUser,	"DefExtension",			".def", 0, "", QByteArray(), false },
		{ ConfIdHtmlExtension,			ConfTypeString,  ConfSrcRegUser,	"HtmlExtension",			".php", 0, "", QByteArray(), false },
		{ ConfIdOverwriteAlgorithm,	ConfTypeInteger, ConfSrcRegUser,	"OverwriteAlgorithm",	"0", 0, "", QByteArray(), false },
	};

	for(int boucle = 0; boucle != sizeof(t) / sizeof(t[0]); boucle++)
	{
		ConfElt_t * p = &t[boucle];
		if(p->Id == id)
			return p;
	}

	// element not found
	FATAL("Conf: unable to find element %d", id);
	return nullptr;
}

int Conf::Integer(ConfId_t id)
{
	// find element
	ConfElt_t * p = Find(id);
	if(p == nullptr)
		return 0;

	// check type
	if(p->Type != ConfTypeInteger)
	{
		FATAL("Conf: element %d is not an Integer", id);
		return 0;
	}

	// check if loaded
	if(p->Loaded == true)
		return p->IntegerVal;

	// set default value
	p->IntegerVal = QString(p->DefaultValue).toInt();

	// check if value hard set
	if(p->Source == ConfSrcHard)
	{
		p->Loaded = true;
		return p->IntegerVal;
	}

	// check if registry
	if(p->Source == ConfSrcRegUser)
	{
		QSettings settings(QSettings::NativeFormat,
								 QSettings::UserScope,
								 Company(),
								 Application());
		p->IntegerVal = settings.value(p->AccessPath, p->DefaultValue).toInt();
		p->Loaded = true;
		return p->IntegerVal;
	}

	// check if registry
	if(p->Source == ConfSrcRegHost)
	{
		QSettings settings(QSettings::NativeFormat,
								 QSettings::SystemScope,
								 Company(),
								 Application());
		p->IntegerVal = settings.value(p->AccessPath, p->DefaultValue).toInt();
		p->Loaded = true;
		return p->IntegerVal;
	}

	// check if Version resource
	if(p->Source == ConfSrcVersion)
	{
		// load version resource
		IfsImageFile img;
		if(img.IsValid() == false)
		{
			ERREUR("Conf: Unable to map file into memory to read Version resource");
			return 0;
		}

		// get version resource
		IfsResVersion version(img.GetResourceVersionPtr());
		if(version.IsValid() == false)
		{
			ERREUR("Conf: Unable to read Version resource");
			return 0;
		}

		// get values
		p->IntegerVal = version.String(p->AccessPath).toInt();
		p->Loaded = true;
		return p->IntegerVal;
	}

	// unknown source
	FATAL("Conf: element %d has an unknown source", id);
	return 0;
}

void Conf::Integer(ConfId_t id, int value)
{
	// find element
	ConfElt_t * p = Find(id);
	if(p == nullptr)
		return;

	// check type
	if(p->Type != ConfTypeInteger)
	{
		FATAL("Conf: element %d is not an Integer", id);
		return;
	}

	// check if value hard set
	if( (p->Source == ConfSrcHard) || (p->Source == ConfSrcVersion) )
	{
		// not possible to change hard value
		FATAL("Conf: unable to modify hard value of element %d", id);
		return;
	}

	// check if registry
	if(p->Source == ConfSrcRegUser)
	{
		QSettings settings(QSettings::NativeFormat,
								 QSettings::UserScope,
								 Company(),
								 Application());
		settings.setValue(p->AccessPath, value);
		p->IntegerVal = value;
		p->Loaded = true;
		return;
	}

	// check if registry
	if(p->Source == ConfSrcRegHost)
	{
		QSettings settings(QSettings::NativeFormat,
								 QSettings::SystemScope,
								 Company(),
								 Application());
		settings.setValue(p->AccessPath, value);
		p->IntegerVal = value;
		p->Loaded = true;
		return;
	}

	// unknown source
	FATAL("Conf: element %d has an unknown source", id);
	return;
}

QString Conf::String(ConfId_t id)
{
	// find element
	ConfElt_t * p = Find(id);
	if(p == nullptr)
		return "";

	// check type
	if(p->Type != ConfTypeString)
	{
		FATAL("Conf: element %d is not a String", id);
		return "";
	}

	// check if loaded
	if(p->Loaded == true)
		return p->StringVal;

	// set default value
	p->StringVal = QString(p->DefaultValue);

	// check if value hard set
	if(p->Source == ConfSrcHard)
	{
		p->Loaded = true;
		return p->StringVal;
	}

	// check if registry
	if(p->Source == ConfSrcRegUser)
	{
		QSettings settings(QSettings::NativeFormat,
								 QSettings::UserScope,
								 Company(),
								 Application());
		p->StringVal = settings.value(p->AccessPath, p->DefaultValue).toString();
		p->Loaded = true;
		return p->StringVal;
	}

	// check if registry
	if(p->Source == ConfSrcRegHost)
	{
		QSettings settings(QSettings::NativeFormat,
								 QSettings::SystemScope,
								 Company(),
								 Application());
		p->StringVal = settings.value(p->AccessPath, p->DefaultValue).toString();
		p->Loaded = true;
		return p->StringVal;
	}

	// check if Version resource
	if(p->Source == ConfSrcVersion)
	{
		// load version resource
		IfsImageFile img;
		if(img.IsValid() == false)
		{
			ERREUR("Conf: Unable to map file into memory to read Version resource");
			return "";
		}

		// get version resource
		IfsResVersion version(img.GetResourceVersionPtr());
		if(version.IsValid() == false)
		{
			ERREUR("Conf: Unable to read Version resource");
			return "";
		}

		// get values
		p->StringVal = version.String(p->AccessPath);
		p->Loaded = true;
		return p->StringVal;
	}

	// unknown source
	FATAL("Conf: element %d has an unknown source", id);
	return "";
}

void Conf::String(ConfId_t id, const QString & value)
{
	// find element
	ConfElt_t * p = Find(id);
	if(p == nullptr)
		return;

	// check type
	if(p->Type != ConfTypeString)
	{
		FATAL("Conf: element %d is not a String", id);
		return;
	}

	// check if value hard set
	if( (p->Source == ConfSrcHard) || (p->Source == ConfSrcVersion) )
	{
		// not possible to change hard value
		FATAL("Conf: unable to modify hard value of element %d", id);
		return;
	}

	// check if registry
	if(p->Source == ConfSrcRegUser)
	{
		QSettings settings(QSettings::NativeFormat,
								 QSettings::UserScope,
								 Company(),
								 Application());
		settings.setValue(p->AccessPath, value);
		p->StringVal = value;
		p->Loaded = true;
		return;
	}

	// check if registry
	if(p->Source == ConfSrcRegHost)
	{
		QSettings settings(QSettings::NativeFormat,
								 QSettings::SystemScope,
								 Company(),
								 Application());
		settings.setValue(p->AccessPath, value);
		p->StringVal = value;
		p->Loaded = true;
		return;
	}

	// unknown source
	FATAL("Conf: element %d has an unknown source", id);
	return;
}

QByteArray Conf::Array(ConfId_t id)
{
	// find element
	ConfElt_t * p = Find(id);
	if(p == nullptr)
		return QByteArray();

	// check type
	if(p->Type != ConfTypeArray)
	{
		FATAL("Conf: element %d is not an Array", id);
		return QByteArray();
	}

	//check source, Version cannot handle array
	if(p->Source == ConfSrcVersion)
	{
		FATAL("Conf: Version cannot handle Array");
		return QByteArray();
	}

	// check if loaded
	if(p->Loaded == true)
		return p->ArrayVal;

	// set default value (no default value possible for byte array)
	p->ArrayVal = QByteArray();

	// check if value hard set
	if(p->Source == ConfSrcHard)
	{
		p->Loaded = true;
		return p->ArrayVal;
	}

	// check if registry
	if(p->Source == ConfSrcRegUser)
	{
		QSettings settings(QSettings::NativeFormat,
								 QSettings::UserScope,
								 Company(),
								 Application());
		p->ArrayVal = settings.value(p->AccessPath, p->DefaultValue).toByteArray();
		p->Loaded = true;
		return p->ArrayVal;
	}

	// check if registry
	if(p->Source == ConfSrcRegHost)
	{
		QSettings settings(QSettings::NativeFormat,
								 QSettings::SystemScope,
								 Company(),
								 Application());
		p->ArrayVal = settings.value(p->AccessPath, p->DefaultValue).toByteArray();
		p->Loaded = true;
		return p->ArrayVal;
	}

	// unknown source
	FATAL("Conf: element %d has an unknown source", id);
	return QByteArray();
}

void Conf::Array(ConfId_t id, const QByteArray & value)
{
	// find element
	ConfElt_t * p = Find(id);
	if(p == nullptr)
		return;

	// check type
	if(p->Type != ConfTypeArray)
	{
		FATAL("Conf: element %d is not an Array", id);
		return;
	}

	// check if value hard set
	if( (p->Source == ConfSrcHard) || (p->Source == ConfSrcVersion) )
	{
		// not possible to change hard value
		FATAL("Conf: unable to modify hard value of element %d", id);
		return;
	}

	// check if registry
	if(p->Source == ConfSrcRegUser)
	{
		QSettings settings(QSettings::NativeFormat,
								 QSettings::UserScope,
								 Company(),
								 Application());
		settings.setValue(p->AccessPath, value);
		p->ArrayVal = value;
		p->Loaded = true;
		return;
	}

	// check if registry
	if(p->Source == ConfSrcRegHost)
	{
		QSettings settings(QSettings::NativeFormat,
								 QSettings::SystemScope,
								 Company(),
								 Application());
		settings.setValue(p->AccessPath, value);
		p->ArrayVal = value;
		p->Loaded = true;
		return;
	}

	// unknown source
	FATAL("Conf: element %d has an unknown source", id);
	return;
}
