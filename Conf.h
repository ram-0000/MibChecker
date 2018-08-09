#ifndef CONF_H
#define CONF_H
#include <QColor>

class Conf
{
public:
	static QString Company(void);
	static QString Application(void);
	static QString Copyright(void);
	static QString Version(void);
	static QString AuthorName(void);
	static QString AuthorMail(void);
	static QString SiteName(void);
	static QString SiteUrl(void);

	static QString InputFolder(void);
	static void InputFolder(const QString & value);

	static QString BadFolder(void);
	static void BadFolder(const QString & value);

	static QString OutputMibFolder(void);
	static void OutputMibFolder(const QString & value);

	static QString OutputDefFolder(void);
	static void OutputDefFolder(const QString & value);

	static QString OutputHtmlFolder(void);
	static void OutputHtmlFolder(const QString & value);

	static QByteArray WndMainGeometry(void);
	static void WndMainGeometry(const QByteArray value);

	static QByteArray WndMainState(void);
	static void WndMainState(const QByteArray value);

	static QString EditorPath(void);
	static void EditorPath(const QString & value);

	static QString MibExtension(void);
	static void MibExtension(const QString & value);

	static QString DefExtension(void);
	static void DefExtension(const QString & value);

	static QString HtmlExtension(void);
	static void HtmlExtension(const QString & value);

	static int OverwriteAlgorithm(void);
	static void OverwriteAlgorithm(int value);

protected:

private:
	// identifiers of values
	typedef enum { ConfIdCompany,
						ConfIdApplication,
						ConfIdCopyright,
						ConfIdVersion,
						ConfIdAuthorName,
						ConfIdAuthorMail,
						ConfIdSiteName,
						ConfIdSiteUrl,

						ConfIdInputFolder,
						ConfIdBadFolder,
						ConfIdOutputDefFolder,
						ConfIdOutputHtmlFolder,
						ConfIdOutputMibFolder,
						ConfIdWndMainGeometry,
						ConfIdWndMainState,
						ConfIdEditorPath,
						ConfIdMibExtension,
						ConfIdDefExtension,
						ConfIdHtmlExtension,
						ConfIdOverwriteAlgorithm,

					 } ConfId_t;

	// type of value
	typedef enum { ConfTypeInteger, ConfTypeString, ConfTypeArray } ConfType_t;

	// source where values come from
	typedef enum { ConfSrcHard, ConfSrcVersion, ConfSrcRegUser, ConfSrcRegHost } ConfSrc_t;

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
	static inline QColor Color(ConfId_t id) { return QColor(Integer(id)); }
	static inline void Color(ConfId_t id, const QColor & value) { Integer(id, value.rgb()); }

	// to find an configuration element
	static ConfElt_t * Find(ConfId_t id);

	// table of configuration values
	static ConfElt_t m_table[];

	// private constuctor, it is a static only class
	inline Conf() { }

	static ConfElt_t * table(void);
};

#endif // CONF_H
