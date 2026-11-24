#include "multiplechoicequestiondialog.h"

#include <algorithm>
#include <cassert>
#include <stdexcept>

#include "multiplechoicequestion.h"
#include "question.h"

ribi::MultipleChoiceQuestionDialog::MultipleChoiceQuestionDialog(
  const MultipleChoiceQuestion& question)
  : QuestionDialog{},
    m_answer_in_progress{-1},
    m_question(question)
{
  assert(!HasSubmitted());
}

ribi::MultipleChoiceQuestionDialog::MultipleChoiceQuestionDialog(const std::string& question)
  : QuestionDialog{},
    m_answer_in_progress{-1},
    m_question{question}
{
  assert(!HasSubmitted());
}

ribi::MultipleChoiceQuestionDialog::~MultipleChoiceQuestionDialog()
{

}

const ribi::Question& ribi::MultipleChoiceQuestionDialog::GetQuestion() const noexcept
{
  return m_question;
}

void ribi::MultipleChoiceQuestionDialog::SetAnswerInProgress(const int index) noexcept
{
  m_answer_in_progress = index;
}

void ribi::MultipleChoiceQuestionDialog::Submit(const std::string& s)
{
  if (HasSubmitted())
  {
    throw std::logic_error("Cannot submit a second answer");
  }
  int index = -1;
  try
  {
    std::size_t n_chars_read = 0;
    index = std::stoi(s, &n_chars_read);
    if (n_chars_read != s.size()) throw std::invalid_argument("Trailing characters");
  }
  catch (const std::exception&)
  {
    throw std::logic_error("Must submit an index to a multiple choice question dialog");
  }
  const auto sz = static_cast<int>(GetMultipleChoiceQuestion().GetOptions().size());
  if (index < 0 || index >= sz)
  {
    throw std::logic_error("Must submit an existing index to a multiple choice question dialog");
  }
  //The real (that is non-index) answer
  const std::string& t = GetMultipleChoiceQuestion().GetOptions()[static_cast<std::size_t>(index)];
  this->SetIsCorrect(GetQuestion().IsCorrect(t));
}

void ribi::TestMultipleChoiceQuestionDialog() noexcept
{
  //Test setting the multiple choice questions
  for(const std::string& s: GetValidMultipleChoiceQuestions())
  {
    const MultipleChoiceQuestion q(s);
    const MultipleChoiceQuestionDialog d(q);
    assert(!d.HasSubmitted());
  }
  //Test submitting correct and incorrect answers to this dialog
  for (const std::string& s: GetValidMultipleChoiceQuestions())
  {
    const MultipleChoiceQuestion question(s);
    const std::vector<std::string>& options = question.GetOptions();

    //Submit correct answer to this dialog
    {
      MultipleChoiceQuestionDialog dialog(question);
      const auto index = std::distance(options.begin(),std::find(options.begin(),options.end(),question.GetAnswer()));
      assert(index < static_cast<int>(options.size()));
      dialog.Submit(std::to_string(index));
      assert(dialog.HasSubmitted());
      assert(dialog.IsAnswerCorrect());
    }
    //Submit incorrect answer to this dialog
    {
      MultipleChoiceQuestionDialog dialog(question);
      const std::string wrong_answer = question.GetWrongAnswers().at(0);
      if (wrong_answer == question.GetAnswer()) continue;
      const auto index = std::distance(options.begin(),std::find(options.begin(),options.end(),wrong_answer));
      assert(index < static_cast<int>(options.size()));
      dialog.Submit(std::to_string(index));
      assert(dialog.HasSubmitted());
      assert(!dialog.IsAnswerCorrect());
    }
  }
  //Invalid submissions
  {
    MultipleChoiceQuestionDialog dialog("-,1+1=,2,3");
    for (const std::string s: { "", "two", "-1", "2", "1x" })
    {
      try
      {
        dialog.Submit(s);
        assert(!"Should not get here");
      }
      catch (std::logic_error&)
      {
        //OK
      }
    }
    assert(!dialog.HasSubmitted());
  }
}

std::string ribi::MultipleChoiceQuestionDialog::ToStr() const noexcept
{
  return m_question.ToStr();
}
