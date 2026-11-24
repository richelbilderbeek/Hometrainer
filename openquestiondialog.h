#ifndef OPENQUESTIONDIALOG_H
#define OPENQUESTIONDIALOG_H

#include "questiondialog.h"
#include "openquestion.h"

namespace ribi {

///Dialog for an OpenQuestion
struct OpenQuestionDialog final : public QuestionDialog
{
  explicit OpenQuestionDialog(const OpenQuestion& open_question);

  ///The answer the user has typed in so far
  const std::string& GetAnswerInProgress() const noexcept { return m_answer_in_progress; }

  const OpenQuestion& GetOpenQuestion() const noexcept { return m_open_question; }
  const Question& GetQuestion() const noexcept override;

  ///The answer the user has typed in so far
  void SetAnswerInProgress(const std::string& answer_in_progress) noexcept;

  ///Submit an answer
  ///For an open question, s will be the anwer
  void Submit(const std::string& s) override;

  std::string ToStr() const noexcept override;

  private:
  std::string m_answer_in_progress;
  OpenQuestion m_open_question;
};

void TestOpenQuestionDialog() noexcept;

} //~namespace ribi

#endif // OPENQUESTIONDIALOG_H
