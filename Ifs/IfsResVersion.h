#ifndef IFS_RESVERSION_H
#define IFS_RESVERSION_H

#include <QtGlobal>
#include <QString>
#include <QVector>
#include "IfsPointer.h"
#include "IfsRemainingCounter.h"

// see https://msdn.microsoft.com/en-us/library/windows/desktop/ms646994%28v=vs.85%29.aspx
typedef struct tagResVersion_Var
{
	quint16	wLength;
	quint16	wValueLength;
	quint16	wType;
	QString	szKey;					// must be L"Translation"
	//quint8	Padding;					// to perform a 4 bytes alignment
	quint32	Value;
} ResVersion_Var;

// see https://msdn.microsoft.com/en-us/library/windows/desktop/ms646995%28v=vs.85%29.aspx
typedef struct tagResVersion_VarFileInfo
{
  quint16			wLength;
  quint16			wValueLength;
  quint16			wType;
  QString			szKey;					// must be L"VarFileInfo"
  //quint8			Padding;					// to perform a 4 bytes alignment
  QVector<ResVersion_Var>	var;
} ResVersion_VarFileInfo;

// see https://msdn.microsoft.com/en-us/library/windows/desktop/ms646987%28v=vs.85%29.aspx
typedef struct tagResVersion_String
{
	quint16	wLength;
	quint16	wValueLength;
	quint16	wType;
	QString	szKey;
	//quint8	Padding;					// to perform a 4 bytes alignment
	QString	Value;
	//quint8	Padding;					// to perform a 4 bytes alignment
} ResVersion_String;

// see https://msdn.microsoft.com/en-us/library/windows/desktop/ms646992%28v=vs.85%29.aspx
typedef struct tagResVersion_StringTable
{
	quint16				wLength;
	quint16				wValueLength;
	quint16				wType;
	QString				szKey;
	//quint8				Padding;					// to perform a 4 bytes alignment
	QVector<ResVersion_String>	string;
} ResVersion_StringTable;

// see https://msdn.microsoft.com/en-us/library/windows/desktop/ms646989%28v=vs.85%29.aspx
typedef struct tagResVersion_StringFileInfo
{
	quint16						wLength;
	quint16						wValueLength;
	quint16						wType;
	QString						szKey;					// must be L"StringFileInfo"
	//quint8						Padding;					// to perform a 4 bytes alignment
	QVector<ResVersion_StringTable>	stringTable;
} ResVersion_StringFileInfo;

// see https://msdn.microsoft.com/fr-fr/library/windows/desktop/ms646997%28v=vs.85%29.aspx
typedef struct tagResVersion_VS_FIXEDFILEINFO
{
	quint32	dwSignature;
	quint32	dwStrucVersion;
	quint32	dwFileVersionMS;
	quint32	dwFileVersionLS;
	quint32	dwProductVersionMS;
	quint32	dwProductVersionLS;
	quint32	dwFileFlagsMask;
	quint32	dwFileFlags;
	quint32	dwFileOS;
	quint32	dwFileType;
	quint32	dwFileSubtype;
	quint32	dwFileDateMS;
	quint32	dwFileDateLS;
} ResVersion_VS_FIXEDFILEINFO;

// see https://msdn.microsoft.com/en-us/library/windows/desktop/ms647001%28v=vs.85%29.aspx
typedef struct tagResVersion_VS_VERSIONINFO
{
	quint16								wLength;
	quint16								wValueLength;
	quint16								wType;
	QString								szKey;			// must be L"VS_VERSION_INFO"
	//quint8								Padding1;		// to perform a 4 bytes alignment
	ResVersion_VS_FIXEDFILEINFO	Value;
	//quint8								Padding2;		// to perform a 4 bytes alignment
	ResVersion_StringFileInfo		stringFileInfo;
	ResVersion_VarFileInfo			varFileInfo;
} ResVersion_VS_VERSIONINFO;

class IfsResVersion
{
public:
	IfsResVersion(const void * p_ptr);
	~IfsResVersion(void);

	inline bool IsValid(void) const { return m_valid; }

	quint8 GetFileVersion1(void) const;
	quint8 GetFileVersion2(void) const;
	quint8 GetFileVersion3(void) const;
	quint8 GetFileVersion4(void) const;

	quint8 GetProductVersion1(void) const;
	quint8 GetProductVersion2(void) const;
	quint8 GetProductVersion3(void) const;
	quint8 GetProductVersion4(void) const;

	inline QString Comments(const QString & CodePage = "") const
	{ return String("Comments", CodePage); }
	inline QString CompanyName(const QString & CodePage = "") const
	{ return String("CompanyName", CodePage); }
	inline QString FileDescription(const QString & CodePage = "") const
	{ return String("FileDescription", CodePage); }
	inline QString FileVersion(const QString & CodePage = "") const
	{ return String("FileVersion", CodePage); }
	inline QString InternalName(const QString & CodePage = "") const
	{ return String("InternalName", CodePage); }
	inline QString LegalCopyright(const QString & CodePage = "") const
	{ return String("LegalCopyright", CodePage); }
	inline QString LegalTrademarks(const QString & CodePage = "") const
	{ return String("LegalTrademarks", CodePage); }
	inline QString OriginalFilename(const QString & CodePage = "") const
	{ return String("OriginalFilename", CodePage); }
	inline QString PrivateBuild(const QString & CodePage = "") const
	{ return String("PrivateBuild", CodePage); }
	inline QString ProductName(const QString & CodePage = "") const
	{ return String("ProductName", CodePage); }
	inline QString ProductVersion(const QString & CodePage = "") const
	{ return String("ProductVersion", CodePage); }
	inline QString SpecialBuild(const QString & CodePage = "") const
	{ return String("SpecialBuild", CodePage); }

	QString String(const QString & Name, const QString & CodePage = "") const;

private:
	bool read_main_part(IfsPointer & p_ptr);

	bool read_string_file_info(IfsPointer & p_ptr);
	bool read_string_table(IfsPointer & p_ptr);
	bool read_string(ResVersion_StringTable & stringTable, IfsPointer & p_ptr);

	bool read_var_file_info(IfsPointer & p_ptr);
	bool read_var(IfsPointer & p_ptr);

	ResVersion_VS_VERSIONINFO m_version;

	bool fill_value(quint16 & p_val, IfsPointer & p_ptr);
	bool fill_value(quint32 & p_val, IfsPointer & p_ptr);
	bool fill_value(QString & p_val, IfsPointer & p_ptr, quint32 p_len, bool p_pad);

	bool padding(IfsPointer & p_ptr);

	IfsRemainingCounter m_counter;
	bool m_valid;
};

#endif // IFS_RESVERSION_H
