#include "qtmultiplechoicequestiondialog.h"

#include <algorithm>
#include <cassert>

#include <QRadioButton>

#include "multiplechoicequestion.h"
#include "ui_qtmultiplechoicequestiondialog.h"

ribi::QtMultipleChoiceQuestionDialog::QtMultipleChoiceQuestionDialog(
  const MultipleChoiceQuestion& question,
  QWidget *parent)
  : QtQuestionDialog(parent),
    ui(new Ui::QtMultipleChoiceQuestionDialog),
    m_buttons{},
    m_dialog(question)
{
  ui->setupUi(this);
  const MultipleChoiceQuestion& q = m_dialog.GetMultipleChoiceQuestion();
  DisplayImage(ui->image, q);
  ui->stackedWidget->setCurrentWidget(ui->page_question);
  ui->label_question->setText(q.GetQuestion().c_str());
  ui->label_question_again->setText(q.GetQuestion().c_str());
  ui->label_answer->setText(QString("Correct answer:\n") + q.GetAnswer().c_str());

  for (const std::string& option: q.GetOptions())
  {
    QRadioButton * const button = new QRadioButton(option.c_str());
    ui->layout_options->addWidget(button);
    m_buttons.push_back(button);
  }
}

ribi::QtMultipleChoiceQuestionDialog::~QtMultipleChoiceQuestionDialog() noexcept
{
  delete ui;
}

void ribi::QtMultipleChoiceQuestionDialog::on_button_submit_clicked() noexcept
{
  Submit();
}

void ribi::QtMultipleChoiceQuestionDialog::SelectOption(const int index)
{
  assert(index >= 0);
  assert(index < static_cast<int>(m_buttons.size()));
  m_buttons[static_cast<std::size_t>(index)]->setChecked(true);
}

void ribi::QtMultipleChoiceQuestionDialog::Submit()
{
  if (m_dialog.HasSubmitted()) return;
  const auto i = std::find_if(m_buttons.begin(), m_buttons.end(),
    [](const QRadioButton * const button) { return button->isChecked(); }
  );
  //Nothing is selected yet
  if (i == m_buttons.end()) return;

  const int index = static_cast<int>(std::distance(m_buttons.begin(), i));
  m_dialog.SetAnswerInProgress(index);
  m_dialog.Submit(std::to_string(index));

  const bool is_correct = m_dialog.IsAnswerCorrect();
  if (GetShowFeedback())
  {
    ui->stackedWidget->setCurrentWidget(is_correct
      ? ui->page_correct
      : ui->page_incorrect);
  }
  else
  {
    ui->button_submit->setEnabled(false);
    ui->group_options->setEnabled(false);
  }
  emit signal_submitted(is_correct);
}

void ribi::TestQtMultipleChoiceQuestionDialog() noexcept
{
  const MultipleChoiceQuestion question("-,1+1=,2,1,3,4,5,6,7,8,9");
  const auto& options = question.GetOptions();
  const int index_correct = static_cast<int>(
    std::distance(options.begin(), std::find(options.begin(), options.end(), "2"))
  );
  //Correct answer
  {
    QtMultipleChoiceQuestionDialog d(question);
    bool was_correct = false;
    int n_signals = 0;
    QObject::connect(&d, &QtQuestionDialog::signal_submitted,
      [&](const bool is_correct) { was_correct = is_correct; ++n_signals; }
    );
    d.Submit(); //Nothing selected, is ignored
    assert(n_signals == 0);
    d.SelectOption(index_correct);
    d.Submit();
    assert(n_signals == 1);
    assert(was_correct);
  }
  //Incorrect answer
  {
    QtMultipleChoiceQuestionDialog d(question);
    d.SetShowFeedback(false);
    bool was_correct = true;
    QObject::connect(&d, &QtQuestionDialog::signal_submitted,
      [&](const bool is_correct) { was_correct = is_correct; }
    );
    d.SelectOption(index_correct == 0 ? 1 : 0);
    d.Submit();
    assert(!was_correct);
  }
}
