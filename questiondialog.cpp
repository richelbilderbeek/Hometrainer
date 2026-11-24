#include "questiondialog.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

#include "question.h"

ribi::QuestionDialog::QuestionDialog()
  : m_is_correct{}
{
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

void ribi::QuestionDialog::Execute(const bool show_feedback)
{
  assert(!this->HasSubmitted());

  while (!this->HasSubmitted())
  {
    std::cout << GetQuestion() << '\n';
    std::cout << "Please enter your answer: " << std::endl;

    try
    {
      const std::string s = AskUserForInput();
      if (s.empty())
      {
        //An empty answer means the user wants to quit
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
  if (!show_feedback) return;

  if (this->IsAnswerCorrect())
  {
    std::cout << "Correct\n";
  }
  else
  {
    const std::vector<std::string>& correct_answers = GetQuestion().GetCorrectAnswers();
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

bool ribi::QuestionDialog::IsAnswerCorrect() const
{
  if (!HasSubmitted())
  {
    throw std::logic_error("Can only check if answer is correct, after submitting an answer");
  }
  return *m_is_correct;
}

void ribi::QuestionDialog::SetIsCorrect(const bool is_correct)
{
  if (HasSubmitted())
  {
    throw std::logic_error("Cannot submit a second answer");
  }
  m_is_correct = is_correct;
  assert(HasSubmitted());
  assert(IsAnswerCorrect() == is_correct);
}
