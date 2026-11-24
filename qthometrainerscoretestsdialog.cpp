#include "qthometrainerscoretestsdialog.h"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <stdexcept>

#include <QFileDialog>
#include <QHeaderView>
#include <QMessageBox>

#include "hometrainerhelper.h"
#include "hometrainertestresults.h"
#include "hometrainertestscorer.h"
#include "qthometrainerhelper.h"

#include "ui_qthometrainerscoretestsdialog.h"

ribi::QtHometrainerScoreTestsDialog::QtHometrainerScoreTestsDialog(QWidget *parent)
  : QDialog(parent),
    ui(new Ui::QtHometrainerScoreTestsDialog),
    m_filenames{},
    m_scorer{}
{
  ui->setupUi(this);
  if (IsWebAssembly())
  {
    ui->button_select->setText("Add test results");
  }
  for (QTableWidget * const table: { ui->table_students, ui->table_questions })
  {
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    table->horizontalHeader()->setStretchLastSection(true);
  }
}

ribi::QtHometrainerScoreTestsDialog::~QtHometrainerScoreTestsDialog() noexcept
{
  delete ui;
}

void ribi::QtHometrainerScoreTestsDialog::DisplayScores()
{
  assert(m_scorer);
  //Disable sorting while filling in the tables
  ui->table_students->setSortingEnabled(false);
  ui->table_questions->setSortingEnabled(false);

  const auto& students = m_scorer->GetStudentScores();
  ui->table_students->setRowCount(static_cast<int>(students.size()));
  int row = 0;
  for (const auto& s: students)
  {
    const auto number = [](const double x)
    {
      QTableWidgetItem * const item = new QTableWidgetItem;
      item->setData(Qt::DisplayRole, x);
      return item;
    };
    ui->table_students->setItem(row, 0, new QTableWidgetItem(s.m_name.c_str()));
    ui->table_students->setItem(row, 1, number(s.m_n_correct));
    ui->table_students->setItem(row, 2, number(s.m_n_questions));
    ui->table_students->setItem(row, 3, new QTableWidgetItem(FormatGrade(s.m_grade).c_str()));
    ui->table_students->setItem(row, 4, new QTableWidgetItem(s.m_comment.c_str()));
    ++row;
  }

  const auto questions = m_scorer->GetQuestionScores();
  ui->table_questions->setRowCount(static_cast<int>(questions.size()));
  row = 0;
  for (const auto& q: questions)
  {
    ui->table_questions->setItem(row, 0, new QTableWidgetItem(q.m_question.c_str()));
    ui->table_questions->setItem(row, 1, new QTableWidgetItem(QString::number(q.m_n_correct)));
    ui->table_questions->setItem(row, 2, new QTableWidgetItem(QString::number(q.m_n_answered)));
    ui->table_questions->setItem(row, 3, new QTableWidgetItem(FormatGrade(q.GetFractionCorrect()).c_str()));
    ++row;
  }
  ui->label_students->setText("Students (" + QString::number(students.size()) + ")");
  ui->label_questions->setText("Questions (" + QString::number(questions.size()) + ")");
  ui->table_students->setSortingEnabled(true);
  ui->table_questions->setSortingEnabled(true);
}

void ribi::QtHometrainerScoreTestsDialog::on_button_save_clicked()
{
  assert(m_scorer);
  const std::string folder = m_filenames.empty() ? "" : GetPath(m_filenames[0]);
  const std::vector<std::string> questions = m_scorer->QuestionScoresToCsv();
  SaveFile(this, "Save the scores", JoinPath(folder, "scores.csv"),
    "Comma-seperated files (*.csv);;All files (*)", m_scorer->StudentScoresToCsv(),
    [this, questions](const std::string& filename)
    {
      const std::string questions_filename = RemoveExtension(filename) + "_questions.csv";
      try
      {
        SaveContainer(questions, questions_filename);
        if (IsWebAssembly()) DownloadFile(questions_filename);
      }
      catch (std::exception& e)
      {
        ShowMessage(this, e.what(), QMessageBox::Warning);
        return;
      }
      ShowMessage(this,
        "Student scores saved to '" + QString::fromStdString(filename) + "'\n"
        "Question scores saved to '" + QString::fromStdString(questions_filename) + "'"
      );
    }
  );
}

void ribi::QtHometrainerScoreTestsDialog::on_button_score_clicked()
{
  try
  {
    Score();
  }
  catch (std::exception& e)
  {
    ShowMessage(this, e.what(), QMessageBox::Warning);
  }
}

void ribi::QtHometrainerScoreTestsDialog::on_button_select_clicked()
{
  OpenFiles(this, "Please select the test results of the students", "Test results (*.txt);;All files (*)",
    [this](const std::vector<std::string>& filenames)
    {
      if (!IsWebAssembly())
      {
        SetFilenames(filenames);
        return;
      }
      //In a web browser, files are uploaded one at a time
      std::vector<std::string> v = m_filenames;
      for (const auto& filename: filenames)
      {
        if (std::find(v.begin(), v.end(), filename) == v.end()) v.push_back(filename);
      }
      SetFilenames(v);
    }
  );
}

void ribi::QtHometrainerScoreTestsDialog::Score()
{
  m_scorer = std::make_unique<HometrainerTestScorer>(ui->box_key->value(), ui->box_penalty->value());
  for (const auto& filename: m_filenames) { m_scorer->Add(filename); }
  DisplayScores();
  ui->button_save->setEnabled(true);
}

void ribi::QtHometrainerScoreTestsDialog::SetFilenames(const std::vector<std::string>& filenames)
{
  m_filenames = filenames;
  ui->label_files->setText(QString::number(filenames.size()) + " file(s) selected");
  ui->button_score->setEnabled(!filenames.empty());
}

void ribi::QtHometrainerScoreTestsDialog::SetSettings(const int key, const int penalty)
{
  ui->box_key->setValue(key);
  ui->box_penalty->setValue(penalty);
}

void ribi::TestQtHometrainerScoreTestsDialog() noexcept
{
  const int key = 42;
  const std::string filename = "Alice.txt";
  {
    HometrainerTestResults r("Alice");
    r.Add("--1+1=", true);
    r.Add("--1+2=", false);
    SaveContainer(r.ToEncryptedLines(key), filename);
  }
  QtHometrainerScoreTestsDialog d;
  assert(!d.GetScorer());
  d.SetFilenames( { filename } );
  d.SetSettings(key, 0);
  d.Score();
  assert(d.GetScorer());
  assert(d.GetScorer()->GetStudentScores().size() == 1);
  assert(d.GetScorer()->GetStudentScores()[0].m_grade == 5.5);
  std::remove(filename.c_str());
}
