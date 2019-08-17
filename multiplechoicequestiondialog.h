#ifndef MULTIPLECHOICEQUESTIONDIALOG_H
#define MULTIPLECHOICEQUESTIONDIALOG_H

#include "questiondialog.h"
#include "multiplechoicequestion.h"

namespace ribi {

struct MultipleChoiceQuestion;
struct Question;

struct MultipleChoiceQuestionDialog final : public QuestionDialog
{
  explicit MultipleChoiceQuestionDialog(const std::string& question);

  ///Will work if question is not nullptr
  explicit MultipleChoiceQuestionDialog(const MultipleChoiceQuestion& question);

  virtual ~MultipleChoiceQuestionDialog() override;

  ///The answer the user has typed in so far
  int GetAnswerInProgress() const noexcept { return m_answer_in_progress; }

  const auto& GetMultipleChoiceQuestion() const noexcept { return m_question; }

  const Question& GetQuestion() const noexcept override;

  ///The answer the user has selected
  ///Used for synching between multiple QtMultipleChoiceQuestionDialogs displaying
  ///the same MultipleChoiceQuestionDialog
  void SetAnswerInProgress(const int index) noexcept;

  ///Submit an answer
  ///For a multiple choice question, s will be the index of the answer
  void Submit(const std::string& s) override;

  std::string ToStr() const noexcept override;

  private:
  int m_answer_in_progress;

  const MultipleChoiceQuestion m_question;

  ///Create a default Question
  static std::shared_ptr<MultipleChoiceQuestion> CreateDefaultQuestion() noexcept;

};

void TestMultipleChoiceQuestionDialog() noexcept;

} //~namespace ribi

#endif // MULTIPLECHOICEQUESTIONDIALOG_H
