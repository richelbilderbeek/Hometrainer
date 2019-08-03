#ifndef OPENQUESTIONDIALOG_H
#define OPENQUESTIONDIALOG_H



#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include "questiondialog.h"


namespace ribi {

struct Question;
struct OpenQuestion;

///Dialog for an OpenQuestion
struct OpenQuestionDialog final : public QuestionDialog
{
  OpenQuestionDialog();
  //explicit OpenQuestionDialog(const boost::shared_ptr<const OpenQuestion>& question);

  ///The answer the user has typed in so far
  std::string GetAnswerInProgress() const noexcept { return m_answer_in_progress; }

  boost::shared_ptr<const OpenQuestion> GetOpenQuestion() const noexcept { return m_open_question; }
  boost::shared_ptr<      OpenQuestion> GetOpenQuestion()       noexcept { return m_open_question; }
  boost::shared_ptr<const Question> GetQuestion() const noexcept;

  ///The answer the user has typed in so far
  ///Used for synching between multiple QtOpenQuestionDialogs displaying
  ///the same OpenQuestionDialog
  void SetAnswerInProgress(const std::string& answer_in_progress) noexcept;

  void SetOpenQuestion(const boost::shared_ptr<OpenQuestion>& open_question) noexcept;

  static std::string GetVersion() noexcept;
  static std::vector<std::string> GetVersionHistory() noexcept;

  ///Submit an answer
  ///For an open question, s will be the anwer
  void Submit(const std::string& s);

  std::string ToStr() const noexcept;

  mutable boost::signals2::signal<void (OpenQuestionDialog*)> m_signal_open_question_changed;

  private:
  friend void boost::checked_delete<>(OpenQuestionDialog*);
  friend void boost::checked_delete<>(const OpenQuestionDialog*);
  friend class boost::detail::sp_ms_deleter<OpenQuestionDialog>;
  friend class boost::detail::sp_ms_deleter<const OpenQuestionDialog>;
  ~OpenQuestionDialog() noexcept {}

  std::string m_answer_in_progress;
  boost::shared_ptr<OpenQuestion> m_open_question;

};

void TestOpenQuestionDialog() noexcept;

} //~namespace ribi

#endif // OPENQUESTIONDIALOG_H
