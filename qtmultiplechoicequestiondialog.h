#ifndef QTMULTIPLECHOICEQUESTIONDIALOG_H
#define QTMULTIPLECHOICEQUESTIONDIALOG_H

#include <vector>

#include "multiplechoicequestiondialog.h"
#include "qtquestiondialog.h"

class QRadioButton;

namespace Ui {
  class QtMultipleChoiceQuestionDialog;
}

namespace ribi {

///Qt dialog for a MultipleChoiceQuestion
class QtMultipleChoiceQuestionDialog : public QtQuestionDialog
{
  Q_OBJECT

public:
  explicit QtMultipleChoiceQuestionDialog(
    const MultipleChoiceQuestion& question,
    QWidget *parent = nullptr
  );
  QtMultipleChoiceQuestionDialog(const QtMultipleChoiceQuestionDialog&) = delete;
  QtMultipleChoiceQuestionDialog& operator=(const QtMultipleChoiceQuestionDialog&) = delete;
  ~QtMultipleChoiceQuestionDialog() noexcept;

  const QuestionDialog& GetDialog() const noexcept override { return m_dialog; }

  ///Select an option, as if the user clicked on it
  void SelectOption(const int index);

  ///Submit the answer, as if the user clicked the submit button
  void Submit();

private slots:
  void on_button_submit_clicked() noexcept;

private:
  Ui::QtMultipleChoiceQuestionDialog *ui;

  ///One radio button per option
  std::vector<QRadioButton*> m_buttons;

  MultipleChoiceQuestionDialog m_dialog;
};

void TestQtMultipleChoiceQuestionDialog() noexcept;

} //~namespace ribi

#endif // QTMULTIPLECHOICEQUESTIONDIALOG_H
