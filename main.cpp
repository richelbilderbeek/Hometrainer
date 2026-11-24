#include <QCoreApplication>

#include "hometrainermenudialog.h"

int main(int argc, char * argv[])
{
  //Needed to load images in all formats
  QCoreApplication a(argc, argv);
  const std::vector<std::string> args { ribi::ConvertArguments(argc,argv) };
  return ribi::HometrainerMenuDialog().Execute(args);
}
