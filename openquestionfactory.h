#ifndef OPENQUESTIONFACTORY_H
#define OPENQUESTIONFACTORY_H


#include <string>
#include <vector>





#include <boost/shared_ptr.hpp>


namespace ribi {

struct OpenQuestion;

struct OpenQuestionFactory
{
  OpenQuestionFactory();

  boost::shared_ptr<OpenQuestion> Create(const std::string& question) const;

  boost::shared_ptr<OpenQuestion>
    Create(
      const std::string& filename,
      const std::string& question,
      const std::vector<std::string>& answers
  ) const noexcept;


  std::string GetExampleOpenQuestionString() const noexcept { return "-,1+1=,2/two/Two"; }
  std::vector<std::string> GetInvalidOpenQuestionStrings() const noexcept;
  std::vector<boost::shared_ptr<OpenQuestion>> GetTestOpenQuestions() const noexcept;
  std::vector<std::string> GetValidOpenQuestionStrings() const noexcept;

  static std::string GetVersion() noexcept;
  static std::vector<std::string> GetVersionHistory() noexcept;

  private:
};

void TestOpenQuestionFactory() noexcept;

} //~namespace ribi

#endif // OPENQUESTIONFACTORY_H
