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
	void onOpenInput(void);
	void onOpenBad(void);
	void onChooseInputFolder(void);
	void onChooseBadFolder(void);
	void onDefFolderChooser(void);
	void onMibFolderChooser(void);
	void onHtmlFolderChooser(void);
	void onDefFolderOpen(void);
	void onMibFolderOpen(void);
	void onHtmlFolderOpen(void);
	void onAlgorithmChanged(void);
	void onCheckToBadFolder(void);
	void onBadToCheckFolder(void);
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
