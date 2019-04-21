#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "FolderManager.h"
#include "ErrorDisplay.h"
#include "MibCheckerThread.h"

namespace Ui
{
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget * parent = nullptr);
	~MainWindow();

public slots:
	void onAbout(void);
	void onClose(void);
	void onClickInput(QListWidgetItem * item);
	void onDoubleClickInput(QListWidgetItem * item);
	void onClickBad(QListWidgetItem * item);
	void onDoubleClickBad(QListWidgetItem * item);
	void onEdit(void);
	void onDelete(void);
	void onCheck(void);
	void onReload(void);
	void onClear(void);
	void onCheckToBadFolder(void);
	void onBadToCheckFolder(void);

	void onInputFolderOpen(void);
	void onInputFolderChoose(void);
	void onInputFolderCmd(void);

	void onBadFolderOpen(void);
	void onBadFolderChoose(void);
	void onBadFolderCmd(void);

	void onOutputMibFolderChooser(void);
	void onOutputMibFolderOpen(void);
	void onOutputMibFolderCmd(void);

	void onOutputHtmlFolderChooser(void);
	void onOutputHtmlFolderOpen(void);
	void onOutputHtmlFolderCmd(void);

	void onOutputDefFolderChooser(void);
	void onOutputDefFolderOpen(void);
	void onOutputDefFolderCmd(void);

	void onOutputIncFolderChooser(void);
	void onOutputIncFolderOpen(void);
	void onOutputIncFolderCmd(void);

	void onAlgorithmChanged(void);

	void onThreadStarted(int Max);
	void onThreadRunning(int Current, const QString Mibname, const QString Error);
	void onThreadFinished(void);

private:
	Ui::MainWindow *ui;
	FolderManager * m_check_manager;
	FolderManager * m_bad_manager;
	FolderManager * m_last_manager;
	ErrorDisplay * m_error;

	void _read_setting(void);
	void _write_setting(void);
	void _create_menu(void);
	void _button_state(void);
	void _fill_algorithm(void);

	MibCheckerThread m_thread;
	bool m_thread_running;

};

#endif // MAINWINDOW_H
