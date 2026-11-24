#ifndef QTQUESTIONDIALOG_H
#define QTQUESTIONDIALOG_H

#include <QDialog>

class QLabel;

namespace ribi {

struct Question;
struct QuestionDialog;

///Qt dialog for QuestionDialog
class QtQuestionDialog : public QDialog
{
  Q_OBJECT

public:
  explicit QtQuestionDialog(QWidget *parent = nullptr);
  virtual ~QtQuestionDialog() noexcept {}

  virtual const QuestionDialog& GetDialog() const noexcept = 0;

  ///Will the user be shown if the answer is correct?
  ///This is not done in a test
  bool GetShowFeedback() const noexcept { return m_show_feedback; }

  ///Set if the user will be shown if the answer is correct.
  ///This is not done in a test
  void SetShowFeedback(const bool show_feedback) noexcept { m_show_feedback = show_feedback; }

signals:
  ///Emitted when the user has submitted an answer
  void signal_submitted(const bool is_correct);

protected:
  ///Display the image of a question in a QLabel.
  ///If the image cannot be found, a message is displayed instead.
  ///Clicking the QLabel shows the image full size
  void DisplayImage(QLabel * const label, const Question& question);

  bool eventFilter(QObject * object, QEvent * event) override;

private:
  ///The filename of the image displayed
  QString m_image_filename;

  bool m_show_feedback;
};

///Show an image full size, in a scrollable dialog
void ShowImageFullSize(const QString& filename, QWidget * const parent = nullptr);

} //~namespace ribi

#endif // QTQUESTIONDIALOG_H
