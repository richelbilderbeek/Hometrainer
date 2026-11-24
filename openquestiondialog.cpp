#include "openquestiondialog.h"

#include <cassert>
#include <stdexcept>

#include "openquestion.h"
#include "openquestiondialogfactory.h"

ribi::OpenQuestionDialog::OpenQuestionDialog(const OpenQuestion& open_question)
  : m_answer_in_progress{},
    m_open_question{open_question}
{

}

const ribi::Question& ribi::OpenQuestionDialog::GetQuestion() const noexcept
{
  return m_open_question;
}

void ribi::OpenQuestionDialog::SetAnswerInProgress(const std::string& answer_in_progress) noexcept
{
  m_answer_in_progress = answer_in_progress;
}

void ribi::OpenQuestionDialog::Submit(const std::string& s)
{
  if (HasSubmitted())
  {
    throw std::logic_error("Cannot submit a second answer");
  }
  this->SetIsCorrect(GetQuestion().IsCorrect(s));
  assert(HasSubmitted());
}

void ribi::TestOpenQuestionDialog() noexcept
{
  //Test setting the open questions
  for(const auto& dialog: OpenQuestionDialogFactory().GetTestOpenQuestionDialogs())
  {
    assert(!dialog.HasSubmitted());
  }
  //Submit a correct answer
  {
    auto d = OpenQuestionDialogFactory().Create("-,1+1=,2/two");
    d.Submit("two");
    assert(d.HasSubmitted());
    assert(d.IsAnswerCorrect());
  }
  //Submit an incorrect answer
  {
    auto d = OpenQuestionDialogFactory().Create("-,1+1=,2/two");
    d.Submit("3");
    assert(d.HasSubmitted());
    assert(!d.IsAnswerCorrect());
  }
  //Cannot submit twice
  {
    auto d = OpenQuestionDialogFactory().Create("-,1+1=,2/two");
    d.Submit("2");
    try
    {
      d.Submit("2");
      assert(!"Should not get here");
    }
    catch (std::logic_error&)
    {
      //OK
    }
  }
}

std::string ribi::OpenQuestionDialog::ToStr() const noexcept
{
  return m_open_question.ToStr();
}
