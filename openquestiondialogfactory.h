#ifndef OPENQUESTIONDIALOGFACTORY_H
#define OPENQUESTIONDIALOGFACTORY_H





#include <string>
#include <vector>

namespace ribi {

struct OpenQuestion;
struct OpenQuestionDialog;

struct OpenQuestionDialogFactory final
{
  OpenQuestionDialogFactory();

  ///Throws an exception if it cannot be constructed
  OpenQuestionDialog Create(
    const OpenQuestion& open_question
  ) const;

  ///Throws an exception if it cannot be constructed
  OpenQuestionDialog Create(
    const std::string& filename,
    const std::string& question,
    const std::vector<std::string>& answers
  ) const;

  ///Throws an exception if it cannot be constructed
  OpenQuestionDialog Create(
    const std::string& s
  ) const;

  std::vector<OpenQuestionDialog> GetTestOpenQuestionDialogs() const noexcept;
};

void TestOpenQuestionDialogFactory() noexcept;

} //~namespace ribi

#endif // OPENQUESTIONDIALOGFACTORY_H
