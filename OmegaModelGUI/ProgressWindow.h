#pragma once

#include <QObject>
#include "ui_ProgressWindow.h"

class ProgressWindow : public QWidget
{
	Q_OBJECT

public:

	ProgressWindow(QWidget *parent);
	~ProgressWindow();

	void SetProgressValue(int value);
	void SetProgressText(QString text);
	void SerialistaionComplete();

private slots:

	void onClickDone();
	void onClickQuit();

private:

	QWidget * progressWin;
	Ui::Form ui;
};
