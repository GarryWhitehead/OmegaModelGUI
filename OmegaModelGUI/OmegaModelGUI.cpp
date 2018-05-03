#include "OmegaModelGUI.h"
#include "file_log.h"
#include "OMFConverter/OMFConverter.h"
#include "ProgressWindow.h"
#include <qdrag.h>
#include <qevent.h>
#include <qmimedata.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qthread.h>

OmegaModelGUI::OmegaModelGUI(QWidget *parent)
	: QMainWindow(parent)
{
	setAcceptDrops(true);		// allow files to be dragged into main window

	ui.setupUi(this);

	// connect buttons with slots
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(onClickSerialise()));			// serialise button
	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(onClickClear()));			// quit button
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(onClickQuit()));				// clear list button
	connect(ui.toolButton, SIGNAL(clicked()), this, SLOT(onClickSetDestination()));		// save to destination button
}

// slots
void OmegaModelGUI::onClickClear()
{
	ui.listWidget->clear();
}

void OmegaModelGUI::onClickQuit()
{
	this->close();
}

void OmegaModelGUI::onClickSerialise()
{
	// check that the destination filename has been set
	if (destination.isEmpty()) {

		QMessageBox msgBox;
		msgBox.setText(QString("Unable to convert files! No destination filename selected."));
		msgBox.exec();
	}
	else if (ui.listWidget->count() == 0) {

		QMessageBox msgBox;
		msgBox.setText(QString("Unable to convert files! No source files selected."));
		msgBox.exec();
	}
	else {

		// create progress window
		progressWin = new ProgressWindow(this);
		
		// set destination file
		OMFConverter *p_converter = new OMFConverter();
		p_converter->SetDestinationFilename(destination.toStdString());

		// calculate steps for progress bar based on number of files to convert
		int steps = 100 / ui.listWidget->count();
		int count = 0;

		// import and convert all files that are in the list to OMF format
		for (int c = 0; c < ui.listWidget->count(); ++c) {
			
			QListWidgetItem *listItem = ui.listWidget->item(c);
			QString filename = listItem->text();
			*g_filelog << "serialising row " << c << " of filename " << filename.toStdString() << "\n";

			progressWin->SetProgressText("Importing " + filename + "....");
	
			p_converter->ConvertFileToOMF(filename.toStdString());

			count += steps;
			progressWin->SetProgressValue(count);
		}

		// serialise all imported model files to destination
		progressWin->SetProgressText("Beginning serialisation of models...");
		p_converter->SerialiseModelData();

		progressWin->SetProgressText("Serialisation of files successful.");
		progressWin->SerialistaionComplete();

		delete p_converter;
	}
}

void OmegaModelGUI::onClickSetDestination()
{
	QFileDialog fileDialog(this, tr("Save Destination"), "", tr("All Files (*)"));
	if (fileDialog.exec()) {

		destination = fileDialog.selectedFiles()[0];

		ui.lineEdit->setText(destination);
	}
}

// override functions for file drop
void OmegaModelGUI::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasUrls()) {

		event->acceptProposedAction();
	}
}

void OmegaModelGUI::dragMoveEvent(QDragMoveEvent *event)
{
	event->acceptProposedAction();
}

bool OmegaModelGUI::CheckFileFormat(QString filename) 
{
	if (filename.contains(".obj") || filename.contains(".dae")) {

		return true;
	}
	return false;
}

void OmegaModelGUI::dropEvent(QDropEvent *event)
{
	for (auto& url : event->mimeData()->urls()) {

		// get the filename of the dragged file
		QString filename = url.toLocalFile();

		// ensure that the filename is in a format supported by this program
		if (CheckFileFormat(filename)) {

			// and add it to the list
			QListWidgetItem *item = new QListWidgetItem();
			item->setText(filename);

			int nextRow = ui.listWidget->count() + 1;
			ui.listWidget->insertItem(nextRow, item);

		}
	}
}
