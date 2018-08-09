#ifndef FOLDERMANAGER_H
#define FOLDERMANAGER_H

#include <QString>
#include <QListWidget>
#include <QLabel>

class FolderManager
{
public:
	FolderManager(const QString & Folder, QListWidget * List, QLabel * Label);

	inline const QString & Folder(void) const { return m_folder_name; }

	void Set(const QString & Folder, QListWidget * List, QLabel * Label);

	void Reload(void);

	QList<QListWidgetItem *> Selected(void);

private:
	void Reselect(void);

	QString m_folder_name;
	QListWidget * m_list;
	QLabel * m_label;
	int m_current;
};

#endif // FOLDERMANAGER_H
