#include <QApplication>
#include <QDir>

#include "hometrainerresources.h"
#include "qthometrainermenudialog.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  ribi::QtHometrainerMenuDialog w;
#ifdef Q_OS_WASM
  //In a web browser, files are kept in memory.
  //Put the example exercises there, so they can be used directly
  QDir().mkpath("/hometrainer");
  QDir::setCurrent("/hometrainer");
  const ribi::HometrainerResources resources;

  //The web page is the window: no title bar, top-left of the canvas
  w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
  w.move(0, 0);
#endif
  w.show();
  return a.exec();
}
