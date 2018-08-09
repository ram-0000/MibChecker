#include "DlgAbout.h"
#include "ui_DlgAbout.h"
#include "Conf.h"

DlgAbout::DlgAbout(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DlgAbout)
{
	ui->setupUi(this);

	QString str;

	str = Conf::Application() +
#ifdef MCR_DEBUG
			" - debug version "
#else
			" - release version "
#endif
			+ Conf::Version();
	setLabel(ui->ProgramName, str);

	str = Conf::Copyright() + " - <a href='" +
			Conf::AuthorMail() +
			"'>" +
			Conf::AuthorName() +
			"</a>";
	setLabel(ui->Copyright, str);

	str = "<a href='" +
			Conf::SiteUrl() +
			"'>" +
			Conf::SiteName() +
			"</a>";
	setLabel(ui->Url, str);
}

DlgAbout::~DlgAbout()
{
	delete ui;
}

void DlgAbout::setLabel(QLabel * label, const QString & text)
{
	label->setText(text);
	label->setTextFormat(Qt::RichText);
	label->setTextInteractionFlags(Qt::TextBrowserInteraction);
	label->setOpenExternalLinks(true);
}
