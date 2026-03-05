#include "MainWindow.h"
#include <QApplication>
#include <QFileInfo>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  // Get the executable's filename (without path)
  QString executableName = QFileInfo(argv[0]).baseName();

  MainWindow window(executableName);
  window.show();

  return app.exec();
}
