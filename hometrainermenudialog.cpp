#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "hometrainermenudialog.h"

#include <iostream>

#include "container.h"
#include "exercise.h"
#include "fileio.h"
#include "hometrainermaindialog.h"
#include "multiplechoicequestion.h"
#include "multiplechoicequestiondialog.h"
#include "openquestion.h"
#include "openquestiondialog.h"
#include "hometrainerresources.h"
#include "question.h"
#include "questiondialog.h"

#include <QFile>

#pragma GCC diagnostic pop

ribi::HometrainerMenuDialog::HometrainerMenuDialog()
{

}

void ribi::HometrainerMenuDialog::CreateExamples() noexcept
{
  HometrainerResources();
}

int ribi::HometrainerMenuDialog::ExecuteSpecific(const std::vector<std::string>& argv) noexcept
{
  const int argc = static_cast<int>(argv.size());
  if (argc == 1)
  {
    std::cout <<  GetHelp() << '\n';
    return 1;
  }
  assert(argc >= 2);
  const std::string arg = argv[1];
  if (arg == "-e" || arg == "--example")
  {
    CreateExamples();
    return 0;
  }
  if (fileio::FileIo().IsRegularFile(arg))
  {
    HometrainerMainDialog d(arg);
    d.Execute();
    return 0;
  }
  if ( (arg == "-f" || arg == "--filename")
    && argc >= 3 && fileio::FileIo().IsRegularFile(argv[2]))
  {
    HometrainerMainDialog d(argv[2]);
    d.Execute();
    return 0;
  }
  std::cout <<  GetHelp() << '\n';
  return 1;
}

ribi::About ribi::HometrainerMenuDialog::GetAbout() const noexcept
{
  About a(
    "Richel Bilderbeek",
    "Hometrainer",
    "exercise and survey suite",
    "the 23rd of October 2013",
    "2009-2015",
    "http://www.richelbilderbeek.nl/ToolHometrainer.htm",
    GetVersion(),
    GetVersionHistory());
  a.AddLibrary("Exercise version: " + Exercise::GetVersion());
  a.AddLibrary("FileIo version: " + ribi::fileio::FileIo().GetVersion());
  a.AddLibrary("Hometrainer version: " + ribi::HometrainerMenuDialog::GetVersion());
  a.AddLibrary("MultipleChoiceQuestion version: " + MultipleChoiceQuestion::GetVersion());
  a.AddLibrary("MultipleChoiceQuestionDialog version: " + MultipleChoiceQuestionDialog::GetVersion());
  a.AddLibrary("OpenQuestion version: " + OpenQuestion::GetVersion());
  a.AddLibrary("OpenQuestionDialog version: " + OpenQuestionDialog::GetVersion());
  a.AddLibrary("Question version: " + Question::GetVersion());
  a.AddLibrary("QuestionDialog version: " + QuestionDialog::GetVersion());
  return a;
}

ribi::Help ribi::HometrainerMenuDialog::GetHelp() const noexcept
{
  return ribi::Help(
    GetAbout().GetFileTitle(),
    GetAbout().GetFileDescription(),
    {
      Help::Option('e',"example","create example exercises"),
      Help::Option('f',"file","filename of Hometrainer exercise")
    },
    {
      "Hometrainer --about",
      "Hometrainer -f my_exercise.txt",
      "Hometrainer -my_exercise.txt"
    }
  );
}

std::string ribi::HometrainerMenuDialog::GetVersion() const noexcept
{
  return "3.0";
}

std::vector<std::string> ribi::HometrainerMenuDialog::GetVersionHistory() const noexcept
{
  return {
    "2009-xx-xx: Version 0.9: initial version (called HomeTrainer)",
    "2009-04-28: Version 1.0: renamed 'HomeTrainer' to 'Hometrainer 2'",
    "2009-05-09: Version 1.1",
    "2009-08-23: Version 1.2",
    "2009-08-29: Version 1.3",
    "2009-09-20: Version 1.4",
    "2009-10-29: Version 1.5",
    "2009-11-03: Version 1.6",
    "2010-01-07: Version 1.7",
    "2010-01-28: Version 1.8",
    "2011-10-16: Version 2.0: initial website version, renamed 'Hometrainer 2' to 'Hometrainer'"
    "2013-10-23: Version 3.0: major architectural rewrite, added console version"
  };
}

void ribi::TestHometrainerMenuDialog() noexcept
{
  HometrainerMenuDialog().Execute( { "Hometrainer", "-e" } );
  const HometrainerMainDialog d(HometrainerResources().GetExerciseClouds());
  assert(d.GetNumberCorrect() == 0);
}
