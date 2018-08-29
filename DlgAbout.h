#ifndef DLGABOUT_H
#define DLGABOUT_H

#include <QDialog>
#include <QLabel>

namespace Ui
{
	class DlgAbout;
}

class DlgAbout : public QDialog
{
	Q_OBJECT

public:
	explicit DlgAbout(QWidget *parent = nullptr);
	~DlgAbout(void);

private:
	Ui::DlgAbout *ui;
	void setLabel(QLabel * label, const QString & text);
};

#endif // DLGABOUT_H
