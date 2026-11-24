#ifndef QTHOMETRAINERCREATEPRINTABLETESTDIALOG_H
#define QTHOMETRAINERCREATEPRINTABLETESTDIALOG_H

#include <string>
#include <vector>

#include <QDialog>

namespace Ui {
  class QtHometrainerCreatePrintableTestDialog;
}

namespace ribi {

///Qt dialog to create a test on paper from an exercise
class QtHometrainerCreatePrintableTestDialog : public QDialog
{
  Q_OBJECT

public:
  explicit QtHometrainerCreatePrintableTestDialog(QWidget *parent = nullptr);
  QtHometrainerCreatePrintableTestDialog(const QtHometrainerCreatePrintableTestDialog&) = delete;
  QtHometrainerCreatePrintableTestDialog& operator=(const QtHometrainerCreatePrintableTestDialog&) = delete;
  ~QtHometrainerCreatePrintableTestDialog() noexcept;

  ///Create the test on paper, as if the user clicked the button.
  ///Returns false if this fails
  bool CreatePrintableTest();

  ///Obtain the test on paper, which is empty if not created yet
  const std::vector<std::string>& GetPrintableTest() const noexcept { return m_printable_test; }

  ///Set the exercise, as if the user loaded it
  void SetExercise(const std::vector<std::string>& lines);

  ///Set the settings, as if the user typed these.
  ///An empty answer code means no answer code is used.
  ///'html' sets the format to HTML, else it is plain text
  void SetSettings(const int n_questions, const std::string& answer_code, const bool html);

private slots:
  void on_box_use_code_toggled(bool checked);
  void on_button_create_clicked();
  void on_button_load_clicked();
  void on_button_save_clicked();
  void on_edit_exercise_textChanged();

private:
  Ui::QtHometrainerCreatePrintableTestDialog *ui;

  ///The folder of the exercise, to save the test in by default,
  ///so that images can be found
  QString m_folder;

  ///The test on paper
  std::vector<std::string> m_printable_test;
};

void TestQtHometrainerCreatePrintableTestDialog() noexcept;

} //~namespace ribi

#endif // QTHOMETRAINERCREATEPRINTABLETESTDIALOG_H
