#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "questiondialog.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

#include "openquestion.h"
#include "multiplechoicequestion.h"
#include "question.h"


#pragma GCC diagnostic pop

ribi::QuestionDialog::QuestionDialog()
  : m_signal_request_quit{},
    m_signal_submitted{},
    m_is_correct(Tribool::Indeterminate)
{
  assert(m_is_correct == Tribool::Indeterminate && "Answer is indeterminate at construction");
  assert(!HasSubmitted());
}

ribi::QuestionDialog::~QuestionDialog() noexcept
{

}

std::string ribi::QuestionDialog::AskUserForInput() const noexcept
{
  std::string t;
  std::getline(std::cin,t);
  return t;
}

//std::vector<std::string> ribi::QuestionDialog::GetCorrectAnswers() const noexcept
//{
// return this->GetQuestion()->GetAnswers();
//}

void ribi::QuestionDialog::Execute()
{
  assert(!this->HasSubmitted());

  while (!this->HasSubmitted())
  {
    assert(this->GetQuestion());
    std::cout
      << (*this->GetQuestion()) << '\n';

    std::cout << "Please enter your answer: " << std::endl;

    try
    {
      const std::string s = AskUserForInput();
      if (s.empty())
      {
        m_signal_request_quit();
        return;
      }

      this->Submit(s);
      assert(this->HasSubmitted());
    }
    catch (std::logic_error& e)
    {
      std::cout
        << "Invalid input: " << e.what() << '\n'
        << "Please try again or press enter to quit\n"
        << '\n';
    }
  }

  std::cout << std::endl;

  if (this->IsAnswerCorrect())
  {
    std::cout << "Correct\n";
  }
  else
  {
    const std::vector<std::string> correct_answers { this->GetQuestion()->GetCorrectAnswers() };
    std::cout
      << "Incorrect, "
      << (correct_answers.size() == 1 ? "the correct answer is: " : "the correct answers are: ")
      << '\n';
    for (const std::string& s: correct_answers)
    {
      std::cout << "  " << s << '\n';
    }
  }
  std::cout << std::endl;
}

std::string ribi::QuestionDialog::GetVersion() noexcept
{
  return "1.3";
}

std::vector<std::string> ribi::QuestionDialog::GetVersionHistory() noexcept
{
  return {
    "2011-06-29: version 1.0: initial version",
    "2013-09-26: version 1.1: improved const-correctness, added noexcept",
    "2013-10-24: version 1.2: added Execute for console application",
    "2013-10-25: version 1.3: added testing to derived classes"
  };
}

bool ribi::QuestionDialog::IsAnswerCorrect() const
{
  if (!HasSubmitted())
  {
    throw std::logic_error("Cannot only check if answer is correct, after submitting an answer");
  }
  assert(HasSubmitted());
  return m_is_correct == Tribool::True;
}

void ribi::QuestionDialog::SetIsCorrect(const bool is_correct)
{
  assert(!HasSubmitted() && "Can only answer exactly once");
  m_is_correct = is_correct ? Tribool::True : Tribool::False;

  assert(HasSubmitted());
  assert(IsAnswerCorrect() == is_correct);

  m_signal_submitted(is_correct);
}


