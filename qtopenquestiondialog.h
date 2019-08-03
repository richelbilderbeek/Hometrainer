#ifndef QTOPENQUESTIONDIALOG_H
#define QTOPENQUESTIONDIALOG_H





#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "qthideandshowdialog.h"

#include "questiondialog.h"
#include "qtquestiondialog.h"


namespace Ui {
  class QtOpenQuestionDialog;
}

namespace ribi {

struct Question;
struct OpenQuestion;
struct OpenQuestionDialog;

class QtOpenQuestionDialog : public QtQuestionDialog
{
  Q_OBJECT

public:
  explicit QtOpenQuestionDialog(QWidget *parent = 0);
  ///Will throw if the QuestionDialog is not an OpenQuestionDialog
  //explicit QtOpenQuestionDialog(QWidget *parent = 0);
  QtOpenQuestionDialog(const QtOpenQuestionDialog&) = delete;
  QtOpenQuestionDialog& operator=(const QtOpenQuestionDialog&) = delete;
  ~QtOpenQuestionDialog() noexcept;

  boost::shared_ptr<const QuestionDialog> GetDialog() const;
  void SetDialog(const boost::shared_ptr<QuestionDialog>& dialog);

  void SetOpenQuestionDialog(const boost::shared_ptr<OpenQuestionDialog>& dialog) noexcept;


  static std::string GetVersion() noexcept;
  static std::vector<std::string> GetVersionHistory() noexcept;

private slots:
  void on_button_submit_clicked() noexcept;

  void on_edit_answer_textChanged(const QString &arg1);

private:
  Ui::QtOpenQuestionDialog *ui;
  boost::shared_ptr<OpenQuestionDialog> m_openquestiondialog;

  void OnOpenQuestionDialogChanged(OpenQuestionDialog * const open_question_dialog) noexcept;
  void OnOpenQuestionChanged(const boost::shared_ptr<OpenQuestion>& open_question) noexcept;
  void OnQuit() noexcept;
  void OnSubmit(const bool was_answer_correct) noexcept;
};

} //~namespace ribi

#endif // QTOPENQUESTIONDIALOG_H
