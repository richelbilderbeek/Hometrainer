#include "qtopenquestiondialog.h"

#include <cassert>

#include "openquestion.h"
#include "openquestionfactory.h"
#include "ui_qtopenquestiondialog.h"

ribi::QtOpenQuestionDialog::QtOpenQuestionDialog(
  const OpenQuestion& open_question,
  QWidget *parent)
  : QtQuestionDialog(parent),
    ui(new Ui::QtOpenQuestionDialog),
    m_dialog{open_question}
{
  ui->setupUi(this);
  DisplayImage(ui->image, open_question);
  ui->stackedWidget->setCurrentWidget(ui->page_question);
  ui->label_question->setText(open_question.GetQuestion().c_str());
  ui->label_question_again->setText(open_question.GetQuestion().c_str());
  //Only show the first correct answer, the others are mostly spelling variants
  ui->label_answer->setText(
    QString("Correct answer:\n") + open_question.GetCorrectAnswers()[0].c_str()
  );
  //Pressing enter submits the answer
  QObject::connect(ui->edit_answer, &QLineEdit::returnPressed, this, &QtOpenQuestionDialog::Submit);
  ui->edit_answer->setFocus();
}

ribi::QtOpenQuestionDialog::~QtOpenQuestionDialog() noexcept
{
  delete ui;
}

void ribi::QtOpenQuestionDialog::on_button_submit_clicked() noexcept
{
  Submit();
}

void ribi::QtOpenQuestionDialog::on_edit_answer_textChanged(const QString &arg1)
{
  m_dialog.SetAnswerInProgress(arg1.toStdString());
}

void ribi::QtOpenQuestionDialog::SetAnswer(const std::string& answer)
{
  ui->edit_answer->setText(answer.c_str());
}

void ribi::QtOpenQuestionDialog::Submit()
{
  if (m_dialog.HasSubmitted()) return;
  //An empty answer is not submitted
  if (ui->edit_answer->text().trimmed().isEmpty()) return;
  m_dialog.Submit(ui->edit_answer->text().toStdString());
  const bool is_correct = m_dialog.IsAnswerCorrect();
  if (GetShowFeedback())
  {
    ui->stackedWidget->setCurrentWidget(is_correct
      ? ui->page_correct
      : ui->page_incorrect
    );
  }
  else
  {
    ui->button_submit->setEnabled(false);
    ui->edit_answer->setEnabled(false);
  }
  emit signal_submitted(is_correct);
}

void ribi::TestQtOpenQuestionDialog() noexcept
{
  //Correct answer
  {
    QtOpenQuestionDialog d(OpenQuestionFactory().Create("-,1+1=,2/two"));
    bool was_correct = false;
    int n_signals = 0;
    QObject::connect(&d, &QtQuestionDialog::signal_submitted,
      [&](const bool is_correct) { was_correct = is_correct; ++n_signals; }
    );
    d.Submit(); //Empty answers are ignored
    assert(n_signals == 0);
    d.SetAnswer("two");
    assert(d.GetDialog().ToStr() == "-,1+1=,2/two");
    d.Submit();
    assert(n_signals == 1);
    assert(was_correct);
    d.Submit(); //Second submit is ignored
    assert(n_signals == 1);
  }
  //Incorrect answer, without feedback
  {
    QtOpenQuestionDialog d(OpenQuestionFactory().Create("nonexisting.png,1+1=,2"));
    d.SetShowFeedback(false);
    bool was_correct = true;
    QObject::connect(&d, &QtQuestionDialog::signal_submitted,
      [&](const bool is_correct) { was_correct = is_correct; }
    );
    d.SetAnswer("3");
    d.Submit();
    assert(!was_correct);
    assert(d.GetDialog().HasSubmitted());
  }
}
