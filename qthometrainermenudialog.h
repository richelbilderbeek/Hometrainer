#ifndef QTHOMETRAINERMENUDIALOG_H
#define QTHOMETRAINERMENUDIALOG_H

#include "qthideandshowdialog.h"


namespace Ui {
  class QtHometrainerMenuDialog;
}

namespace ribi {

struct HometrainerMainDialog;

class QtHometrainerMenuDialog : public QtHideAndShowDialog
{
  Q_OBJECT

public:
  explicit QtHometrainerMenuDialog(QWidget *parent = 0) noexcept;
  QtHometrainerMenuDialog(const QtHometrainerMenuDialog&) = delete;
  QtHometrainerMenuDialog& operator=(const QtHometrainerMenuDialog&) = delete;
  ~QtHometrainerMenuDialog() noexcept;

private:
  Ui::QtHometrainerMenuDialog *ui;
  boost::shared_ptr<const HometrainerMainDialog> m_main_dialog;


private slots:
  void on_button_about_clicked() noexcept;
  void on_button_quit_clicked() noexcept;
  void on_button_start_exercise_clicked() noexcept;
  void on_button_load_exercise_clicked() noexcept;
  void on_button_create_examples_clicked();
};

void TestQtHometrainerMenuDialog() noexcept;

} //~namespace ribi

#endif // QTHOMETRAINERMENUDIALOG_H
