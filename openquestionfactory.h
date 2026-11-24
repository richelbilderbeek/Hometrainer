#ifndef OPENQUESTIONFACTORY_H
#define OPENQUESTIONFACTORY_H

#include <string>
#include <vector>

namespace ribi {

struct OpenQuestion;

struct OpenQuestionFactory
{
  OpenQuestionFactory();

  ///Create an OpenQuestion from a line of a Hometrainer file,
  ///for example '-,1+1=,2/two/Two'.
  ///Throws std::logic_error if this is not an open question
  OpenQuestion Create(const std::string& question) const;

  ///Throws std::logic_error if the OpenQuestion cannot be constructed
  OpenQuestion Create(
      const std::string& filename,
      const std::string& question,
      const std::vector<std::string>& answers
  ) const;

  std::string GetExampleOpenQuestionString() const noexcept { return "-,1+1=,2/two/Two"; }
  std::vector<std::string> GetInvalidOpenQuestionStrings() const noexcept;
  std::vector<OpenQuestion> GetTestOpenQuestions() const noexcept;
  std::vector<std::string> GetValidOpenQuestionStrings() const noexcept;
};

void TestOpenQuestionFactory() noexcept;

} //~namespace ribi

#endif // OPENQUESTIONFACTORY_H
