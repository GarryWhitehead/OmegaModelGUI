#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OmegaModelGUI.h"

class ProgressWindow;

class OmegaModelGUI : public QMainWindow
{
	Q_OBJECT

public:

	OmegaModelGUI(QWidget *parent = Q_NULLPTR);

	bool CheckFileFormat(QString filename);

	// overidden functions to alow for dragging of files into main window
	void dragEnterEvent(QDragEnterEvent *event) override;
	void dropEvent(QDropEvent *event) override;
	void dragMoveEvent(QDragMoveEvent *event) override;

private slots:
	
	void onClickClear();
	void onClickQuit();
	void onClickSerialise();
	void onClickSetDestination();

private:

	Ui::OmegaModelGUIClass ui;
	ProgressWindow *progressWin;

	QString destination;
};
