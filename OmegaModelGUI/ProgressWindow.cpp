#include "ProgressWindow.h"
#include <qthread.h>

ProgressWindow::ProgressWindow(QWidget *parent)
	: QWidget(parent)
{
	progressWin = new QWidget();
	
	ui.setupUi(progressWin);

	ui.progressBar->setRange(0, 100);
	ui.progressBar->setValue(10);
	progressWin->show();

	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(onClickDone()));			// done button
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(onClickQuit()));				// quit button
}

ProgressWindow::~ProgressWindow()
{
}

void ProgressWindow::onClickDone()
{
	progressWin->close();
}

void ProgressWindow::onClickQuit()
{

}

void ProgressWindow::SetProgressValue(int value)
{
	ui.progressBar->setValue(value);
}

void ProgressWindow::SetProgressText(QString text)
{
	ui.label->setText(text);
}

void ProgressWindow::SerialistaionComplete()
{
	ui.pushButton_2->setEnabled(true);
}
