#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

class VideoDecoder;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);


protected Q_SLOTS:
	void openFile();
	void openRtsp();
	
	void retry();
	void pause();

private:
    Ui::MainWindowClass ui;
	VideoDecoder *m_pVideoDecoder;
};
