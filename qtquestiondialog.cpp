#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include "qtquestiondialog.h"

#include <cassert>

#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>

#include "questiondialog.h"
#pragma GCC diagnostic pop


ribi::QtQuestionDialog::QtQuestionDialog(
  QWidget *parent)
  : QtHideAndShowDialog(parent)
{

}

std::string ribi::QtQuestionDialog::GetVersion() noexcept
{
  return "1.3";
}

std::vector<std::string> ribi::QtQuestionDialog::GetVersionHistory() noexcept
{
  return {
    "2011-06-30: version 1.0: initial version",
    "2013-10-23: version 1.1: moved m_signal_submitted to QuestionDialog",
    "2013-10-24: version 1.2: moved general m_dialog to derived classes",
    "2014-06-04: version 1.3: added SetDialog member function"
  };
}
