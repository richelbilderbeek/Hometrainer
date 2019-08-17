#ifndef QUESTIONDIALOG_H
#define QUESTIONDIALOG_H

#include <vector>
#include <memory>

#include "tribool.h"

namespace ribi {

struct Question;

///Dialog for (a derived class of) Question
///For an OpenQuestion use an OpenQuestionDialog
///For a MultipleChoiceQuestion use a MultipleChoiceQuestionDialog
struct QuestionDialog
{
  explicit QuestionDialog();

  virtual ~QuestionDialog() noexcept;

  ///Run the dialog from the command line
  void Execute();

  Tribool GetIsCorrect() const noexcept { return m_is_correct; }

  ///Obtain the question
  virtual const Question& GetQuestion() const = 0;

  ///Check if an answer has been submitted
  bool HasSubmitted() const { return m_is_correct != Tribool::Indeterminate; }

  ///See if the submitted answer is correct
  bool IsAnswerCorrect() const;

  ///Try to submit an answer
  ///For an open question, s will be the anwer
  ///For a multiple choice question, s will be the index of the answer
  ///Submit will throw an exception if s is invalid. For example,
  ///if s is a word, where a multiple choice question needs an index (like '2')
  virtual void Submit(const std::string& s) = 0;

  virtual std::string ToStr() const noexcept = 0;

  ///Set whether the user has answered the client correct
  void SetIsCorrect(const bool is_correct);

  private:

  ///Was the submitted answer correct?
  ///Emulates a bool*:
  ///m_is_correct.empty() -> nullptr -> indeterminate
  ///m_is_correct[0] == 0 -> false
  ///m_is_correct[0] == 1 -> true
  ///Other values and sizes are invalid
  Tribool m_is_correct;

  std::string AskUserForInput() const noexcept;
};

} //~namespace ribi

#endif // OPENQUESTIONDIALOG_H
