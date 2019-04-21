#ifndef CONF_H
#define CONF_H
#include <QColor>

class Conf
{
public:
	inline static QString Company(void)			{ return String(ConfIdCompany); }
	inline static QString Application(void)	{ return String(ConfIdApplication); }
	inline static QString Copyright(void)		{ return String(ConfIdCopyright); }
	inline static QString Version(void)			{ return String(ConfIdVersion); }
	inline static QString FileDate(void)		{ return String(ConfIdFileDate); }
	inline static QString AuthorName(void)		{ return String(ConfIdAuthorName); }
	inline static QString AuthorMail(void)		{ return String(ConfIdAuthorMail); }
	inline static QString SiteName(void)		{ return String(ConfIdSiteName); }
	inline static QString SiteUrl(void)			{ return String(ConfIdSiteUrl); }

	inline static QString InputFolder(void)				{ return String(ConfIdInputFolder); }
	inline static void InputFolder(const QString & value) { String(ConfIdInputFolder, value); }

	inline static QString BadFolder(void)				{ return String(ConfIdBadFolder); }
	inline static void BadFolder(const QString & value) { String(ConfIdBadFolder, value); }

	inline static QString OutputMibFolder(void)		{ return String(ConfIdOutputMibFolder); }
	inline static void OutputMibFolder(const QString & value) { String(ConfIdOutputMibFolder, value); }

	inline static QString OutputDefFolder(void)		{ return String(ConfIdOutputDefFolder); }
	inline static void OutputDefFolder(const QString & value) { String(ConfIdOutputDefFolder, value); }

	inline static QString OutputIncFolder(void)		{ return String(ConfIdOutputIncFolder); }
	inline static void OutputIncFolder(const QString & value) { String(ConfIdOutputIncFolder, value); }

	inline static QString OutputHtmlFolder(void)		{ return String(ConfIdOutputHtmlFolder); }
	inline static void OutputHtmlFolder(const QString & value) { String(ConfIdOutputHtmlFolder, value); }

	inline static QByteArray WndMainGeometry(void)	{ return Array(ConfIdWndMainGeometry); }
	inline static void WndMainGeometry(const QByteArray value) { Array(ConfIdWndMainGeometry, value); }

	inline static QByteArray WndMainState(void)		{ return Array(ConfIdWndMainState); }
	inline static void WndMainState(const QByteArray value) { Array(ConfIdWndMainState, value); }

	inline static QString EditorPath(void)				{ return String(ConfIdEditorPath); }
	inline static void EditorPath(const QString & value) { String(ConfIdEditorPath, value); }

	inline static QString CmdPath(void)				{ return String(ConfIdCmdPath); }
	inline static void CmdPath(const QString & value) { String(ConfIdCmdPath, value); }

	inline static QString MibExtension(void)			{ return String(ConfIdMibExtension); }
	inline static void MibExtension(const QString & value) { String(ConfIdMibExtension, value); }

	inline static QString IncExtension(void)			{ return String(ConfIdIncExtension); }
	inline static void IncExtension(const QString & value) { String(ConfIdIncExtension, value); }

	inline static QString DefExtension(void)			{ return String(ConfIdDefExtension); }
	inline static void DefExtension(const QString & value) { String(ConfIdDefExtension, value); }

	inline static QString HtmlExtension(void)			{ return String(ConfIdHtmlExtension); }
	inline static void HtmlExtension(const QString & value) { String(ConfIdHtmlExtension, value); }

	inline static int OverwriteAlgorithm(void)			{ return Integer(ConfIdOverwriteAlgorithm); }
	inline static void OverwriteAlgorithm(int value) { Integer(ConfIdOverwriteAlgorithm, value); }

protected:

private:
	// identifiers of values
	typedef enum { ConfIdCompany,
						ConfIdApplication,
						ConfIdCopyright,
						ConfIdVersion,
						ConfIdFileDate,
						ConfIdAuthorName,
						ConfIdAuthorMail,
						ConfIdSiteName,
						ConfIdSiteUrl,

						ConfIdInputFolder,
						ConfIdBadFolder,
						ConfIdOutputDefFolder,
						ConfIdOutputIncFolder,
						ConfIdOutputHtmlFolder,
						ConfIdOutputMibFolder,
						ConfIdWndMainGeometry,
						ConfIdWndMainState,
						ConfIdEditorPath,
						ConfIdCmdPath,
						ConfIdMibExtension,
						ConfIdIncExtension,
						ConfIdDefExtension,
						ConfIdHtmlExtension,
						ConfIdOverwriteAlgorithm,

					 } ConfId_t;

	// type of value
	typedef enum { ConfTypeInteger, ConfTypeString, ConfTypeArray } ConfType_t;

	// source where values come from
	typedef enum { ConfSrcHard, ConfSrcVersion, ConfSrcRegUser, ConfSrcRegHost } ConfSrc_t;

	// structure of an entry in table describing all existing entries
	typedef struct
	{	ConfId_t Id;
		ConfType_t Type;
		ConfSrc_t Source;
		const char * AccessPath;
		const char * DefaultValue;
		int IntegerVal;
		QString StringVal;
		QByteArray ArrayVal;
		bool Loaded;
	} ConfElt_t;

	// to manage integer values
	static int Integer(ConfId_t id);
	static void Integer(ConfId_t id, int value);

	// to manage string values
	static QString String(ConfId_t id);
	static void String(ConfId_t id, const QString & value);

	// to manage array values
	static QByteArray Array(ConfId_t id);
	static void Array(ConfId_t id, const QByteArray & value);

	// to manage color values
	inline static QColor Color(ConfId_t id) { return QColor(static_cast<quint32>(Integer(id))); }
	inline static void Color(ConfId_t id, const QColor & value) { Integer(id, static_cast<qint32>(value.rgb())); }

	// to find an configuration element
	static ConfElt_t * Find(ConfId_t id);

	// private constuctor, it is a static only class
	inline Conf() { }
};

#endif // CONF_H
