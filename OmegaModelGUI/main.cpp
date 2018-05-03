#include "OmegaModelGUI.h"
#include "file_log.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	//Initialise file logging for debugging purposes
	g_filelog = new FileLog("editor-debug", (int)FileLogFlags::FILELOG_WRITE_DATE_INIT | (int)FileLogFlags::FILELOG_CLOSE_AFTER_EACH_WRITE);
	
	QApplication editorApp(argc, argv);
	OmegaModelGUI window;

	window.show();
	return editorApp.exec();
}
