#ifndef QTMULTIPLECHOICEQUESTIONDIALOG_H
#define QTMULTIPLECHOICEQUESTIONDIALOG_H

#include "qthideandshowdialog.h"

#include "questiondialog.h"
#include "qtquestiondialog.h"



namespace Ui {
  class QtMultipleChoiceQuestionDialog;
}

namespace ribi {

struct Question;
struct MultipleChoiceQuestion;
struct MultipleChoiceQuestionDialog;

class QtMultipleChoiceQuestionDialog : public QtQuestionDialog
{
  Q_OBJECT

public:
  explicit QtMultipleChoiceQuestionDialog(
    QWidget *parent = 0);

  QtMultipleChoiceQuestionDialog(
    const boost::shared_ptr<MultipleChoiceQuestionDialog>& dialog,
    QWidget *parent = 0);
  QtMultipleChoiceQuestionDialog(const QtMultipleChoiceQuestionDialog&) = delete;
  QtMultipleChoiceQuestionDialog& operator=(const QtMultipleChoiceQuestionDialog&) = delete;

  ~QtMultipleChoiceQuestionDialog() noexcept;

  boost::shared_ptr<const QuestionDialog> GetDialog() const;

  ///Obtain the version of this class
  static std::string GetVersion() noexcept;

  ///Obtain the version history of this class
  static std::vector<std::string> GetVersionHistory() noexcept;

  void SetDialog(const boost::shared_ptr<QuestionDialog>& dialog);

private slots:
  void on_button_submit_clicked() noexcept;

private:
  Ui::QtMultipleChoiceQuestionDialog *ui;
  boost::shared_ptr<MultipleChoiceQuestionDialog> m_dialog;

  ///Set the Question
  //void SetQuestion(const boost::shared_ptr<const MultipleChoiceQuestion>& question) noexcept;

  void OnMultipleChoiceQuestionDialogChanged(MultipleChoiceQuestionDialog * const mc_question_dialog) noexcept;
  void OnMultipleChoiceQuestionChanged(const boost::shared_ptr<MultipleChoiceQuestionDialog>& mc_question) noexcept;

};

} //~namespace ribi

#endif // QTMULTIPLECHOICEQUESTIONDIALOG_H
