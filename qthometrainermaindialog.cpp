#include "qthometrainermaindialog.h"

#include <cassert>
#include <cstdio>
#include <stdexcept>

#include <QKeyEvent>
#include <QMessageBox>
#include <QTimer>
#include <QVBoxLayout>

#include "hometrainerhelper.h"
#include "hometrainermaindialog.h"
#include "multiplechoicequestion.h"
#include "openquestion.h"
#include "openquestionfactory.h"
#include "qtmultiplechoicequestiondialog.h"
#include "qtopenquestiondialog.h"
#include "ui_qthometrainermaindialog.h"

ribi::QtHometrainerMainDialog::QtHometrainerMainDialog(
  const std::shared_ptr<HometrainerMainDialog>& dialog,
  QWidget *parent)
  : QDialog(parent),
    ui(new Ui::QtHometrainerMainDialog),
    m_dialog{dialog},
    m_qtdialog{nullptr},
    m_timer{new QTimer(this)}
{
  ui->setupUi(this);
  assert(m_dialog);
  setWindowTitle(m_dialog->IsTest() ? "Hometrainer test" : "Hometrainer exercise");
  m_timer->setSingleShot(true);
  QObject::connect(m_timer, &QTimer::timeout, this, &QtHometrainerMainDialog::NewQuestion);
  ui->button_next->setVisible(false);
  SetQuestion(m_dialog->GetCurrentQuestion());
}

ribi::QtHometrainerMainDialog::~QtHometrainerMainDialog() noexcept
{
  delete ui;
}

ribi::QtQuestionDialog * ribi::CreateQtQuestionDialog(const Question& question)
{
  if (const auto q = dynamic_cast<const OpenQuestion*>(&question))
  {
    return new QtOpenQuestionDialog(*q);
  }
  if (const auto q = dynamic_cast<const MultipleChoiceQuestion*>(&question))
  {
    return new QtMultipleChoiceQuestionDialog(*q);
  }
  throw std::logic_error("CreateQtQuestionDialog: unknown question type");
}

void ribi::QtHometrainerMainDialog::DisplayScore()
{
  ui->label_score->setText(m_dialog->GetScoreText().c_str());
}

void ribi::QtHometrainerMainDialog::keyPressEvent(QKeyEvent* event)
{
  if (event->key() == Qt::Key_Escape) { close(); return; }
  QDialog::keyPressEvent(event);
}

void ribi::QtHometrainerMainDialog::NewQuestion()
{
  m_timer->stop();
  ui->button_next->setVisible(false);
  SetQuestion(m_dialog->GetCurrentQuestion());
}

void ribi::QtHometrainerMainDialog::on_button_next_clicked()
{
  NewQuestion();
}

void ribi::QtHometrainerMainDialog::on_button_quit_clicked()
{
  close();
}

void ribi::QtHometrainerMainDialog::OnSubmitted(const bool is_correct)
{
  m_dialog->RegisterAnswer(is_correct);
  DisplayScore();
  if (m_dialog->IsTest())
  {
    if (m_dialog->IsDone())
    {
      if (isVisible())
      {
        QMessageBox::information(this, "Hometrainer",
          QString("Test done. Your results are saved to '")
          + m_dialog->GetTestSession()->m_results_filename.c_str() + "'"
        );
      }
      close();
      return;
    }
    //Go to the next question directly, after the signal is handled
    m_timer->start(0);
    return;
  }
  //Show the feedback for a while
  ui->button_next->setVisible(true);
  m_timer->start(is_correct ? 1000 : 5000);
}

void ribi::QtHometrainerMainDialog::SetQuestion(const Question& question)
{
  if (m_qtdialog)
  {
    ui->layout_contents->removeWidget(m_qtdialog);
    m_qtdialog->deleteLater();
    m_qtdialog = nullptr;
  }
  m_qtdialog = CreateQtQuestionDialog(question);
  m_qtdialog->setParent(ui->contents_here);
  m_qtdialog->setWindowFlags(Qt::Widget);
  m_qtdialog->setFont(font());
  m_qtdialog->SetShowFeedback(!m_dialog->IsTest());
  QObject::connect(m_qtdialog, &QtQuestionDialog::signal_submitted,
    this, &QtHometrainerMainDialog::OnSubmitted);
  ui->layout_contents->addWidget(m_qtdialog);
  m_qtdialog->show();
  m_qtdialog->setFocus();
  DisplayScore();
}

void ribi::TestQtHometrainerMainDialog() noexcept
{
  std::vector<std::shared_ptr<const Question>> v;
  v.push_back(std::make_shared<MultipleChoiceQuestion>("-,1+1=,2,1,3"));
  v.push_back(OpenQuestionFactory().Create("-,1+1=,2/Two/two").Clone());

  //Exercise
  {
    const auto dialog = std::make_shared<HometrainerMainDialog>(v);
    QtHometrainerMainDialog d(dialog);
    assert(d.GetQuestionDialog());
    if (auto q = dynamic_cast<QtOpenQuestionDialog*>(d.GetQuestionDialog()))
    {
      q->SetAnswer("2");
      q->Submit();
    }
    else if (auto r = dynamic_cast<QtMultipleChoiceQuestionDialog*>(d.GetQuestionDialog()))
    {
      r->SelectOption(0);
      r->Submit();
    }
    assert(d.GetDialog().GetNumberCorrect() + d.GetDialog().GetNumberIncorrect() == 1);
  }
  //Test
  {
    const std::string filename = "qthometrainermaindialog_test_tmp.txt";
    HometrainerTestSession session;
    session.m_username = "Jane Doe";
    session.m_key = 42;
    session.m_results_filename = filename;
    const auto dialog = std::make_shared<HometrainerMainDialog>(v, session);
    QtHometrainerMainDialog d(dialog);
    for (int i = 0; i != 2; ++i)
    {
      if (auto q = dynamic_cast<QtOpenQuestionDialog*>(d.GetQuestionDialog()))
      {
        q->SetAnswer("two");
        q->Submit();
      }
      else if (auto r = dynamic_cast<QtMultipleChoiceQuestionDialog*>(d.GetQuestionDialog()))
      {
        r->SelectOption(0);
        r->Submit();
      }
      //Process the timer that sets the next question
      QTimer::singleShot(0, [](){});
      QCoreApplication::processEvents();
    }
    assert(d.GetDialog().IsDone());
    assert(IsRegularFile(filename));
    std::remove(filename.c_str());
  }
}
