#if defined(_MSC_VER) && (_MSC_VER >= 1600)    
# pragma execution_character_set("utf-8")    
#endif
#include "MainWindow.h"
#include <QFileDialog>
#include "VideoDecoder.h"
#include "OpenRtspDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	QAction *action_openFile = ui.mainToolBar->addAction(tr("file"));
	QAction *action_openRtsp = ui.mainToolBar->addAction(tr("rtsp"));

	ui.mainToolBar->addSeparator();

	QAction *action_retry = ui.mainToolBar->addAction(tr("retry"));
	QAction *action_pause = ui.mainToolBar->addAction(tr("pause"));

	connect(action_openFile, SIGNAL(triggered()), this, SLOT(openFile()));
	connect(action_openRtsp, SIGNAL(triggered()), this, SLOT(openRtsp()));
	connect(ui.action_openFile, SIGNAL(triggered()), this, SLOT(openFile()));
	connect(ui.action_openRtsp, SIGNAL(triggered()), this, SLOT(openRtsp()));

	connect(action_retry, SIGNAL(triggered()), this, SLOT(retry()));
	connect(action_pause, SIGNAL(triggered()), this, SLOT(pause()));
	connect(ui.action_retry, SIGNAL(triggered()), this, SLOT(retry()));
	connect(ui.action_pause, SIGNAL(triggered()), this, SLOT(pause()));

	m_pVideoDecoder = new VideoDecoder(this);

#if 0
	connect(m_pVideoDecoder, SIGNAL(showYUV(QByteArray, int, int)), ui.widget_player, SLOT(showYUV(QByteArray, int, int)));
#else
	connect(m_pVideoDecoder, &VideoDecoder::showYUV, [=](QByteArray byte, int width, int height) mutable {
		ui.widget_player->showYUV(byte, width, height);
	});
#endif

	//m_pRtspDecoder->start("rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov");
	//m_pRtspDecoder->start("F:\\work\\Demo\\OpenGLPlayer\\OpenGLPlayer\\OpenGLPlayer\\ffmpegexe\\002.mp4");
}

void MainWindow::openFile()
{
	QFileDialog dlg(this);
	dlg.setWindowTitle(tr("Open video file"));
	dlg.setDirectory(".");

	dlg.setNameFilter(tr("Video Files(*.mp4)"));
	if (dlg.exec() == QDialog::Accepted) {
		QString path = dlg.selectedFiles().at(0);
		if (!path.isEmpty())
		{
			m_pVideoDecoder->stop();
			m_pVideoDecoder->start(path);
			ui.statusBar->showMessage(path);
		}
	}
}

void MainWindow::openRtsp()
{
	OpenRtspDialog dlg(this);
	if (dlg.exec() == QDialog::Accepted) {
		QString path = dlg.m_url;
		if (!path.isEmpty())
		{
			m_pVideoDecoder->stop();
			m_pVideoDecoder->start(path);
			ui.statusBar->showMessage(path);
		}
	}
}

void MainWindow::retry()
{
	m_pVideoDecoder->stop();
	m_pVideoDecoder->start();
}
void MainWindow::pause()
{
	m_pVideoDecoder->togglePause();
}