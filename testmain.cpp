#include <cstdlib>
#include <iostream>

#include <QApplication>
#include <QDir>
#include <QTemporaryDir>

#include "encranger.h"
#include "hometrainerhelper.h"
#include "hometrainermaindialog.h"
#include "hometrainermenudialog.h"
#include "hometrainerprintabletest.h"
#include "hometrainerresources.h"
#include "hometrainertestfile.h"
#include "hometrainertestresults.h"
#include "hometrainertestscorer.h"
#include "multiplechoicequestion.h"
#include "multiplechoicequestiondialog.h"
#include "openquestion.h"
#include "openquestiondialog.h"
#include "openquestiondialogfactory.h"
#include "openquestionfactory.h"
#include "qthometrainercreateprintabletestdialog.h"
#include "qthometrainercreatetestdialog.h"
#include "qthometrainermaindialog.h"
#include "qthometrainermenudialog.h"
#include "qthometrainerscoretestsdialog.h"
#include "qtmultiplechoicequestiondialog.h"
#include "qtopenquestiondialog.h"
#include "question.h"

#ifdef NDEBUG
#error "The tests use assert, so NDEBUG must not be defined"
#endif

int main(int argc, char *argv[])
{
  //The tests do not need a screen
  if (std::getenv("QT_QPA_PLATFORM") == nullptr)
  {
    qputenv("QT_QPA_PLATFORM", "offscreen");
  }
  QApplication a(argc, argv);

  //Run the tests in a temporary folder, as these create files
  const QTemporaryDir folder;
  QDir::setCurrent(folder.path());

  using namespace ribi;
  TestHometrainerHelper();
  TestEncranger();
  TestOpenQuestion();
  TestOpenQuestionFactory();
  TestMultipleChoiceQuestion();
  TestQuestion();
  TestOpenQuestionDialog();
  TestOpenQuestionDialogFactory();
  TestMultipleChoiceQuestionDialog();
  TestHometrainerTestFile();
  TestHometrainerTestResults();
  TestHometrainerTestScorer();
  TestHometrainerPrintableTest();
  TestHometrainerMainDialog();
  TestHometrainerResources();
  TestHometrainerMenuDialog();

  TestQtOpenQuestionDialog();
  TestQtMultipleChoiceQuestionDialog();
  TestQtHometrainerMainDialog();
  TestQtHometrainerMenuDialog();
  TestQtHometrainerCreateTestDialog();
  TestQtHometrainerScoreTestsDialog();
  TestQtHometrainerCreatePrintableTestDialog();

  std::cout << "All tests passed\n";
  return 0;
}
