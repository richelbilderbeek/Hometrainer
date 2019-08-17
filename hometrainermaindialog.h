#ifndef HOMETRAINERMAINDIALOG_H
#define HOMETRAINERMAINDIALOG_H

#include <string>
#include <vector>

#include "question.h"
#include "questiondialog.h"
#include "multiplechoicequestiondialog.h"
#include "openquestiondialog.h"

namespace ribi {

///HometrainerMainDialog loads a collection of questions from file.
///From then on, it selects questions and keeps track of the score.
///The user comminication is done via a QuestionDialog
struct HometrainerMainDialog
{
  ///Load questions from file
  HometrainerMainDialog(const std::string& filename);

  HometrainerMainDialog(const std::vector<ribi::Question>& questions);

  ///Start the command-line version
  void Execute();

  const Question& GetCurrentQuestion() const noexcept;

  int GetNumberCorrect() const noexcept { return m_n_correct; }
  int GetNumberIncorrect() const noexcept { return m_n_incorrect; }

  ///Get the parsed questions
  const std::vector<Question>& GetQuestions() const noexcept
  {
    return m_questions;
  }

  void Submit(const std::string& answer_from_user);

  private:
  ///The current question its index
  int m_current_question_index;

  ///Number of correct answers
  int m_n_correct;

  ///Number of incorrect answers
  int m_n_incorrect;

  ///The original polymorphic type
  std::unique_ptr<QuestionDialog> m_question_dialog;

  ///The questions loaded
  std::vector<Question> m_questions;

  ///Does the user want to quit?
  bool m_quit;

  void DisplayScore() const noexcept;

  ///Create a new question
  void NewQuestion();

  ///Respond to the user requesting to quit
  void OnRequestQuit() noexcept;

  ///Respond to the user submitting an answer
  void OnSubmitted(const bool is_correct) noexcept;

};

///Create a Question from a std::string
///Returns nullptr if the string cannot be converted to a question
std::unique_ptr<Question> CreateQuestion(const std::string& s) noexcept;

std::vector<Question> CreateQuestions(
  const std::string& filename
);

///Build the correct dialog for a (derived class of) question
std::unique_ptr<QuestionDialog> CreateQuestionDialog(
  const Question& question
) noexcept;

void TestHometrainerMainDialog() noexcept;

} //namespace ribi

#endif // HOMETRAINERMAINDIALOG_H
