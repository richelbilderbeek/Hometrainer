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
#include "qthometrainerhelper.h"
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
  //In a web browser, the example exercises are always available
  ui->button_create_examples->setVisible(!IsWebAssembly());
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
  QMessageBox * const box = new QMessageBox(this);
  box->setAttribute(Qt::WA_DeleteOnClose);
  box->setWindowTitle("About Hometrainer");
  box->setText(about.trimmed());
  box->setDetailedText(details);
  box->setIconPixmap(QPixmap(":/resources/ToolHometrainerQuestionmark.png").scaledToHeight(64, Qt::SmoothTransformation));
  box->open();
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
    ShowMessage(this, "Created the example exercises in '" + folder + "'");
  }
  catch (std::exception& e)
  {
    ShowMessage(this, e.what(), QMessageBox::Critical);
  }
  QDir::setCurrent(old_folder);
}

void ribi::QtHometrainerMenuDialog::on_button_create_printable_test_clicked()
{
  ShowDialog(new QtHometrainerCreatePrintableTestDialog(this));
}

void ribi::QtHometrainerMenuDialog::on_button_create_test_clicked()
{
  ShowDialog(new QtHometrainerCreateTestDialog(this));
}

void ribi::QtHometrainerMenuDialog::on_button_load_exercise_clicked() noexcept
{
  OpenExercise(this,
    [this](const std::string& filename)
    {
      try
      {
        if (!LoadExercise(filename))
        {
          ShowMessage(this, "The file has no questions and cannot be used", QMessageBox::Warning);
        }
      }
      catch (std::exception& e)
      {
        ShowMessage(this, e.what(), QMessageBox::Warning);
      }
    }
  );
}

void ribi::QtHometrainerMenuDialog::on_button_load_test_clicked() noexcept
{
  OpenFile(this, "Please select a Hometrainer test", "Hometrainer tests (*.txt);;All files (*)",
    [this](const std::string& filename)
    {
      AskInt(this, "What is the encryption key?", 0,
        [this, filename](const int key)
        {
          try
          {
            LoadTest(filename, key);
          }
          catch (std::exception& e)
          {
            ShowMessage(this, e.what(), QMessageBox::Warning);
          }
        }
      );
    }
  );
}

void ribi::QtHometrainerMenuDialog::on_button_quit_clicked() noexcept
{
  close();
}

void ribi::QtHometrainerMenuDialog::on_button_score_tests_clicked()
{
  ShowDialog(new QtHometrainerScoreTestsDialog(this));
}

void ribi::QtHometrainerMenuDialog::on_button_start_exercise_clicked() noexcept
{
  assert(HasExercise());
  try
  {
    const auto dialog = std::make_shared<HometrainerMainDialog>(m_exercise);
    ShowDialog(new QtHometrainerMainDialog(dialog, this));
  }
  catch (std::exception& e)
  {
    ShowMessage(this, e.what(), QMessageBox::Critical);
  }
}

void ribi::QtHometrainerMenuDialog::on_button_start_test_clicked() noexcept
{
  assert(HasTest());
  AskText(this, "Please enter your full name",
    [this](const QString& name)
    {
      const std::string username = Trim(name.toStdString());
      if (username.empty()) return;
      std::string folder;
      if (IsWebAssembly())
      {
        //The results are downloaded when the test is done
        folder = QDir::currentPath().toStdString();
      }
      else
      {
        folder = QFileDialog::getExistingDirectory(
          this, "Please select the folder to save your results in",
          QString::fromStdString(GetPath(m_test_filename))
        ).toStdString();
        if (folder.empty()) return;
      }
      HometrainerTestSession session;
      session.m_username = username;
      session.m_key = m_test_key;
      session.m_results_filename = JoinPath(folder, username + ".txt");
      if (!IsWebAssembly() && IsRegularFile(session.m_results_filename))
      {
        const auto answer = QMessageBox::question(this, "Hometrainer",
          QString("The file '") + session.m_results_filename.c_str() + "' already exists. Overwrite it?"
        );
        if (answer != QMessageBox::Yes) return;
      }
      try
      {
        const auto dialog = std::make_shared<HometrainerMainDialog>(m_test, session);
        ShowDialog(new QtHometrainerMainDialog(dialog, this));
      }
      catch (std::exception& e)
      {
        ShowMessage(this, e.what(), QMessageBox::Critical);
      }
    }
  );
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
