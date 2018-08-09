#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Parser/SnmpTree.h"
#include "Parser/MibFolder.h"
#include "Debug.h"
#include "Conf.h"
#include "DlgAbout.h"
#include "EditorProcess.h"
#include "MibCheck.h"
#include <QDesktopServices>
#include <QFileDialog>

MainWindow::MainWindow(QWidget * parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	m_thread_running = false;

	_read_setting();
	_create_menu();

	// create folder managers
	m_check_manager = new FolderManager(ui->MibFileCheckFolderEdit->text(),
													ui->MibFileCheckList,
													ui->MibFileCheckLabel);
	m_bad_manager = new FolderManager(ui->MibFileBadFolderEdit->text(),
												 ui->MibFileBadList,
												 ui->MibFileBadLabel);
	m_last_manager = NULL;

	m_error = new ErrorDisplay(ui->ErrorEdit);
	_fill_algorithm();

	connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(onClose()));

	connect(ui->MibFileCheckList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onClickInput(QListWidgetItem*)));
	connect(ui->MibFileCheckList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onDoubleClickInput(QListWidgetItem*)));
	connect(ui->MibFileBadList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onClickBad(QListWidgetItem*)));

	connect(ui->EditButton, SIGNAL(clicked()), this, SLOT(onEdit()));
	connect(ui->DeleteButton, SIGNAL(clicked()), this, SLOT(onDelete()));
	connect(ui->CheckButton, SIGNAL(clicked()), this, SLOT(onCheck()));
	connect(ui->ReloadButton, SIGNAL(clicked()), this, SLOT(onReload()));
	connect(ui->ClearButton, SIGNAL(clicked()), this, SLOT(onClear()));
	connect(ui->OpenInputButton, SIGNAL(clicked(bool)), this, SLOT(onOpenInput()));
	connect(ui->OpenBadButton, SIGNAL(clicked(bool)), this, SLOT(onOpenBad()));
	connect(ui->ChooseInputFolderButton, SIGNAL(clicked(bool)), this, SLOT(onChooseInputFolder()));
	connect(ui->ChooseBadFolderButton, SIGNAL(clicked(bool)), this, SLOT(onChooseBadFolder()));
	connect(ui->DefFolderChooser, SIGNAL(clicked(bool)), this, SLOT(onDefFolderChooser()));
	connect(ui->MibFolderChooser, SIGNAL(clicked(bool)), this, SLOT(onMibFolderChooser()));
	connect(ui->HtmlFolderChooser, SIGNAL(clicked(bool)), this, SLOT(onHtmlFolderChooser()));
	connect(ui->DefFolderOpen, SIGNAL(clicked(bool)), this, SLOT(onDefFolderOpen()));
	connect(ui->MibFolderOpen, SIGNAL(clicked(bool)), this, SLOT(onMibFolderOpen()));
	connect(ui->HtmlFolderOpen, SIGNAL(clicked(bool)), this, SLOT(onHtmlFolderOpen()));
	connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onAlgorithmChanged()));
	connect(ui->CheckToBadButton, SIGNAL(clicked()), this, SLOT(onCheckToBadFolder()));
	connect(ui->BadToCheckButton, SIGNAL(clicked()), this, SLOT(onBadToCheckFolder()));
	connect(&m_thread, SIGNAL(threadStarted(int)), this, SLOT(onThreadStarted(int)));
	connect(&m_thread, SIGNAL(threadRunning(int, const QString, const QString)), this, SLOT(onThreadRunning(int, const QString, const QString)));
	connect(&m_thread, SIGNAL(threadFinished()), this, SLOT(onThreadFinished()));

	_button_state();
}

MainWindow::~MainWindow()
{
	if(m_error != NULL)
		delete m_error;
	if(m_check_manager != NULL)
		delete m_check_manager;
	if(m_bad_manager != NULL)
		delete m_bad_manager;
	_write_setting();
	delete ui;
}

