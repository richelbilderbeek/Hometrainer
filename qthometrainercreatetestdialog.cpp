#include "qthometrainercreatetestdialog.h"

#include <cassert>
#include <stdexcept>

#include <QFileDialog>
#include <QMessageBox>

#include "hometrainerhelper.h"
#include "hometrainertestfile.h"
#include "qthometrainerhelper.h"
#include "question.h"

#include "ui_qthometrainercreatetestdialog.h"

ribi::QtHometrainerCreateTestDialog::QtHometrainerCreateTestDialog(QWidget *parent)
  : QDialog(parent),
    ui(new Ui::QtHometrainerCreateTestDialog),
    m_folder{},
    m_test{}
{
  ui->setupUi(this);
}

ribi::QtHometrainerCreateTestDialog::~QtHometrainerCreateTestDialog() noexcept
{
  delete ui;
}

bool ribi::QtHometrainerCreateTestDialog::CreateTest()
{
  m_test.clear();
  ui->edit_test->clear();
  ui->button_save->setEnabled(false);
  try
  {
    m_test = ribi::CreateTest(
      ToLines(ui->edit_exercise->toPlainText()),
      ui->box_n_questions->value(),
      ui->box_key->value()
    );
  }
  catch (std::exception& e)
  {
    if (isVisible()) ShowMessage(this, e.what(), QMessageBox::Warning);
    return false;
  }
  ui->edit_test->setPlainText(ToText(m_test));
  ui->button_save->setEnabled(true);
  return true;
}

void ribi::QtHometrainerCreateTestDialog::on_button_create_clicked()
{
  CreateTest();
}

void ribi::QtHometrainerCreateTestDialog::on_button_load_clicked()
{
  OpenExercise(this,
    [this](const std::string& filename)
    {
      try
      {
        SetExercise(FileToVector(filename));
        m_folder = QString::fromStdString(GetPath(filename));
      }
      catch (std::exception& e)
      {
        ShowMessage(this, e.what(), QMessageBox::Warning);
      }
    }
  );
}

void ribi::QtHometrainerCreateTestDialog::on_button_save_clicked()
{
  SaveFile(this, "Save the test", JoinPath(m_folder.toStdString(), "test.txt"),
    "Hometrainer tests (*.txt);;All files (*)", m_test,
    [this](const std::string& filename)
    {
      ShowMessage(this,
        "Test saved to '" + QString::fromStdString(filename) + "'.\n\n"
        "Remember the key, as it is needed to do the test and to score it.\n"
        "Put the images used by the test in the same folder as the test."
      );
    }
  );
}

void ribi::QtHometrainerCreateTestDialog::on_edit_exercise_textChanged()
{
  //The test does not match the exercise anymore
  m_test.clear();
  ui->edit_test->clear();
  ui->button_save->setEnabled(false);
  const int n_questions = static_cast<int>(CreateQuestions(ToLines(ui->edit_exercise->toPlainText())).size());
  ui->label_exercise->setText("Exercise (" + QString::number(n_questions) + " questions)");
}

void ribi::QtHometrainerCreateTestDialog::SetExercise(const std::vector<std::string>& lines)
{
  ui->edit_exercise->setPlainText(ToText(lines));
  //Use all questions by default
  const int n_questions = static_cast<int>(CreateQuestions(lines).size());
  if (n_questions > 0) ui->box_n_questions->setValue(n_questions);
}

void ribi::QtHometrainerCreateTestDialog::SetSettings(const int key, const int n_questions)
{
  ui->box_key->setValue(key);
  ui->box_n_questions->setValue(n_questions);
}

std::vector<std::string> ribi::ToLines(const QString& text)
{
  std::vector<std::string> v;
  for (const QString& line: text.split('\n')) { v.push_back(line.toStdString()); }
  return v;
}

QString ribi::ToText(const std::vector<std::string>& lines)
{
  QStringList v;
  for (const auto& line: lines) { v.push_back(QString::fromStdString(line)); }
  return v.join('\n');
}

void ribi::TestQtHometrainerCreateTestDialog() noexcept
{
  const std::vector<std::string> exercise = {
    "Hometrainer (C) 2009 Richel Bilderbeek",
    "-,1+1=,2",
    "-,1+2=,3,1,2"
  };
  assert(ToLines(ToText(exercise)) == exercise);
  //No exercise
  {
    QtHometrainerCreateTestDialog d;
    assert(!d.CreateTest());
    assert(d.GetTest().empty());
  }
  //Create a test
  {
    QtHometrainerCreateTestDialog d;
    d.SetExercise(exercise);
    d.SetSettings(42, 1);
    assert(d.CreateTest());
    assert(d.GetTest().size() == 2);
    assert(DecryptTest(d.GetTest(), 42).size() == 2);
  }
}
