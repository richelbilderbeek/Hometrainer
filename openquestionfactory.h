#ifndef OPENQUESTIONFACTORY_H
#define OPENQUESTIONFACTORY_H


#include <string>
#include <vector>

namespace ribi {

struct OpenQuestion;

struct OpenQuestionFactory
{
  OpenQuestionFactory();

  OpenQuestion Create(const std::string& question) const;

  OpenQuestion Create(
      const std::string& filename,
      const std::string& question,
      const std::vector<std::string>& answers
  ) const noexcept;


  std::string GetExampleOpenQuestionString() const noexcept { return "-,1+1=,2/two/Two"; }
  std::vector<std::string> GetInvalidOpenQuestionStrings() const noexcept;
  std::vector<OpenQuestion> GetTestOpenQuestions() const noexcept;
  std::vector<std::string> GetValidOpenQuestionStrings() const noexcept;

  private:
};

void TestOpenQuestionFactory() noexcept;

} //~namespace ribi

#endif // OPENQUESTIONFACTORY_H
