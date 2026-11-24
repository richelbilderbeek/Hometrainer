#ifndef QTHOMETRAINERMAINDIALOG_H
#define QTHOMETRAINERMAINDIALOG_H

#include <memory>

#include <QDialog>

class QKeyEvent;
class QTimer;

namespace Ui {
  class QtHometrainerMainDialog;
}

namespace ribi {

struct HometrainerMainDialog;
class QtQuestionDialog;
struct Question;

///Qt dialog to do an exercise or a test
class QtHometrainerMainDialog : public QDialog
{
  Q_OBJECT

public:
  explicit QtHometrainerMainDialog(
    const std::shared_ptr<HometrainerMainDialog>& dialog,
    QWidget *parent = nullptr);
  QtHometrainerMainDialog(const QtHometrainerMainDialog&) = delete;
  QtHometrainerMainDialog& operator=(const QtHometrainerMainDialog&) = delete;
  ~QtHometrainerMainDialog() noexcept;

  const HometrainerMainDialog& GetDialog() const noexcept { return *m_dialog; }

  ///Get the dialog of the current question
  QtQuestionDialog * GetQuestionDialog() const noexcept { return m_qtdialog; }

protected:
  void keyPressEvent(QKeyEvent* event) override;

private slots:
  void NewQuestion();
  void OnSubmitted(const bool is_correct);
  void on_button_next_clicked();
  void on_button_quit_clicked();

private:
  Ui::QtHometrainerMainDialog *ui;
  const std::shared_ptr<HometrainerMainDialog> m_dialog;

  ///The dialog of the current question, owned by this dialog
  QtQuestionDialog * m_qtdialog;

  ///Timer to go to the next question automatically
  QTimer * const m_timer;

  void DisplayScore();
  void SetQuestion(const Question& question);
};

///Create the correct Qt dialog for a (derived class of) Question
QtQuestionDialog * CreateQtQuestionDialog(const Question& question);

void TestQtHometrainerMainDialog() noexcept;

} //~namespace ribi

#endif // QTHOMETRAINERMAINDIALOG_H
