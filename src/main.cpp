#include <QtGui/QApplication>

#include "mainWindow.h"

int main (int argc, char *argv[])
{
  QApplication App(argc, argv);

  MainWindow MainW;
  MainW.show();

  return App.exec();
}
