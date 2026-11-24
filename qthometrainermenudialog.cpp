#include "qthometrainermenudialog.h"

#include <cassert>
#include <stdexcept>

#include <QDir>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

#include "hometrainerhelper.h"
#include "hometrainermaindialog.h"
#include "hometrainermenudialog.h"
#include "hometrainerresources.h"
#include "hometrainertestfile.h"
#include "qthometrainercreateprintabletestdialog.h"
#include "qthometrainercreatetestdialog.h"
#include "qthometrainermaindialog.h"
#include "qthometrainerscoretestsdialog.h"
#include "question.h"

#include "ui_qthometrainermenudialog.h"

ribi::QtHometrainerMenuDialog::QtHometrainerMenuDialog(QWidget *parent) noexcept :
    QDialog(parent),
    ui(new Ui::QtHometrainerMenuDialog),
    m_exercise{},
    m_test{},
    m_test_filename{},
    m_test_key{0}
{
  ui->setupUi(this);
  ui->label->setPixmap(
    QPixmap(":/resources/ToolHometrainerQuestionmark.png").scaledToHeight(160, Qt::SmoothTransformation)
  );
  UpdateButtons();
}

ribi::QtHometrainerMenuDialog::~QtHometrainerMenuDialog() noexcept
{
  delete ui;
}

bool ribi::QtHometrainerMenuDialog::LoadExercise(const std::string& filename)
{
  m_exercise = CreateQuestions(filename);
  UpdateButtons();
  return HasExercise();
}

void ribi::QtHometrainerMenuDialog::LoadTest(const std::string& filename, const int key)
{
  m_test.clear();
  UpdateButtons();
  const auto questions = ribi::LoadTest(filename, key);
  if (questions.empty())
  {
    throw std::runtime_error("The test has no questions");
  }
  m_test = questions;
  m_test_filename = filename;
  m_test_key = key;
  UpdateButtons();
}

void ribi::QtHometrainerMenuDialog::on_button_about_clicked() noexcept
{
  const HometrainerMenuDialog d;
  QString about;
  QString details;
  for (const auto& s: d.GetAbout()) { about += QString::fromStdString(s) + "\n"; }
  details += "Example exercise:\n\n";
  for (const auto& s: d.GetExample()) { details += QString::fromStdString(s) + "\n"; }
  details += "\nThe first line must start with 'Hometrainer'.\n"
    "Every other line is a question:\n"
    "  [image],[question],[answer]\n"
    "is an open question, where multiple correct answers are seperated by a slash,\n"
    "  [image],[question],[answer],[wrong answer 1],[wrong answer 2],...\n"
    "is a multiple choice question. Use '-' if there is no image,\n"
    "use '\\,' for a comma that is part of the text.\n";
  QMessageBox box(this);
  box.setWindowTitle("About Hometrainer");
  box.setText(about.trimmed());
  box.setDetailedText(details);
  box.setIconPixmap(QPixmap(":/resources/ToolHometrainerQuestionmark.png").scaledToHeight(64, Qt::SmoothTransformation));
  box.exec();
}

void ribi::QtHometrainerMenuDialog::on_button_create_examples_clicked()
{
  const QString folder = QFileDialog::getExistingDirectory(
    this, "Please select the folder to create the example exercises in", QDir::currentPath()
  );
  if (folder.isEmpty()) return;
  const QString old_folder = QDir::currentPath();
  QDir::setCurrent(folder);
  try
  {
    HometrainerMenuDialog::CreateExamples();
    QMessageBox::information(this, "Hometrainer", "Created the example exercises in '" + folder + "'");
  }
  catch (std::exception& e)
  {
    QMessageBox::critical(this, "Hometrainer", e.what());
  }
  QDir::setCurrent(old_folder);
}

void ribi::QtHometrainerMenuDialog::on_button_create_printable_test_clicked()
{
  QtHometrainerCreatePrintableTestDialog d(this);
  d.exec();
}

void ribi::QtHometrainerMenuDialog::on_button_create_test_clicked()
{
  QtHometrainerCreateTestDialog d(this);
  d.exec();
}

