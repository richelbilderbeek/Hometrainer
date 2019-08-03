#ifndef OPENQUESTIONDIALOGFACTORY_H
#define OPENQUESTIONDIALOGFACTORY_H





#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>


namespace ribi {

struct OpenQuestion;
struct OpenQuestionDialog;

struct OpenQuestionDialogFactory final
{
  OpenQuestionDialogFactory();

  ///Throws an exception if it cannot be constructed
  boost::shared_ptr<OpenQuestionDialog> Create(
    const boost::shared_ptr<OpenQuestion>& open_question
  ) const;

  ///Throws an exception if it cannot be constructed
  boost::shared_ptr<OpenQuestionDialog>
    Create(
      const std::string& filename,
      const std::string& question,
      const std::vector<std::string>& answers
  ) const;

  ///Throws an exception if it cannot be constructed
  boost::shared_ptr<OpenQuestionDialog> Create(
    const std::string& s
  ) const;

  std::vector<boost::shared_ptr<OpenQuestionDialog>> GetTestOpenQuestionDialogs() const noexcept;

  static std::string GetVersion() noexcept;
  static std::vector<std::string> GetVersionHistory() noexcept;
};

void TestOpenQuestionDialogFactory() noexcept;

} //~namespace ribi

#endif // OPENQUESTIONDIALOGFACTORY_H