void MainWindow::_read_setting(void)
{
	// set size and position
	restoreGeometry(Conf::WndMainGeometry());
	restoreState(Conf::WndMainState());

	// set main window title
	setWindowTitle(Conf::Application());

	// set window icon
	setWindowIcon(QIcon(":/res/MibChecker.png"));

	ui->MibFileCheckFolderEdit->setText(Conf::InputFolder());
	ui->MibFileBadFolderEdit->setText(Conf::BadFolder());
	ui->MibFileDefFolderEdit->setText(Conf::OutputDefFolder());
	ui->MibFileHtmlFolderEdit->setText(Conf::OutputHtmlFolder());
	ui->MibFileMibFolderEdit->setText(Conf::OutputMibFolder());
}

void MainWindow::_write_setting(void)
{
	// save size and position
	Conf::WndMainGeometry(saveGeometry());
	Conf::WndMainState(saveState());
}

void MainWindow::_create_menu(void)
{
	QAction * aboutAct = new QAction("F1\tÀ propos", this);
	aboutAct->setShortcut(Qt::Key_F1);
	aboutAct->setStatusTip("À propos");
	connect(aboutAct, &QAction::triggered, this, onAbout);

	QMenu * helpMenu = menuBar()->addMenu("&Aide");
	//helpMenu->addSeparator();
	helpMenu->addAction(aboutAct);
}

void MainWindow::onAbout(void)
{
	DlgAbout dlg(this);
	dlg.exec();
}

void MainWindow::onClose(void)
{
	QApplication::quit();
}

void MainWindow::_button_state(void)
{
	if(m_thread_running == true)
	{
		ui->progressBar->show();
		ui->BadToCheckButton->setEnabled(false);
		ui->CheckToBadButton->setEnabled(false);
		ui->CheckButton->setEnabled(false);
		ui->ReloadButton->setEnabled(true);
		ui->EditButton->setEnabled(false);
		ui->DeleteButton->setEnabled(false);
		return;
	}

	ui->progressBar->hide();
	int nb = 0;
	bool enabled;
	if(m_last_manager == m_check_manager)
	{
		ui->BadToCheckButton->setEnabled(false);
		nb = m_check_manager->Selected().size();
		enabled = false;
		if(nb > 0)
			enabled = true;
		ui->CheckToBadButton->setEnabled(enabled);
	}
	else if(m_last_manager == m_bad_manager)
	{
		ui->CheckToBadButton->setEnabled(false);
		nb = m_bad_manager->Selected().size();
		enabled = false;
		if(nb > 0)
			enabled = true;
		ui->BadToCheckButton->setEnabled(enabled);
	}
	else
	{
		ui->CheckToBadButton->setEnabled(false);
		ui->BadToCheckButton->setEnabled(false);
	}

	// check and edit buttons
	enabled = false;
	if( (m_last_manager == m_check_manager) && (nb != 0) )
		enabled = true;
	ui->CheckButton->setEnabled(enabled);
	ui->EditButton->setEnabled(enabled);

	// delete button
	enabled = false;
	if(nb != 0)
		enabled = true;
	ui->DeleteButton->setEnabled(enabled);

	// reload is always enabled
	ui->ReloadButton->setEnabled(true);
}

void MainWindow::onClickInput(QListWidgetItem * /*item*/)
{
	m_last_manager = m_check_manager;
	_button_state();
}

void MainWindow::onDoubleClickInput(QListWidgetItem * /*item*/)
{
	m_last_manager = m_check_manager;
	_button_state();
	onCheck();
}

void MainWindow::onClickBad(QListWidgetItem * /*item*/)
{
	m_last_manager = m_bad_manager;
	_button_state();
}

void MainWindow::onEdit(void)
{
	if(m_last_manager != m_check_manager)
		return;
	if(m_last_manager->Selected().size() == 0)
		return;
	DEBUG("OnEdit()");

	QList<QListWidgetItem *> list = m_last_manager->Selected();
	for(const auto & item : list)
	{
		new EditorProcess(m_last_manager->Folder(), item->text());
	}
}

