#ifndef QTOPENQUESTIONDIALOG_H
#define QTOPENQUESTIONDIALOG_H

#include "openquestiondialog.h"
#include "qtquestiondialog.h"

namespace Ui {
  class QtOpenQuestionDialog;
}

namespace ribi {

///Qt dialog for an OpenQuestion
class QtOpenQuestionDialog : public QtQuestionDialog
{
  Q_OBJECT

public:
  explicit QtOpenQuestionDialog(
    const OpenQuestion& open_question,
    QWidget *parent = nullptr
  );
  QtOpenQuestionDialog(const QtOpenQuestionDialog&) = delete;
  QtOpenQuestionDialog& operator=(const QtOpenQuestionDialog&) = delete;
  ~QtOpenQuestionDialog() noexcept;

  const QuestionDialog& GetDialog() const noexcept override { return m_dialog; }

  ///Set the answer, as if the user typed it
  void SetAnswer(const std::string& answer);

  ///Submit the answer, as if the user clicked the submit button
  void Submit();

private slots:
  void on_button_submit_clicked() noexcept;
  void on_edit_answer_textChanged(const QString &arg1);

private:
  Ui::QtOpenQuestionDialog *ui;
  OpenQuestionDialog m_dialog;
};

void TestQtOpenQuestionDialog() noexcept;

} //~namespace ribi

#endif // QTOPENQUESTIONDIALOG_H
