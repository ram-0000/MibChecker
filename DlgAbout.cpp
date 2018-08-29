#include "DlgAbout.h"
#include "ui_DlgAbout.h"
#include "Conf.h"

DlgAbout::DlgAbout(QWidget *parent /*= nullptr */)
	: QDialog(parent)
	, ui(new Ui::DlgAbout)
{
	ui->setupUi(this);
	setWindowTitle ("About...");

	// remove question mark in the title
	setWindowFlags(windowFlags() & (~Qt::WindowContextHelpButtonHint));

	QString str;

	// set application name and version number
	str = Conf::Application() + " - ";
	str += " version " + Conf::Version();
	setLabel(ui->ProgramName, str);

	// set build date
	str = "";
#ifdef MCR_DEBUG
			str += "Debug";
#else
	str += "Release";
#endif
	str += " build - ";
	str += Conf::FileDate();
	setLabel(ui->BuildDate, str);

	// set copyright with author mail
	str = Conf::Copyright() + " - <a href='" + Conf::AuthorMail() + "'>" + Conf::AuthorName() + "</a>";
	setLabel(ui->Copyright, str);

	// set site url
	str = "<a href='" + Conf::SiteUrl() + "'>" + Conf::SiteName() + "</a>";
	setLabel(ui->Url, str);
}

DlgAbout::~DlgAbout(void)
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
