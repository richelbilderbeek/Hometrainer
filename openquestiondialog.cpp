#include "openquestiondialog.h"

#include <cassert>
#include <sstream>

#include "openquestion.h"
#include "openquestiondialogfactory.h"

ribi::OpenQuestionDialog::OpenQuestionDialog()
  : m_answer_in_progress{},
    m_open_question{}
{

}

const ribi::Question& ribi::OpenQuestionDialog::GetQuestion() const noexcept
{
  return m_open_question;
}

void ribi::OpenQuestionDialog::SetAnswerInProgress(const std::string& answer_in_progress) noexcept
{
  if (m_answer_in_progress != answer_in_progress)
  {
    m_answer_in_progress = answer_in_progress;
  }
}

void ribi::OpenQuestionDialog::SetOpenQuestion(
  const OpenQuestion& open_question) noexcept
{
  m_open_question = open_question;
}


void ribi::OpenQuestionDialog::Submit(const std::string& s)
{
  if (HasSubmitted())
  {
    throw std::logic_error("Cannot submit a second answer");
  }
  assert(!HasSubmitted());

  this->SetIsCorrect(GetQuestion()->IsCorrect(s));


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