void MainWindow::onDelete(void)
{
	if(m_last_manager->Selected().size() == 0)
		return;
	QList<QListWidgetItem *> list = m_last_manager->Selected();
	for(const auto & item : list)
	{
		QString path = m_last_manager->Folder() + "/" + item->text();
		if(QFile::remove(path) == false)
			m_error->Add(item->text(), "Unable to delete file");
	}
	onReload();
}

void MainWindow::onCheck(void)
{
	// check only in check list
	if(m_last_manager != m_check_manager)
		return;
	if(m_last_manager->Selected().size() == 0)
		return;
	DEBUG("OnCheck()");

	// create list of mib
	QList<QListWidgetItem *> list = m_last_manager->Selected();
	QStringList list_mib;
	for(const auto & item : list)
	{
		// get MIB module
		QString str = item->text();

		// remove extension (if exist)
		if(str.endsWith(Conf::MibExtension()) == true)
			str = str.left(str.length() - Conf::MibExtension().length());

		// add it to the list
		list_mib.append(str);
	}

	// get selected algorithm
	int data = ui->comboBox->currentData().toInt();
	MibCheck::OverwriteAlgorithm algo = static_cast<MibCheck::OverwriteAlgorithm>(data);

	// start the thread
	m_thread_running = true;
	m_thread.Start(algo, list_mib);
}

void MainWindow::onReload(void)
{
	DEBUG("OnReload()");
	QApplication::setOverrideCursor(Qt::WaitCursor);
	m_check_manager->Reload();
	m_bad_manager->Reload();
	QApplication::restoreOverrideCursor();
	_button_state();
}

void MainWindow::onClear(void)
{
	m_error->Clear();
}

void MainWindow::onOpenInput(void)
{
	QDesktopServices::openUrl(QUrl("file:///" + m_check_manager->Folder(), QUrl::TolerantMode));
}

void MainWindow::onOpenBad(void)
{
	QDesktopServices::openUrl(QUrl("file:///" + m_bad_manager->Folder(), QUrl::TolerantMode));
}

void MainWindow::onChooseInputFolder(void)
{
	// getExistingDirectory (	QWidget * parent = 0,
	//									const QString & caption = QString(),
	//									const QString & dir = QString(),
	//									Options options = ShowDirsOnly ) [static]
	QString folder = QFileDialog::getExistingDirectory(this,
																		"Choose input folder",
																		Conf::InputFolder(),
																		QFileDialog::ShowDirsOnly);
	if(folder.length() == 0)
		return;
	Conf::InputFolder(folder);
	ui->MibFileCheckFolderEdit->setText(Conf::InputFolder());
	m_check_manager->Set(ui->MibFileCheckFolderEdit->text(),
								ui->MibFileCheckList,
								ui->MibFileCheckLabel);
	onReload();
}

void MainWindow::onChooseBadFolder(void)
{
	QString folder = QFileDialog::getExistingDirectory(this,
																		"Choose bad folder",
																		Conf::BadFolder(),
																		QFileDialog::ShowDirsOnly);
	if(folder.length() == 0)
		return;
	Conf::BadFolder(folder);
	ui->MibFileBadFolderEdit->setText(Conf::BadFolder());
	m_bad_manager->Set(ui->MibFileBadFolderEdit->text(),
							 ui->MibFileBadList,
							 ui->MibFileBadLabel);
	onReload();
}

void MainWindow::onDefFolderChooser(void)
{
	QString folder = QFileDialog::getExistingDirectory(this,
																		"Choose Def folder",
																		Conf::OutputDefFolder(),
																		QFileDialog::ShowDirsOnly);
	if(folder.length() == 0)
		return;
	Conf::OutputDefFolder(folder);
	ui->MibFileDefFolderEdit->setText(Conf::OutputDefFolder());
}

void MainWindow::onMibFolderChooser(void)
{
	QString folder = QFileDialog::getExistingDirectory(this,
																		"Choose Mib folder",
																		Conf::OutputMibFolder(),
																		QFileDialog::ShowDirsOnly);
	if(folder.length() == 0)
		return;
	Conf::OutputMibFolder(folder);
	ui->MibFileMibFolderEdit->setText(Conf::OutputMibFolder());
}