void ribi::QtHometrainerMenuDialog::on_button_load_exercise_clicked() noexcept
{
  const std::string filename = QFileDialog::getOpenFileName(
    this, "Please select a Hometrainer exercise", QString(), "Hometrainer exercises (*.txt);;All files (*)"
  ).toStdString();
  if (filename.empty()) return;
  try
  {
    if (!LoadExercise(filename))
    {
      QMessageBox::warning(this, "Hometrainer", "The file has no questions and cannot be used");
    }
  }
  catch (std::exception& e)
  {
    QMessageBox::warning(this, "Hometrainer", e.what());
  }
}

void ribi::QtHometrainerMenuDialog::on_button_load_test_clicked() noexcept
{
  const std::string filename = QFileDialog::getOpenFileName(
    this, "Please select a Hometrainer test", QString(), "Hometrainer tests (*.txt);;All files (*)"
  ).toStdString();
  if (filename.empty()) return;
  bool ok = false;
  const int key = QInputDialog::getInt(this, "Hometrainer", "What is the encryption key?", 0,
    -2147483647, 2147483647, 1, &ok
  );
  if (!ok) return;
  try
  {
    LoadTest(filename, key);
  }
  catch (std::exception& e)
  {
    QMessageBox::warning(this, "Hometrainer", e.what());
  }
}

void ribi::QtHometrainerMenuDialog::on_button_quit_clicked() noexcept
{
  close();
}

void ribi::QtHometrainerMenuDialog::on_button_score_tests_clicked()
{
  QtHometrainerScoreTestsDialog d(this);
  d.exec();
}

void ribi::QtHometrainerMenuDialog::on_button_start_exercise_clicked() noexcept
{
  assert(HasExercise());
  try
  {
    const auto dialog = std::make_shared<HometrainerMainDialog>(m_exercise);
    QtHometrainerMainDialog d(dialog, this);
    d.exec();
  }
  catch (std::exception& e)
  {
    QMessageBox::critical(this, "Hometrainer", e.what());
  }
}

void ribi::QtHometrainerMenuDialog::on_button_start_test_clicked() noexcept
{
  assert(HasTest());
  bool ok = false;
  const std::string username = Trim(QInputDialog::getText(
    this, "Hometrainer", "Please enter your full name", QLineEdit::Normal, "", &ok
  ).toStdString());
  if (!ok || username.empty()) return;

  const QString folder = QFileDialog::getExistingDirectory(
    this, "Please select the folder to save your results in",
    QString::fromStdString(GetPath(m_test_filename))
  );
  if (folder.isEmpty()) return;

  HometrainerTestSession session;
  session.m_username = username;
  session.m_key = m_test_key;
  session.m_results_filename = JoinPath(folder.toStdString(), username + ".txt");
  if (IsRegularFile(session.m_results_filename))
  {
    const auto answer = QMessageBox::question(this, "Hometrainer",
      QString("The file '") + session.m_results_filename.c_str() + "' already exists. Overwrite it?"
    );
    if (answer != QMessageBox::Yes) return;
  }
  try
  {
    const auto dialog = std::make_shared<HometrainerMainDialog>(m_test, session);
    QtHometrainerMainDialog d(dialog, this);
    d.exec();
  }
  catch (std::exception& e)
  {
    QMessageBox::critical(this, "Hometrainer", e.what());
  }
}

void ribi::QtHometrainerMenuDialog::UpdateButtons() noexcept
{
  ui->button_start_exercise->setEnabled(HasExercise());
  ui->button_start_test->setEnabled(HasTest());
}

void ribi::TestQtHometrainerMenuDialog() noexcept
{
  const HometrainerResources r;
  QtHometrainerMenuDialog d;
  assert(!d.HasExercise());
  assert(!d.HasTest());
  assert(d.LoadExercise(r.GetExerciseTest()));
  assert(d.HasExercise());

  const std::string test_filename = "qthometrainermenudialog_test_tmp.txt";
  SaveContainer(CreateTest(FileToVector(r.GetExerciseTest()), 10, 42), test_filename);
  try
  {
    d.LoadTest(test_filename, 41);
    assert(!"Should not get here");
  }
  catch (std::exception&)
  {
    //OK
  }
  assert(!d.HasTest());
  d.LoadTest(test_filename, 42);
  assert(d.HasTest());
}
