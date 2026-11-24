#ifndef QTHOMETRAINERHELPER_H
#define QTHOMETRAINERHELPER_H

#include <functional>
#include <string>
#include <vector>

#include <QMessageBox>
#include <QString>

class QDialog;
class QWidget;

namespace ribi {

///Helper functions for the Qt dialogs, that work both on the desktop
///and in a web browser (the WebAssembly version).
///
///In a web browser, dialogs cannot block (that is, QDialog::exec cannot be used)
///and files are uploaded from and downloaded to the computer of the user.
///Hence, these functions do not block and call a function when done.

///Is this the WebAssembly version, that runs in a web browser?
constexpr bool IsWebAssembly() noexcept
{
  #ifdef Q_OS_WASM
  return true;
  #else
  return false;
  #endif
}

///Ask the user for a whole number.
///'on_selected' is only called if the user clicks OK
void AskInt(
  QWidget * const parent,
  const QString& label,
  const int value,
  const std::function<void(int)>& on_selected
);

///Ask the user for a text.
///'on_selected' is only called if the user clicks OK
void AskText(
  QWidget * const parent,
  const QString& label,
  const std::function<void(const QString&)>& on_selected
);

///Download a file to the computer of the user
///(only possible in the WebAssembly version)
void DownloadFile(const std::string& filename);

///Let the user select an exercise.
///On the desktop, the user selects a file.
///In a web browser, the user selects an example exercise
///or uploads an exercise.
///'on_opened' is called with the filename of the exercise
void OpenExercise(
  QWidget * const parent,
  const std::function<void(const std::string&)>& on_opened
);

///Let the user select a file.
///In a web browser, the file is uploaded to the current folder.
///'on_opened' is called with the filename
void OpenFile(
  QWidget * const parent,
  const QString& caption,
  const QString& filter,
  const std::function<void(const std::string&)>& on_opened
);

///Let the user select one or more files.
///In a web browser, only one file can be uploaded at a time.
///'on_opened' is called with the filenames
void OpenFiles(
  QWidget * const parent,
  const QString& caption,
  const QString& filter,
  const std::function<void(const std::vector<std::string>&)>& on_opened
);

///Let the user save lines to a file.
///On the desktop, the user selects the filename,
///in a web browser, the file is downloaded.
///'default_filename' is the suggested filename, possibly with a path.
///'on_saved' is called with the filename when saved
void SaveFile(
  QWidget * const parent,
  const QString& caption,
  const std::string& default_filename,
  const QString& filter,
  const std::vector<std::string>& lines,
  const std::function<void(const std::string&)>& on_saved = [](const std::string&) {}
);

///Show a dialog without blocking. The dialog is deleted when closed
void ShowDialog(QDialog * const dialog);

///Show a message without blocking
void ShowMessage(
  QWidget * const parent,
  const QString& text,
  const QMessageBox::Icon icon = QMessageBox::Information
);

} //~namespace ribi

#endif // QTHOMETRAINERHELPER_H
