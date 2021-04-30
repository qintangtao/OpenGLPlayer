#ifndef OPENRTSPDIALOG_H
#define OPENRTSPDIALOG_H

#include <QDialog>

namespace Ui {
class OpenRtspDialog;
}

class OpenRtspDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenRtspDialog(QWidget *parent = nullptr);
    ~OpenRtspDialog();

	QString m_url;

private slots:
	void on_pushButton_clicked();

private:
	void loadXml();
	void saveXml();

private:
    Ui::OpenRtspDialog *ui;
};

#endif // OPENRTSPDIALOG_H
