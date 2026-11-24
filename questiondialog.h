#ifndef QUESTIONDIALOG_H
#define QUESTIONDIALOG_H

#include <optional>
#include <string>
#include <vector>

namespace ribi {

struct Question;

///Dialog for (a derived class of) Question
///For an OpenQuestion use an OpenQuestionDialog
///For a MultipleChoiceQuestion use a MultipleChoiceQuestionDialog
struct QuestionDialog
{
  explicit QuestionDialog();

  virtual ~QuestionDialog() noexcept;

  ///Run the dialog from the command line.
  ///If 'show_feedback' is false, the user is not told if the
  ///answer is correct, which is used in a test.
  ///If the user enters an empty line, no answer is submitted,
  ///which means that the user wants to quit
  void Execute(const bool show_feedback = true);

  ///Obtain the question
  virtual const Question& GetQuestion() const noexcept = 0;

  ///Check if an answer has been submitted
  bool HasSubmitted() const noexcept { return m_is_correct.has_value(); }

  ///See if the submitted answer is correct.
  ///Throws std::logic_error if no answer has been submitted yet
  bool IsAnswerCorrect() const;

  ///Try to submit an answer
  ///For an open question, s will be the anwer
  ///For a multiple choice question, s will be the index of the answer
  ///Submit will throw an exception if s is invalid. For example,
  ///if s is a word, where a multiple choice question needs an index (like '2')
  virtual void Submit(const std::string& s) = 0;

  virtual std::string ToStr() const noexcept = 0;

  protected:
  ///Set whether the user has answered the client correct
  void SetIsCorrect(const bool is_correct);

  private:

  ///Was the submitted answer correct?
  ///Is empty if no answer has been submitted yet
  std::optional<bool> m_is_correct;

  std::string AskUserForInput() const noexcept;
};

} //~namespace ribi

#endif // QUESTIONDIALOG_H
