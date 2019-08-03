



#include "openquestiondialog.h"

#include <cassert>
#include <sstream>

#include "openquestion.h"
#include "openquestiondialogfactory.h"




ribi::OpenQuestionDialog::OpenQuestionDialog()
  : m_signal_open_question_changed{},
    m_answer_in_progress{},
    m_open_question{}
{

}

boost::shared_ptr<const ribi::Question> ribi::OpenQuestionDialog::GetQuestion() const noexcept
{
  return m_open_question;
}

std::string ribi::OpenQuestionDialog::GetVersion() noexcept
{
  return "1.3";
}

std::vector<std::string> ribi::OpenQuestionDialog::GetVersionHistory() noexcept
{
  return {
    "2011-06-29: version 1.0: initial version",
    "2013-10-24: version 1.1: added testing",
    "2014-06-05: version 1.2: moved some code to OpenQuestionDialogFactory",
    "2014-06-12: version 1.3: added support for displaying an answer-in-progress"
  };
}

void ribi::OpenQuestionDialog::SetAnswerInProgress(const std::string& answer_in_progress) noexcept
{
  if (m_answer_in_progress != answer_in_progress)
  {
    m_answer_in_progress = answer_in_progress;
    m_signal_open_question_changed(this);
  }
}

void ribi::OpenQuestionDialog::SetOpenQuestion(const boost::shared_ptr<OpenQuestion>& open_question) noexcept
{
  if (m_open_question != open_question)
  {
    m_open_question = open_question;
    m_signal_open_question_changed(this);
  }
}


void ribi::OpenQuestionDialog::Submit(const std::string& s)
{
  if (HasSubmitted())
  {
    throw std::logic_error("Cannot submit a second answer");
  }
  assert(!HasSubmitted());

  this->SetIsCorrect(GetQuestion()->IsCorrect(s));
  //this->m_signal_submitted(this);
}

void ribi::TestOpenQuestionDialog() noexcept
{
  //Test setting the open questions
  for(const auto dialog: OpenQuestionDialogFactory().GetTestOpenQuestionDialogs())
  {
    assert(!dialog->HasSubmitted());
  }
}

std::string ribi::OpenQuestionDialog::ToStr() const noexcept
{
  std::stringstream s;
  s << m_open_question->ToStr();
  return s.str();
}
