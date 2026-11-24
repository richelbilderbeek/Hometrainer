#ifndef QTHOMETRAINERCREATETESTDIALOG_H
#define QTHOMETRAINERCREATETESTDIALOG_H

#include <string>
#include <vector>

#include <QDialog>

namespace Ui {
  class QtHometrainerCreateTestDialog;
}

namespace ribi {

///Qt dialog to create an encrypted test from an exercise
class QtHometrainerCreateTestDialog : public QDialog
{
  Q_OBJECT

public:
  explicit QtHometrainerCreateTestDialog(QWidget *parent = nullptr);
  QtHometrainerCreateTestDialog(const QtHometrainerCreateTestDialog&) = delete;
  QtHometrainerCreateTestDialog& operator=(const QtHometrainerCreateTestDialog&) = delete;
  ~QtHometrainerCreateTestDialog() noexcept;

  ///Create the test, as if the user clicked the button.
  ///Returns false if this fails
  bool CreateTest();

  ///Obtain the encrypted test, which is empty if not created yet
  const std::vector<std::string>& GetTest() const noexcept { return m_test; }

  ///Set the exercise, as if the user loaded it
  void SetExercise(const std::vector<std::string>& lines);

  ///Set the key and number of questions, as if the user typed these
  void SetSettings(const int key, const int n_questions);

private slots:
  void on_button_create_clicked();
  void on_button_load_clicked();
  void on_button_save_clicked();
  void on_edit_exercise_textChanged();

private:
  Ui::QtHometrainerCreateTestDialog *ui;

  ///The folder of the exercise, to save the test in by default
  QString m_folder;

  ///The encrypted test
  std::vector<std::string> m_test;
};

///Convert the lines of a QPlainTextEdit to a std::vector
std::vector<std::string> ToLines(const QString& text);

///Convert a std::vector of lines to a text for a QPlainTextEdit
QString ToText(const std::vector<std::string>& lines);

void TestQtHometrainerCreateTestDialog() noexcept;

} //~namespace ribi

#endif // QTHOMETRAINERCREATETESTDIALOG_H
