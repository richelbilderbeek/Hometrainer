#ifndef QTHOMETRAINERMENUDIALOG_H
#define QTHOMETRAINERMENUDIALOG_H

#include <memory>
#include <string>
#include <vector>

#include <QDialog>

namespace Ui {
  class QtHometrainerMenuDialog;
}

namespace ribi {

struct Question;

///The main menu of Hometrainer
class QtHometrainerMenuDialog : public QDialog
{
  Q_OBJECT

public:
  explicit QtHometrainerMenuDialog(QWidget *parent = nullptr) noexcept;
  QtHometrainerMenuDialog(const QtHometrainerMenuDialog&) = delete;
  QtHometrainerMenuDialog& operator=(const QtHometrainerMenuDialog&) = delete;
  ~QtHometrainerMenuDialog() noexcept;

  ///Load an exercise, as if the user selected this file.
  ///Returns false if the file has no questions
  bool LoadExercise(const std::string& filename);

  ///Load a test, as if the user selected this file and typed in the key.
  ///Throws if the file has no questions or the key is incorrect
  void LoadTest(const std::string& filename, const int key);

  ///Is an exercise loaded, so that it can be started?
  bool HasExercise() const noexcept { return !m_exercise.empty(); }

  ///Is a test loaded, so that it can be started?
  bool HasTest() const noexcept { return !m_test.empty(); }

private:
  Ui::QtHometrainerMenuDialog *ui;

  ///The questions of the loaded exercise
  std::vector<std::shared_ptr<const Question>> m_exercise;

  ///The questions of the loaded test
  std::vector<std::shared_ptr<const Question>> m_test;

  ///The filename of the loaded test
  std::string m_test_filename;

  ///The key of the loaded test
  int m_test_key;

  void UpdateButtons() noexcept;

private slots:
  void on_button_about_clicked() noexcept;
  void on_button_create_examples_clicked();
  void on_button_create_printable_test_clicked();
  void on_button_create_test_clicked();
  void on_button_load_exercise_clicked() noexcept;
  void on_button_load_test_clicked() noexcept;
  void on_button_quit_clicked() noexcept;
  void on_button_score_tests_clicked();
  void on_button_start_exercise_clicked() noexcept;
  void on_button_start_test_clicked() noexcept;
};

void TestQtHometrainerMenuDialog() noexcept;

} //~namespace ribi

#endif // QTHOMETRAINERMENUDIALOG_H