void MainWindow::onHtmlFolderChooser(void)
{
	QString folder = QFileDialog::getExistingDirectory(this,
																		"Choose Html folder",
																		Conf::OutputHtmlFolder(),
																		QFileDialog::ShowDirsOnly);
	if(folder.length() == 0)
		return;
	Conf::OutputHtmlFolder(folder);
	ui->MibFileHtmlFolderEdit->setText(Conf::OutputHtmlFolder());
}

void MainWindow::onDefFolderOpen(void)
{
	QDesktopServices::openUrl(QUrl("file:///" + Conf::OutputDefFolder(), QUrl::TolerantMode));
}

void MainWindow::onMibFolderOpen(void)
{
	QDesktopServices::openUrl(QUrl("file:///" + Conf::OutputMibFolder(), QUrl::TolerantMode));
}

void MainWindow::onHtmlFolderOpen(void)
{
	QDesktopServices::openUrl(QUrl("file:///" + Conf::OutputHtmlFolder(), QUrl::TolerantMode));
}

void MainWindow::_fill_algorithm(void)
{
	ui->comboBox->addItem("Only check MIB file syntax", MibCheck::CheckOnly);
	ui->comboBox->addItem("Never overwrite MIB file", MibCheck::NeverOverwrite);
	ui->comboBox->addItem("Overwrite MIB file if size is greater", MibCheck::OverwriteIfGreaterSize);
	ui->comboBox->addItem("Always overwrite MIB file", MibCheck::AlwaysOverwrite);

	// find the currebnt algorithm
	int algo = Conf::OverwriteAlgorithm();
	int idx = ui->comboBox->findData(algo);

	// force to first algo if not found
	if(idx == -1)
		idx = 0;

	// select algorithm
	ui->comboBox->setCurrentIndex(idx);
}

void MainWindow::onAlgorithmChanged(void)
{
	// record selected algorithm
	int data = ui->comboBox->currentData().toInt();
	MibCheck::OverwriteAlgorithm algo = static_cast<MibCheck::OverwriteAlgorithm>(data);
	Conf::OverwriteAlgorithm(algo);
}

void MainWindow::onCheckToBadFolder(void)
{
	// check only in check list
	if(m_last_manager != m_check_manager)
		return;
	if(m_check_manager->Selected().size() == 0)
		return;
	DEBUG("onCheckToBadFolder()");

	QList<QListWidgetItem *> list_file = m_check_manager->Selected();
	for(const auto & file : list_file)
	{
		QString src = m_check_manager->Folder() + "/" + file->text();
		QFile f(src);

		QString dst = m_bad_manager->Folder() + "/" + file->text();
		f.rename(dst);
	}
	onReload();
}

void MainWindow::onBadToCheckFolder(void)
{
	// check only in check list
	if(m_last_manager != m_bad_manager)
		return;
	if(m_bad_manager->Selected().size() == 0)
		return;
	DEBUG("onBadToCheckFolder()");

	QList<QListWidgetItem *> list_file = m_bad_manager->Selected();
	for(const auto & file : list_file)
	{
		QString src = m_bad_manager->Folder() + "/" + file->text();
		QFile f(src);

		QString dst = m_check_manager->Folder() + "/" + file->text();
		f.rename(dst);
	}
	onReload();
}

void MainWindow::onThreadStarted(int Max)
{
	DEBUG("Thread is started");
	QApplication::setOverrideCursor(Qt::WaitCursor);
	ui->progressBar->setMinimum(0);
	ui->progressBar->setValue(0);
	ui->progressBar->setMaximum(Max);
	onClear();
	_button_state();
}

void MainWindow::onThreadRunning(int Pos, const QString Mibname, const QString Error)
{
	DEBUG("Thread is running");
	ui->progressBar->setValue(Pos);
	if(Error.length() != 0)
		m_error->Add(Mibname, Error);
}

void MainWindow::onThreadFinished(void)
{
	DEBUG("Thread is finished");
	QApplication::restoreOverrideCursor();
	m_thread_running = false;
	onReload();
}

