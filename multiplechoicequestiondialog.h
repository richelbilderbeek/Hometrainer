#ifndef MULTIPLECHOICEQUESTIONDIALOG_H
#define MULTIPLECHOICEQUESTIONDIALOG_H

#include "questiondialog.h"
#include "multiplechoicequestion.h"

namespace ribi {

struct MultipleChoiceQuestionDialog final : public QuestionDialog
{
  ///Throws std::logic_error if the std::string is no multiple choice question
  explicit MultipleChoiceQuestionDialog(const std::string& question);

  explicit MultipleChoiceQuestionDialog(const MultipleChoiceQuestion& question);

  virtual ~MultipleChoiceQuestionDialog() override;

  ///The index of the answer the user has selected so far,
  ///is -1 if nothing has been selected yet
  int GetAnswerInProgress() const noexcept { return m_answer_in_progress; }

  const MultipleChoiceQuestion& GetMultipleChoiceQuestion() const noexcept { return m_question; }

  const Question& GetQuestion() const noexcept override;

  ///The index of the answer the user has selected
  void SetAnswerInProgress(const int index) noexcept;

  ///Submit an answer
  ///For a multiple choice question, s will be the index of the answer
  void Submit(const std::string& s) override;

  std::string ToStr() const noexcept override;

  private:
  int m_answer_in_progress;

  MultipleChoiceQuestion m_question;
};

void TestMultipleChoiceQuestionDialog() noexcept;

} //~namespace ribi

#endif // MULTIPLECHOICEQUESTIONDIALOG_H
