CONFIG(debug, debug|release) {
	DEFINES += MCR_DEBUG
} else {
	DEFINES += MCR_RELEASE
}

#-------------------------------------------------
#
# Project created by QtCreator 2017-04-11T08:59:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MibChecker
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
	 Parser/ParserException.cpp \
	 Debug.cpp \
	 Parser/MappedFile.cpp \
	 Parser/MibLexer.cpp \
	 Parser/MibParser.cpp \
	 Parser/VariantParam.cpp \
	 Parser/ParserPow.cpp \
	 Parser/ParserGeneric.cpp \
	 Parser/ParserItem.cpp \
	 Parser/ParserContextExec.cpp \
	 Parser/ListParserContextExec.cpp \
	 Parser/ParserValue.cpp \
	 Parser/ParserAction.cpp \
	 Parser/SnmpOid.cpp \
	 Parser/SnmpOidMember.cpp \
	 Parser/SnmpValue.cpp \
	 Parser/SnmpTree.cpp \
	 Parser/SnmpType.cpp \
	 Parser/MibFolder.cpp \
	 Conf.cpp \
	 MainWindow.cpp \
	 Ifs/Ifs.cpp \
	 Ifs/IfsDosHeader.cpp \
	 Ifs/IfsFileHeader.cpp \
	 Ifs/IfsImageDataDirectory.cpp \
	 Ifs/IfsImageFile.cpp \
	 Ifs/IfsImageResourceDataEntry.cpp \
	 Ifs/IfsImageResourceDirectory.cpp \
	 Ifs/IfsImageResourceDirectoryEntry.cpp \
	 Ifs/IfsImageSectionHeader.cpp \
	 Ifs/IfsNtHeaders.cpp \
	 Ifs/IfsOptionalHeader32.cpp \
	 Ifs/IfsOptionalHeader64.cpp \
	 Ifs/IfsPointer.cpp \
	 Ifs/IfsRemainingCounter.cpp \
	 Ifs/IfsResVersion.cpp \
	 DlgAbout.cpp \
	 FolderManager.cpp \
	 EditorProcess.cpp \
	 MibCheck.cpp \
	 ErrorDisplay.cpp \
	 Parser/SnmpImportManager.cpp \
	 MibCheckerThread.cpp \
    AsciiFile.cpp \
    ShellProcess.cpp \
    Parser/StringMatch.cpp \
    Parser/SnmpStatus.cpp \
    Parser/SnmpAccess.cpp

FORMS += \
	 MainWindow.ui \
	 DlgAbout.ui

HEADERS += \
	 Parser/ParserException.h \
	 Debug.h \
	 Parser/MappedFile.h \
	 Parser/MibLexer.h \
	 Parser/MibParser.h \
	 Parser/VariantParam.h \
	 Parser/ParserPow.h \
	 Parser/ParserGeneric.h \
	 Parser/ParserItem.h \
	 Parser/ParserContextExec.h \
	 Parser/ListParserContextExec.h \
	 Parser/ParserValue.h \
	 Parser/ParserAction.h \
	 Parser/SnmpOid.h \
	 Parser/SnmpOidMember.h \
	 Parser/SnmpValue.h \
	 Parser/SnmpTree.h \
	 Parser/SnmpType.h \
	 Parser/MibFolder.h \
	 Conf.h \
	 MainWindow.h \
	 Ifs/Ifs.h \
	 Ifs/IfsDosHeader.h \
	 Ifs/IfsFileHeader.h \
	 Ifs/IfsImageDataDirectory.h \
	 Ifs/IfsImageFile.h \
	 Ifs/IfsImageResourceDataEntry.h \
	 Ifs/IfsImageResourceDirectory.h \
	 Ifs/IfsImageResourceDirectoryEntry.h \
	 Ifs/IfsImageSectionHeader.h \
	 Ifs/IfsNtHeaders.h \
	 Ifs/IfsOptionalHeader32.h \
	 Ifs/IfsOptionalHeader64.h \
	 Ifs/IfsPointer.h \
	 Ifs/IfsRemainingCounter.h \
	 Ifs/IfsResVersion.h \
	 DlgAbout.h \
	 FolderManager.h \
	 EditorProcess.h \
	 MibCheck.h \
	 ErrorDisplay.h \
	 Parser/SnmpImportManager.h \
	 MibCheckerThread.h \
    AsciiFile.h \
    ShellProcess.h \
    Parser/StringMatch.h \
    Parser/SnmpStatus.h \
    Parser/SnmpAccess.h

DISTFILES += \
	 Grammar/asn1.grammar \
    MibChecker.ver \
    Grammar/asn.g

RESOURCES += \
	 Res.qrc

RC_ICONS += res/MibChecker.ico


# will be filled during setup generation
# these fields must be big enougth to be 'patched'
VERSION                  = 9999.9999.9999.9999
QMAKE_TARGET_COMPANY     = "xxxxCompanyxxxx"
QMAKE_TARGET_DESCRIPTION = "xxxxDescriptionxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
QMAKE_TARGET_COPYRIGHT   = "xxxxCopyrightxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
QMAKE_TARGET_PRODUCT     = "MibChecker"
