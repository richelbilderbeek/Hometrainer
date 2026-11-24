#include "openquestiondialogfactory.h"

#include <cassert>

#include "openquestiondialog.h"
#include "openquestionfactory.h"

ribi::OpenQuestionDialogFactory::OpenQuestionDialogFactory()
{

}

ribi::OpenQuestionDialog ribi::OpenQuestionDialogFactory::Create(
  const std::string& s
) const
{
  return Create(OpenQuestionFactory().Create(s));
}

ribi::OpenQuestionDialog ribi::OpenQuestionDialogFactory::Create(
    const std::string& filename,
    const std::string& question,
    const std::vector<std::string>& answers
) const
{
  return Create(OpenQuestionFactory().Create(filename,question,answers));
}

ribi::OpenQuestionDialog ribi::OpenQuestionDialogFactory::Create(
  const OpenQuestion& open_question
) const
{
  return OpenQuestionDialog(open_question);
}

std::vector<ribi::OpenQuestionDialog>
  ribi::OpenQuestionDialogFactory::GetTestOpenQuestionDialogs() const noexcept
{
  std::vector<OpenQuestionDialog> v;
  for (const auto& open_question: OpenQuestionFactory().GetTestOpenQuestions())
  {
    v.push_back(Create(open_question));
  }
  return v;
}

void ribi::TestOpenQuestionDialogFactory() noexcept
{
  for(const std::string& s: OpenQuestionFactory().GetValidOpenQuestionStrings())
  {
    const auto d = OpenQuestionDialogFactory().Create(s);
    assert(d.ToStr() == s);
  }
}
