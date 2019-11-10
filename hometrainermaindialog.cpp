#include "hometrainermaindialog.h"

#include <algorithm>
#include <cassert>
#include <ctime>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include <boost/shared_ptr.hpp>

#include "fileio.h"
#include "openquestion.h"
#include "openquestionfactory.h"
#include "openquestiondialog.h"
#include "openquestiondialogfactory.h"
#include "questiondialog.h"
#include "multiplechoicequestion.h"
#include "multiplechoicequestiondialog.h"




ribi::HometrainerMainDialog::HometrainerMainDialog(
  const std::string& filename)
  : HometrainerMainDialog(CreateQuestions(filename))
{

}

ribi::HometrainerMainDialog::HometrainerMainDialog(
  const std::vector<ribi::Question>& questions)
  : m_current_question_index(0),
    m_n_correct(0),
    m_n_incorrect(0),
    m_question_dialog{},
    m_questions(questions),
    m_quit(false)

{
  if (m_questions.empty())
  {
    throw std::logic_error("HometrainerMainDialog: no valid question");
  }

  #ifdef NDEBUG
  //Only randomize in release mode
  std::srand(std::time(0));
  #endif

  std::random_shuffle(m_questions.begin(),m_questions.end());
}

std::unique_ptr<ribi::Question> ribi::CreateQuestion(
  const std::string& s
) noexcept
{
  try
  {
    auto q = std::make_unique<OpenQuestion>(s);
    assert(q);
    return std::move(q);
  }
  catch (std::exception&)
  {
    //Question could not be parsed
  }
  try
  {
    auto q = std::make_unique<MultipleChoiceQuestion>(s);
    assert(q);
    return std::move(q);
  }
  catch (std::exception&)
  {
    //Question could not be parsed
  }
  std::unique_ptr<Question> q;
  assert(!q);
  return q;
}

std::unique_ptr<ribi::QuestionDialog> ribi::CreateQuestionDialog(
  const Question& question) noexcept
{
  //Open q
  try
  {
    auto d = std::make_unique<OpenQuestionDialog>(question.ToStr());
    if (d) return std::move(d);
  }
  catch (const std::logic_error&)
  {
    //OK
  }
  {
    auto d = std::make_unique<MultipleChoiceQuestionDialog>(question.ToStr());
    if (d) return std::move(d);
  }
  assert(false);
  return {};
}

std::vector<boost::shared_ptr<const ribi::Question> >
  ribi::CreateQuestions(
    const std::string& filename)
{
  if (!fileio::FileIo().IsRegularFile(filename))
  {
    throw std::logic_error("HometrainerMainDialog: no valid file");
  }
  const std::vector<std::string> text { fileio::FileIo().FileToVector(filename) };

  std::vector<boost::shared_ptr<const Question> > v;
  for (const std::string& s: text)
  {
    const boost::shared_ptr<const Question> q = CreateQuestion(s);
    //q will be nullptr if no Question could be created
    if (q)
    {
      v.push_back(q);
    }
  }
  return v;
}

void ribi::HometrainerMainDialog::DisplayScore() const noexcept
{
  const int total = m_n_correct + m_n_incorrect;
  const double fraction_correct = static_cast<double>(m_n_correct) / static_cast<double>(total);
  const int percentage_correct = static_cast<int>(100.0 * fraction_correct);

  std::cout
    << "Current score: "
    << m_n_correct
    << " / "
    << total
    << " ("
    << percentage_correct
    << "%)"
    << '\n';
}


void ribi::HometrainerMainDialog::Execute()
{
  assert(!m_questions.empty());
  if (m_questions.empty())
  {
    throw std::logic_error("HometrainerMainDialog: must have questions");
  }

  while (1)
  {
    ///Prepare the current question
    assert(GetCurrentQuestion());
    m_question_dialog = CreateQuestionDialog(GetCurrentQuestion());
    assert(m_question_dialog);

    //Interface with the user about the current question
    //const std::string s = AskUserForInput();
    //Submit(s);
    //DisplayScore();
    m_question_dialog->Execute();

    DisplayScore();

    if (m_quit) return;

    ///Set up a next question
    NewQuestion();
  }
}

boost::shared_ptr<const ribi::Question> ribi::HometrainerMainDialog::GetCurrentQuestion() const noexcept
{
  assert(m_current_question_index < static_cast<int>(GetQuestions().size()));
  const boost::shared_ptr<const ribi::Question> question {
    GetQuestions()[static_cast<size_t>(m_current_question_index)]
  };
  assert(question);
  return question;
}


void ribi::HometrainerMainDialog::NewQuestion()
{
  ++m_current_question_index;
  if (m_current_question_index == static_cast<int>(m_questions.size()))
  {
    std::random_shuffle(m_questions.begin(),m_questions.end());
    m_current_question_index = 0;
  }
  assert(m_current_question_index < static_cast<int>(m_questions.size()));
}

void ribi::HometrainerMainDialog::OnRequestQuit() noexcept
{
  m_quit = true;
}

void ribi::HometrainerMainDialog::OnSubmitted(const bool is_correct) noexcept
{
  if (is_correct) ++m_n_correct; else ++m_n_incorrect;
}

void ribi::HometrainerMainDialog::Submit(const std::string& answer_from_user)
{
  const bool is_correct = this->GetCurrentQuestion()->IsCorrect(answer_from_user);
  if (is_correct) ++m_n_correct; else ++m_n_incorrect;
  NewQuestion();
}

void ribi::TestHometrainerMainDialog() noexcept
{
  for(const std::string& s: OpenQuestionFactory().GetValidOpenQuestionStrings())
  {
    assert(CreateQuestion(s));
    assert(CreateQuestionDialog(CreateQuestion(s)));
  }
  for(const std::string& s: GetValidMultipleChoiceQuestions())
  {
    assert(CreateQuestion(s));
    assert(CreateQuestionDialog(CreateQuestion(s)));
  }
}
