#include "qthometrainercreateprintabletestdialog.h"

#include <algorithm>
#include <cassert>
#include <stdexcept>

#include <QFileDialog>
#include <QMessageBox>

#include "hometrainerhelper.h"
#include "hometrainerprintabletest.h"
#include "qthometrainercreatetestdialog.h"
#include "question.h"

#include "ui_qthometrainercreateprintabletestdialog.h"

ribi::QtHometrainerCreatePrintableTestDialog::QtHometrainerCreatePrintableTestDialog(QWidget *parent)
  : QDialog(parent),
    ui(new Ui::QtHometrainerCreatePrintableTestDialog),
    m_folder{},
    m_printable_test{}
{
  ui->setupUi(this);
}

ribi::QtHometrainerCreatePrintableTestDialog::~QtHometrainerCreatePrintableTestDialog() noexcept
{
  delete ui;
}

bool ribi::QtHometrainerCreatePrintableTestDialog::CreatePrintableTest()
{
  m_printable_test.clear();
  ui->edit_printable->clear();
  ui->button_save->setEnabled(false);
  try
  {
    m_printable_test = ribi::CreatePrintableTest(
      ToLines(ui->edit_exercise->toPlainText()),
      ui->box_n_questions->value(),
      ui->box_format->currentIndex() == 0 ? PrintableTestFormat::html : PrintableTestFormat::text,
      ui->box_use_code->isChecked() ? ui->edit_code->text().toStdString() : ""
    );
  }
  catch (std::exception& e)
  {
    if (isVisible()) QMessageBox::warning(this, "Hometrainer", e.what());
    return false;
  }
  ui->edit_printable->setPlainText(ToText(m_printable_test));
  ui->button_save->setEnabled(true);
  return true;
}

void ribi::QtHometrainerCreatePrintableTestDialog::on_box_use_code_toggled(bool checked)
{
  ui->edit_code->setEnabled(checked);
}

void ribi::QtHometrainerCreatePrintableTestDialog::on_button_create_clicked()
{
  CreatePrintableTest();
}

void ribi::QtHometrainerCreatePrintableTestDialog::on_button_load_clicked()
{
  const QString filename = QFileDialog::getOpenFileName(
    this, "Please select a Hometrainer exercise", m_folder, "Hometrainer exercises (*.txt);;All files (*)"
  );
  if (filename.isEmpty()) return;
  try
  {
    SetExercise(FileToVector(filename.toStdString()));
    m_folder = QString::fromStdString(GetPath(filename.toStdString()));
  }
  catch (std::exception& e)
  {
    QMessageBox::warning(this, "Hometrainer", e.what());
  }
}

void ribi::QtHometrainerCreatePrintableTestDialog::on_button_save_clicked()
{
  const bool html = ui->box_format->currentIndex() == 0;
  const QString filename = QFileDialog::getSaveFileName(
    this, "Save the test on paper", m_folder,
    html ? "Web pages (*.html);;All files (*)" : "Text files (*.txt);;All files (*)"
  );
  if (filename.isEmpty()) return;
  try
  {
    SaveContainer(m_printable_test, filename.toStdString());
    QMessageBox::information(this, "Hometrainer",
      "Test on paper saved to '" + filename + "'.\n\n"
      + (html ? "Images are shown if they are in the same folder." : "")
    );
  }
  catch (std::exception& e)
  {
    QMessageBox::warning(this, "Hometrainer", e.what());
  }
}

void ribi::QtHometrainerCreatePrintableTestDialog::on_edit_exercise_textChanged()
{
  m_printable_test.clear();
  ui->edit_printable->clear();
  ui->button_save->setEnabled(false);
  const int n_questions = static_cast<int>(CreateQuestions(ToLines(ui->edit_exercise->toPlainText())).size());
  ui->label_exercise->setText("Exercise (" + QString::number(n_questions) + " questions)");
}

void ribi::QtHometrainerCreatePrintableTestDialog::SetExercise(const std::vector<std::string>& lines)
{
  ui->edit_exercise->setPlainText(ToText(lines));
  const int n_questions = static_cast<int>(CreateQuestions(lines).size());
  if (n_questions > 0) ui->box_n_questions->setValue(n_questions);
}

void ribi::QtHometrainerCreatePrintableTestDialog::SetSettings(
  const int n_questions,
  const std::string& answer_code,
  const bool html)
{
  ui->box_n_questions->setValue(n_questions);
  ui->box_use_code->setChecked(!answer_code.empty());
  if (!answer_code.empty()) ui->edit_code->setText(answer_code.c_str());
  ui->box_format->setCurrentIndex(html ? 0 : 1);
}

void ribi::TestQtHometrainerCreatePrintableTestDialog() noexcept
{
  const std::vector<std::string> exercise = {
    "Hometrainer (C) 2009 Richel Bilderbeek",
    "-,1+1=,2",
    "-,1+2=,3,1,2"
  };
  //No exercise
  {
    QtHometrainerCreatePrintableTestDialog d;
    assert(!d.CreatePrintableTest());
  }
  //HTML
  {
    QtHometrainerCreatePrintableTestDialog d;
    d.SetExercise(exercise);
    d.SetSettings(10, "", true);
    assert(d.CreatePrintableTest());
    assert(d.GetPrintableTest().front() == "<!DOCTYPE html>");
  }
  //Text with code
  {
    QtHometrainerCreatePrintableTestDialog d;
    d.SetExercise(exercise);
    d.SetSettings(10, "XY", false);
    assert(d.CreatePrintableTest());
    const auto& v = d.GetPrintableTest();
    assert(std::count_if(v.begin(), v.end(),
      [](const std::string& s) { return s == "O X) 3" || s == "O Y) 3"; }) == 1
    );
  }
}
