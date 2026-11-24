#include "hometrainermaindialog.h"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "hometrainerhelper.h"
#include "multiplechoicequestion.h"
#include "multiplechoicequestiondialog.h"
#include "openquestion.h"
#include "openquestiondialog.h"
#include "openquestionfactory.h"
#include "questiondialog.h"

ribi::HometrainerMainDialog::HometrainerMainDialog(
  const std::string& filename)
  : HometrainerMainDialog(CreateQuestions(filename))
{

}

ribi::HometrainerMainDialog::HometrainerMainDialog(
  const std::vector<std::shared_ptr<const Question>>& questions,
  const std::optional<HometrainerTestSession>& test_session)
  : m_current_question_index(0),
    m_n_correct(0),
    m_n_incorrect(0),
    m_questions(questions),
    m_test_results{},
    m_test_session{test_session}
{
  if (m_questions.empty())
  {
    throw std::logic_error("HometrainerMainDialog: no valid question");
  }
  if (m_test_session)
  {
    m_test_results = HometrainerTestResults(m_test_session->m_username);
    //Save the empty results directly, so that it is known the student started
    SaveTestResults();
  }
  std::shuffle(m_questions.begin(),m_questions.end(), GetRng());
}

std::unique_ptr<ribi::QuestionDialog> ribi::CreateQuestionDialog(
  const Question& question)
{
  if (const auto q = dynamic_cast<const OpenQuestion*>(&question))
  {
    return std::make_unique<OpenQuestionDialog>(*q);
  }
  if (const auto q = dynamic_cast<const MultipleChoiceQuestion*>(&question))
  {
    return std::make_unique<MultipleChoiceQuestionDialog>(*q);
  }
  throw std::logic_error("CreateQuestionDialog: unknown question type");
}

void ribi::HometrainerMainDialog::Execute()
{
  while (!IsDone())
  {
    const auto dialog = CreateQuestionDialog(GetCurrentQuestion());
    dialog->Execute(!IsTest());
    if (!dialog->HasSubmitted())
    {
      //The user wants to quit
      return;
    }
    RegisterAnswer(dialog->IsAnswerCorrect());
    std::cout << GetScoreText() << '\n';
  }
  if (IsTest())
  {
    std::cout << "Test done. Results are saved to '"
      << m_test_session->m_results_filename << "'\n";
  }
}

const ribi::Question& ribi::HometrainerMainDialog::GetCurrentQuestion() const noexcept
{
  assert(m_current_question_index >= 0);
  assert(m_current_question_index < static_cast<int>(GetQuestions().size()));
  return *GetQuestions()[static_cast<std::size_t>(m_current_question_index)];
}

std::string ribi::HometrainerMainDialog::GetScoreText() const noexcept
{
  std::stringstream s;
  const int n_answered = m_n_correct + m_n_incorrect;
  if (IsTest())
  {
    s << "Answered " << n_answered << " of " << GetNumberOfQuestions() << " questions.";
    return s.str();
  }
  const int percentage = n_answered == 0 ? 0 : (100 * m_n_correct) / n_answered;
  s << "Correct: " << m_n_correct
    << ", incorrect: " << m_n_incorrect
    << ", score: " << percentage << "%";
  return s.str();
}

bool ribi::HometrainerMainDialog::IsDone() const noexcept
{
  return IsTest() && m_n_correct + m_n_incorrect == GetNumberOfQuestions();
}

void ribi::HometrainerMainDialog::NewQuestion()
{
  ++m_current_question_index;
  if (m_current_question_index == static_cast<int>(m_questions.size()))
  {
    //In a test, stay at the last question
    if (IsTest())
    {
      --m_current_question_index;
      return;
    }
    std::shuffle(m_questions.begin(),m_questions.end(), GetRng());
    m_current_question_index = 0;
  }
  assert(m_current_question_index < static_cast<int>(m_questions.size()));
}

void ribi::HometrainerMainDialog::RegisterAnswer(const bool is_correct)
{
  if (IsDone())
  {
    throw std::logic_error("Cannot answer questions when the test is done");
  }
  if (is_correct) ++m_n_correct; else ++m_n_incorrect;
  if (m_test_results)
  {
    m_test_results->Add(GetCurrentQuestion(), is_correct);
    SaveTestResults();
  }
  NewQuestion();
}

void ribi::HometrainerMainDialog::SaveTestResults() const
{
  assert(m_test_results);
  assert(m_test_session);
  SaveContainer(
    m_test_results->ToEncryptedLines(m_test_session->m_key),
    m_test_session->m_results_filename
  );
}

void ribi::HometrainerMainDialog::Submit(const std::string& answer_from_user)
{
  RegisterAnswer(GetCurrentQuestion().IsCorrect(answer_from_user));
}

void ribi::TestHometrainerMainDialog() noexcept
{
  std::vector<std::shared_ptr<const Question>> questions;
  for(const std::string& s: OpenQuestionFactory().GetValidOpenQuestionStrings())
  {
    const std::shared_ptr<const Question> q = CreateQuestion(s);
    assert(q);
    assert(CreateQuestionDialog(*q));
    questions.push_back(q);
  }
  for(const std::string& s: GetValidMultipleChoiceQuestions())
  {
    const std::shared_ptr<const Question> q = CreateQuestion(s);
    assert(q);
    assert(CreateQuestionDialog(*q));
    questions.push_back(q);
  }
  //An exercise is never done
  {
    HometrainerMainDialog d(questions);
    assert(!d.IsTest());
    for (int i = 0; i != 3 * d.GetNumberOfQuestions(); ++i)
    {
      d.RegisterAnswer(i % 2 == 0);
      assert(!d.IsDone());
    }
    assert(d.GetNumberCorrect() + d.GetNumberIncorrect() == 3 * d.GetNumberOfQuestions());
  }
  //A test is done after all questions are answered
  {
    const std::string filename = "hometrainer_test_results_tmp.txt";
    HometrainerTestSession session;
    session.m_username = "John Doe";
    session.m_key = 42;
    session.m_results_filename = filename;
    HometrainerMainDialog d(questions, session);
    assert(d.IsTest());
    assert(IsRegularFile(filename));
    for (int i = 0; i != d.GetNumberOfQuestions(); ++i)
    {
      assert(!d.IsDone());
      d.Submit(d.GetCurrentQuestion().GetCorrectAnswers()[0]);
    }
    assert(d.IsDone());
    assert(d.GetNumberCorrect() == d.GetNumberOfQuestions());
    const auto lines = DecryptTestResults(FileToVector(filename), 42);
    assert(static_cast<int>(lines.size()) == d.GetNumberOfQuestions() + 1);
    std::remove(filename.c_str());
  }
}
