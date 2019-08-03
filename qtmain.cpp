

#include <QApplication>
#include "qthometrainermenudialog.h"



int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  ribi::QtHometrainerMenuDialog w;
  w.show();
  return a.exec();
}

