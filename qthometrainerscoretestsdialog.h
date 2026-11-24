#ifndef QTHOMETRAINERSCORETESTSDIALOG_H
#define QTHOMETRAINERSCORETESTSDIALOG_H

#include <memory>
#include <string>
#include <vector>

#include <QDialog>

namespace Ui {
  class QtHometrainerScoreTestsDialog;
}

namespace ribi {

struct HometrainerTestScorer;

///Qt dialog to grade the test results of students
class QtHometrainerScoreTestsDialog : public QDialog
{
  Q_OBJECT

public:
  explicit QtHometrainerScoreTestsDialog(QWidget *parent = nullptr);
  QtHometrainerScoreTestsDialog(const QtHometrainerScoreTestsDialog&) = delete;
  QtHometrainerScoreTestsDialog& operator=(const QtHometrainerScoreTestsDialog&) = delete;
  ~QtHometrainerScoreTestsDialog() noexcept;

  ///Obtain the scorer, nullptr if the tests have not been scored yet
  const HometrainerTestScorer * GetScorer() const noexcept { return m_scorer.get(); }

  ///Score the tests, as if the user clicked the button
  void Score();

  ///Set the test results files, as if the user selected these
  void SetFilenames(const std::vector<std::string>& filenames);

  ///Set the key and penalty, as if the user typed these
  void SetSettings(const int key, const int penalty);

private slots:
  void on_button_save_clicked();
  void on_button_score_clicked();
  void on_button_select_clicked();

private:
  Ui::QtHometrainerScoreTestsDialog *ui;

  ///The test result files
  std::vector<std::string> m_filenames;

  std::unique_ptr<HometrainerTestScorer> m_scorer;

  void DisplayScores();
};

void TestQtHometrainerScoreTestsDialog() noexcept;

} //~namespace ribi

#endif // QTHOMETRAINERSCORETESTSDIALOG_H
