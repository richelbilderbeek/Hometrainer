#ifndef QTQUESTIONDIALOG_H
#define QTQUESTIONDIALOG_H

#include "qthideandshowdialog.h"

namespace ribi {

struct Question;
struct QuestionDialog;

///Qt dialog for QuestionDialog
struct QtQuestionDialog : public QtHideAndShowDialog
{
  explicit QtQuestionDialog(QWidget *parent = 0);
  virtual ~QtQuestionDialog() noexcept {}

  virtual boost::shared_ptr<const QuestionDialog> GetDialog() const = 0;
  virtual void SetDialog(const boost::shared_ptr<QuestionDialog>& dialog) = 0;

  private:
  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

} //~namespace ribi

#endif // CPPQTQUESTIONDIALOG_H
