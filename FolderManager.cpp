#include "FolderManager.h"
#include "Conf.h"
#include <QDir>
#include <QDateTime>

FolderManager::FolderManager(const QString & Folder, QListWidget * List, QLabel * Label)
{
	Set(Folder, List, Label);
}

void FolderManager::Set(const QString & Folder, QListWidget * List, QLabel * Label)
{
	m_folder_name = Folder;
	m_list = List;
	m_label = Label;
	m_current = -1;
	Reload();
}

void FolderManager::Reload(void)
{
	// create directory listing
	QDir dir(Folder(), "", QDir::Name, QDir::Files);
	QStringList list_entry = dir.entryList();

	// reset list
	m_list->clear();
	for(const auto & entry : list_entry)
	{
		// create new QListWidgetItem
		QListWidgetItem * w = new QListWidgetItem;
		w->setText(entry);
		SetToolTip(w, entry);
		m_list->addItem(w);
	}

	// display number of items
	QString str;
	str = QString::asprintf("%d file", list_entry.size());
	if(list_entry.size() > 1)
		str += "s";
	m_label->setText(str);

	Reselect();
}

void FolderManager::SetToolTip(QListWidgetItem * widget, const QString & Filename)
{
	// build complete file name
	QString file_name = Folder() + "/" + Filename;
	//DEBUG("file=%s", file_name.toLatin1().constData());
	QFileInfo file_info(file_name);

	// the locale
	QLocale locale;
	QString tip;

	// name of the file
	tip += Filename;

	tip += "\r\n";

	// size of the file
	tip += locale.toString(file_info.size());
	tip += " bytes";

	tip += "\r\n";

	// modification date of the file
	tip += file_info.lastModified().toString(locale.dateTimeFormat());

	// set the tooltip
	widget->setToolTip(tip);
}

QList<QListWidgetItem *> FolderManager::Selected(void)
{
	m_current = m_list->currentRow();
	return m_list->selectedItems();
}

void FolderManager::Reselect(void)
{
	if(m_current == -1) return;
	m_list->setCurrentRow(m_current);
}
