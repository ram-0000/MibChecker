#include "FolderManager.h"
#include "Conf.h"
#include <QDir>

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
	m_list->addItems(list_entry);

	// display number of items
	QString str;
	str = QString::asprintf("%d file", list_entry.size());
	if(list_entry.size() > 1)
		str += "s";
	m_label->setText(str);

	Reselect();
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
