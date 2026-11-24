#include "qtquestiondialog.h"

#include <cassert>

#include <QEvent>
#include <QLabel>
#include <QPixmap>
#include <QScrollArea>
#include <QVBoxLayout>

#include "hometrainerhelper.h"
#include "qthometrainerhelper.h"
#include "question.h"
#include "questiondialog.h"

ribi::QtQuestionDialog::QtQuestionDialog(
  QWidget *parent)
  : QDialog(parent),
    m_image_filename{},
    m_show_feedback{true}
{

}

void ribi::QtQuestionDialog::DisplayImage(QLabel * const label, const Question& question)
{
  assert(label);
  m_image_filename.clear();
  if (!question.HasImage())
  {
    label->setVisible(false);
    return;
  }
  const QPixmap pixmap(QString::fromStdString(question.GetFilename()));
  if (pixmap.isNull())
  {
    label->setText("File not found: " + QString::fromStdString(question.GetFilename()));
    return;
  }
  m_image_filename = QString::fromStdString(question.GetFilename());
  const int max_height = 300;
  label->setPixmap(pixmap.height() > max_height
    ? pixmap.scaledToHeight(max_height, Qt::SmoothTransformation)
    : pixmap
  );
  label->installEventFilter(this);
}

bool ribi::QtQuestionDialog::eventFilter(QObject * object, QEvent * event)
{
  if (event->type() == QEvent::MouseButtonRelease && !m_image_filename.isEmpty())
  {
    ShowImageFullSize(m_image_filename, this);
    return true;
  }
  return QDialog::eventFilter(object, event);
}

void ribi::ShowImageFullSize(const QString& filename, QWidget * const parent)
{
  QDialog * const d = new QDialog(parent);
  d->setWindowTitle(GetFileBasename(filename.toStdString()).c_str());
  QLabel * const label = new QLabel;
  label->setPixmap(QPixmap(filename));
  QScrollArea * const area = new QScrollArea;
  area->setWidget(label);
  QVBoxLayout * const layout = new QVBoxLayout(d);
  layout->addWidget(area);
  d->resize(800, 600);
  ShowDialog(d);
}
