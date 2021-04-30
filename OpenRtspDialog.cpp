#include "OpenRtspDialog.h"
#include "ui_OpenRtspDialog.h"
#include <QFile>
#include <QDomDocument>
#include <QTextStream>

OpenRtspDialog::OpenRtspDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenRtspDialog)
{
    ui->setupUi(this);
	loadXml();
}

OpenRtspDialog::~OpenRtspDialog()
{
	saveXml();
    delete ui;
}

void OpenRtspDialog::on_pushButton_clicked()
{
	m_url = ui->comboBox->currentText();
	this->accept();
}

void OpenRtspDialog::loadXml()
{
	QFile file(QString("rtsp_list.xml"));
	if (!file.open(QIODevice::ReadOnly)) {
		return;
	}

	QDomDocument doc;
	bool bLoadFile = doc.setContent(&file);
	file.close();

	if (!bLoadFile)
		return;

	QDomElement eleRoot = doc.documentElement();
	for (int i = 0; i < eleRoot.childNodes().count(); i++) {
		QDomNode nodeItem = eleRoot.childNodes().at(i);
		if (!nodeItem.isElement())
			continue;

		QDomElement eleItem = nodeItem.toElement();
		QString url = eleItem.attribute(tr("url"));

		ui->comboBox->addItem(url);
	}
}
void OpenRtspDialog::saveXml()
{
	if (m_url.isEmpty())
		return;

	QDomDocument doc;
	QDomElement eleRoot;

	QFile file(QString("rtsp_list.xml"));
	if (file.open(QIODevice::ReadOnly)) {
		bool bLoadFile = doc.setContent(&file);
		file.close();

		if (!bLoadFile)
			return;

		eleRoot = doc.documentElement();
	}
	else
	{
		QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
		doc.appendChild(instruction);

		eleRoot = doc.createElement("rtsp");
		doc.appendChild(eleRoot);
	}
	

	for (int i = 0; i < eleRoot.childNodes().count(); i++) {
		QDomNode nodeUser = eleRoot.childNodes().at(i);
		if (!nodeUser.isElement())
			continue;

		QDomElement eleUser = nodeUser.toElement();
		QString url = eleUser.attribute(QString("url"));
		if (url != m_url)
			continue;

		eleRoot.removeChild(nodeUser);
		break;
	}


	QDomElement eleItem = doc.createElement(tr("item"));
	eleItem.setAttribute(QString("url"), m_url);
	eleRoot.insertBefore(eleItem, eleRoot.firstChild());

	if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
		return;

	QTextStream out(&file);
	doc.save(out, 4);
	file.close();
}