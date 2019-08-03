#ifndef MULTIPLECHOICEQUESTIONDIALOG_H
#define MULTIPLECHOICEQUESTIONDIALOG_H

#pragma GCC diagnostic push


#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "questiondialog.h"
#pragma GCC diagnostic pop

namespace ribi {

struct MultipleChoiceQuestion;
struct Question;

struct MultipleChoiceQuestionDialog final : public QuestionDialog
{
  explicit MultipleChoiceQuestionDialog(const std::string& question);

  ///Will work if question is not nullptr
  explicit MultipleChoiceQuestionDialog(const boost::shared_ptr<const MultipleChoiceQuestion> question);

  ~MultipleChoiceQuestionDialog() noexcept;

  ///The answer the user has typed in so far
  int GetAnswerInProgress() const noexcept { return m_answer_in_progress; }

  boost::shared_ptr<const MultipleChoiceQuestion> GetMultipleChoiceQuestion() const noexcept { return m_question; }


  boost::shared_ptr<const Question> GetQuestion() const noexcept;

  ///Obtain the version
  static std::string GetVersion() noexcept;

  ///Obtain the version history
  static std::vector<std::string> GetVersionHistory() noexcept;

  ///The answer the user has selected
  ///Used for synching between multiple QtMultipleChoiceQuestionDialogs displaying
  ///the same MultipleChoiceQuestionDialog
  void SetAnswerInProgress(const int index) noexcept;

  ///Submit an answer
  ///For a multiple choice question, s will be the index of the answer
  void Submit(const std::string& s);

  std::string ToStr() const noexcept;

  mutable boost::signals2::signal<void (MultipleChoiceQuestionDialog*)> m_signal_mc_question_changed;

  private:
  int m_answer_in_progress;

  const boost::shared_ptr<const MultipleChoiceQuestion> m_question;

  ///Create a default Question
  //static MultipleChoiceQuestion * CreateDefaultQuestion();
  static boost::shared_ptr<MultipleChoiceQuestion> CreateDefaultQuestion() noexcept;

};

void TestMultipleChoiceQuestionDialog() noexcept;

} //~namespace ribi

#endif // MULTIPLECHOICEQUESTIONDIALOG_H
