#ifndef HOMETRAINERMAINDIALOG_H
#define HOMETRAINERMAINDIALOG_H

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "hometrainertestresults.h"
#include "question.h"
#include "questiondialog.h"

namespace ribi {

///The settings of a student doing a test
struct HometrainerTestSession
{
  ///The full name of the student
  std::string m_username;

  ///The key the test is encrypted with
  int m_key;

  ///The file the results are saved to
  std::string m_results_filename;
};

///HometrainerMainDialog loads a collection of questions from file.
///From then on, it selects questions and keeps track of the score.
///
///In an exercise, the questions are asked in a random order, over and over,
///and the user gets feedback on each answer.
///
///In a test, each question is asked once and the user gets no feedback.
///After each answer, the encrypted results are saved to file.
///
///The user comminication is done via a QuestionDialog
struct HometrainerMainDialog
{
  ///Load an exercise from file.
  ///Throws std::runtime_error if the file does not exist
  ///Throws std::logic_error if there are no valid questions
  explicit HometrainerMainDialog(const std::string& filename);

  ///Throws std::logic_error if there are no questions
  explicit HometrainerMainDialog(
    const std::vector<std::shared_ptr<const Question>>& questions,
    const std::optional<HometrainerTestSession>& test_session = std::nullopt
  );

  ///Start the command-line version
  void Execute();

  ///Obtain the question to be answered now
  const Question& GetCurrentQuestion() const noexcept;

  int GetNumberCorrect() const noexcept { return m_n_correct; }
  int GetNumberIncorrect() const noexcept { return m_n_incorrect; }
  int GetNumberOfQuestions() const noexcept { return static_cast<int>(m_questions.size()); }

  ///Get the parsed questions
  const std::vector<std::shared_ptr<const Question>>& GetQuestions() const noexcept { return m_questions; }

  ///Obtain a text that describes the score so far
  std::string GetScoreText() const noexcept;

  ///Obtain the test session, if this is a test
  const std::optional<HometrainerTestSession>& GetTestSession() const noexcept { return m_test_session; }

  ///Is the test done? An exercise is never done
  bool IsDone() const noexcept;

  ///Is this a test (instead of an exercise)?
  bool IsTest() const noexcept { return m_test_session.has_value(); }

  ///Register that the current question is answered (in)correctly,
  ///then go to the next question.
  ///In a test, the results are saved to file
  ///Throws std::logic_error if the test is done
  void RegisterAnswer(const bool is_correct);

  ///Submit an answer to the current question, then go to the next question
  void Submit(const std::string& answer_from_user);

  private:
  ///The current question its index
  int m_current_question_index;

  ///Number of correct answers
  int m_n_correct;

  ///Number of incorrect answers
  int m_n_incorrect;

  ///The questions loaded
  std::vector<std::shared_ptr<const Question>> m_questions;

  ///The results of the test, if this is a test
  std::optional<HometrainerTestResults> m_test_results;

  ///The test settings, if this is a test
  std::optional<HometrainerTestSession> m_test_session;

  ///Go to the next question
  void NewQuestion();

  ///Save the test results to file
  void SaveTestResults() const;
};

///Build the correct dialog for a (derived class of) question
std::unique_ptr<QuestionDialog> CreateQuestionDialog(
  const Question& question
);

void TestHometrainerMainDialog() noexcept;

} //namespace ribi

#endif // HOMETRAINERMAINDIALOG_H
