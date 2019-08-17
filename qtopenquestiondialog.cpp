



#include "qtopenquestiondialog.h"

#include <boost/lambda/lambda.hpp>

#include "fileio.h"
#include "openquestion.h"
#include "openquestiondialog.h"

#include "ui_qtopenquestiondialog.h"

#include <QFile>


ribi::QtOpenQuestionDialog::QtOpenQuestionDialog(QWidget *parent)
  : QtQuestionDialog(parent),
    ui(new Ui::QtOpenQuestionDialog),
    m_openquestiondialog{}
{
  ui->setupUi(this);

}

ribi::QtOpenQuestionDialog::~QtOpenQuestionDialog() noexcept
{
  delete ui;
}

boost::shared_ptr<const ribi::QuestionDialog> ribi::QtOpenQuestionDialog::GetDialog() const
{
  return m_openquestiondialog;
}

void ribi::QtOpenQuestionDialog::OnOpenQuestionDialogChanged(OpenQuestionDialog * const open_question_dialog) noexcept
{
  assert(open_question_dialog);

  ui->edit_answer->setText(open_question_dialog->GetAnswerInProgress().c_str());

  OnOpenQuestionChanged(open_question_dialog->GetOpenQuestion());
}

void ribi::QtOpenQuestionDialog::OnOpenQuestionChanged(const boost::shared_ptr<OpenQuestion>& open_question) noexcept
{
  assert(open_question);
  if (fileio::FileIo().IsRegularFile(open_question->GetFilename().c_str()))
  {
    ui->image->setPixmap(QPixmap(open_question->GetFilename().c_str()));
  }

  ui->stackedWidget->setCurrentWidget(ui->page_question);
  ui->label_question->setText(open_question->GetQuestion().c_str());
  ui->label_question_again->setText(open_question->GetQuestion().c_str());
  ui->label_answer->setText(open_question->GetCorrectAnswers()[0].c_str());

}

void ribi::QtOpenQuestionDialog::SetDialog(const boost::shared_ptr<QuestionDialog>& dialog)
{
  const bool verbose{false};

  const boost::shared_ptr<OpenQuestionDialog> openquestiondialog
    = boost::dynamic_pointer_cast<OpenQuestionDialog>(dialog);
  if (!openquestiondialog)
  {
    std::stringstream s;
    s << "QtOpenQuestionDialog::SetDialog: not an OpenQuestionDialog '" << dialog->ToStr() << "'\n";
    //this->ui->stackedWidget->setVisible(false);
    //this->ui->image->setVisible(false);
    return;
  }

  //this->ui->stackedWidget->setVisible(true);
  //this->ui->image->setVisible(true);

  assert(openquestiondialog);
  if (m_openquestiondialog == openquestiondialog)
  {
    return;
  }
  if (verbose)
  {
    std::stringstream s;
    s << "Setting openquestiondialog '" << openquestiondialog->ToStr() << "'\n";
  }
  const auto open_question_after = openquestiondialog->GetOpenQuestion();

  bool open_question_changed  = true;

  if (m_openquestiondialog)
  {
    const auto open_question_before = m_openquestiondialog->GetOpenQuestion();

    open_question_changed = open_question_before != open_question_after;

  }

  //Replace m_example by the new one
  m_openquestiondialog = openquestiondialog;

  assert(m_openquestiondialog->GetOpenQuestion() == open_question_after);

  assert(openquestiondialog == m_openquestiondialog);
}

void ribi::QtOpenQuestionDialog::OnQuit() noexcept
{
  close();
}

void ribi::QtOpenQuestionDialog::OnSubmit(const bool /*was_answer_correct*/) noexcept
{
  //Do the change in GUI as on_button_submit_clicked
  const bool is_correct = m_openquestiondialog->IsAnswerCorrect();
  ui->stackedWidget->setCurrentWidget(is_correct
    ? ui->page_correct
    : ui->page_incorrect
  );
}

void ribi::QtOpenQuestionDialog::on_button_submit_clicked() noexcept
{
  assert(m_openquestiondialog);
  assert(!m_openquestiondialog->HasSubmitted());
  m_openquestiondialog->Submit(this->ui->edit_answer->text().toStdString());

  const bool is_correct = m_openquestiondialog->IsAnswerCorrect();
  ui->stackedWidget->setCurrentWidget(is_correct
    ? ui->page_correct
    : ui->page_incorrect
  );
}



void ribi::QtOpenQuestionDialog::on_edit_answer_textChanged(const QString &arg1)
{
  m_openquestiondialog->SetAnswerInProgress(arg1.toStdString());
}

void ribi::QtOpenQuestionDialog::SetOpenQuestionDialog(
  const boost::shared_ptr<OpenQuestionDialog>& dialog
) noexcept
{
  this->m_openquestiondialog = dialog;
  //SetOpenQuestionDialog(dialog);
}
