#pragma GCC diagnostic push

#include <QApplication>
#include "qthometrainermenudialog.h"

#pragma GCC diagnostic pop

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  ribi::QtHometrainerMenuDialog w;
  w.show();
  return a.exec();
}

