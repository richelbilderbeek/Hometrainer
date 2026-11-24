#include "qthometrainerhelper.h"

#include <cassert>
#include <stdexcept>

#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>

#include "hometrainerhelper.h"
#include "hometrainerresources.h"

void ribi::AskInt(
  QWidget * const parent,
  const QString& label,
  const int value,
  const std::function<void(int)>& on_selected
)
{
  QInputDialog * const d = new QInputDialog(parent);
  d->setAttribute(Qt::WA_DeleteOnClose);
  d->setWindowTitle("Hometrainer");
  d->setInputMode(QInputDialog::IntInput);
  d->setIntRange(-2147483647, 2147483647);
  d->setIntValue(value);
  d->setLabelText(label);
  QObject::connect(d, &QInputDialog::intValueSelected, d, on_selected);
  d->open();
}

void ribi::AskText(
  QWidget * const parent,
  const QString& label,
  const std::function<void(const QString&)>& on_selected
)
{
  QInputDialog * const d = new QInputDialog(parent);
  d->setAttribute(Qt::WA_DeleteOnClose);
  d->setWindowTitle("Hometrainer");
  d->setInputMode(QInputDialog::TextInput);
  d->setLabelText(label);
  QObject::connect(d, &QInputDialog::textValueSelected, d, on_selected);
  d->open();
}

void ribi::DownloadFile(const std::string& filename)
{
  QFile f(QString::fromStdString(filename));
  if (!f.open(QIODevice::ReadOnly)) return;
  QFileDialog::saveFileContent(f.readAll(), QString::fromStdString(GetFileBasename(filename)));
}

void ribi::OpenExercise(
  QWidget * const parent,
  const std::function<void(const std::string&)>& on_opened
)
{
  const QString caption = "Please select a Hometrainer exercise";
  const QString filter = "Hometrainer exercises (*.txt);;All files (*)";
  if (!IsWebAssembly())
  {
    OpenFile(parent, caption, filter, on_opened);
    return;
  }
  //In a web browser, the example exercises are in the current folder
  const QString upload = "Upload an exercise from your computer...";
  QStringList items;
  for (const auto& s: HometrainerResources().GetExercises()) { items.push_back(QString::fromStdString(s)); }
  items.push_back(upload);

  QInputDialog * const d = new QInputDialog(parent);
  d->setAttribute(Qt::WA_DeleteOnClose);
  d->setWindowTitle("Hometrainer");
  d->setLabelText(caption);
  d->setComboBoxItems(items);
  d->setComboBoxEditable(false);
  QObject::connect(d, &QInputDialog::textValueSelected, d,
    [parent, caption, filter, upload, on_opened](const QString& item)
    {
      if (item == upload)
      {
        OpenFile(parent, caption, filter, on_opened);
        return;
      }
      on_opened(item.toStdString());
    }
  );
  d->open();
}

void ribi::OpenFile(
  QWidget * const parent,
  const QString& caption,
  const QString& filter,
  const std::function<void(const std::string&)>& on_opened
)
{
  if (!IsWebAssembly())
  {
    const QString filename = QFileDialog::getOpenFileName(parent, caption, QString(), filter);
    if (!filename.isEmpty()) on_opened(filename.toStdString());
    return;
  }
  //Upload the file to the current folder
  QFileDialog::getOpenFileContent(filter,
    [on_opened](const QString& filename, const QByteArray& content)
    {
      if (filename.isEmpty()) return;
      const QString local_filename = QString::fromStdString(GetFileBasename(filename.toStdString()));
      QFile f(local_filename);
      if (!f.open(QIODevice::WriteOnly)) return;
      f.write(content);
      f.close();
      on_opened(local_filename.toStdString());
    }
  );
}

void ribi::OpenFiles(
  QWidget * const parent,
  const QString& caption,
  const QString& filter,
  const std::function<void(const std::vector<std::string>&)>& on_opened
)
{
  if (!IsWebAssembly())
  {
    const QStringList filenames = QFileDialog::getOpenFileNames(parent, caption, QString(), filter);
    if (filenames.isEmpty()) return;
    std::vector<std::string> v;
    for (const auto& s: filenames) { v.push_back(s.toStdString()); }
    on_opened(v);
    return;
  }
  OpenFile(parent, caption, filter,
    [on_opened](const std::string& filename) { on_opened( { filename } ); }
  );
}

void ribi::SaveFile(
  QWidget * const parent,
  const QString& caption,
  const std::string& default_filename,
  const QString& filter,
  const std::vector<std::string>& lines,
  const std::function<void(const std::string&)>& on_saved
)
{
  if (IsWebAssembly())
  {
    //Save it locally, then download it
    const std::string filename = GetFileBasename(default_filename);
    SaveContainer(lines, filename);
    DownloadFile(filename);
    on_saved(filename);
    return;
  }
  const QString filename = QFileDialog::getSaveFileName(
    parent, caption, QString::fromStdString(default_filename), filter
  );
  if (filename.isEmpty()) return;
  try
  {
    SaveContainer(lines, filename.toStdString());
    on_saved(filename.toStdString());
  }
  catch (std::exception& e)
  {
    ShowMessage(parent, e.what(), QMessageBox::Warning);
  }
}

void ribi::ShowDialog(QDialog * const dialog)
{
  assert(dialog);
  dialog->setAttribute(Qt::WA_DeleteOnClose);
  dialog->setWindowModality(Qt::ApplicationModal);
  dialog->show();
}

void ribi::ShowMessage(
  QWidget * const parent,
  const QString& text,
  const QMessageBox::Icon icon
)
{
  QMessageBox * const box = new QMessageBox(icon, "Hometrainer", text, QMessageBox::Ok, parent);
  box->setAttribute(Qt::WA_DeleteOnClose);
  box->open();
}
