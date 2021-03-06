#include "openquestionfactory.h"

#include <cassert>
#include <stdexcept>





#include <boost/algorithm/string/split.hpp>
#include <boost/make_shared.hpp>

#include "container.h"
#include "openquestion.h"



ribi::OpenQuestionFactory::OpenQuestionFactory()
{

}

boost::shared_ptr<ribi::OpenQuestion>
  ribi::OpenQuestionFactory::Create(const std::string& s
) const
{
  if (s.empty())
  {
    throw std::logic_error("An open question must contain text");
  }
  if (s[0] == ',')
  {
    throw std::logic_error("An open question must not start with a comma");
  }
  if (s[s.size() - 1] == ',')
  {
    throw std::logic_error("An open question must not end with a comma");
  }
  if (s.find(",,") != std::string::npos)
  {
    throw std::logic_error("An open question cannot contain two consecutive commas");
  }
  const auto v = Container().SeperateString(s,',');
  if (v.size() != 3)
  {
    throw std::logic_error("An open question has exactly three comma-seperated elements");
  }
  const auto filename = v[0];
  const auto question = v[1];
  const auto answers = Container().SeperateString(v[2],'/');
  if (answers.size() == 0)
  {
    throw std::logic_error("An open question has at least one correct answer");
  }
  return Create(filename,question,answers);

}

boost::shared_ptr<ribi::OpenQuestion>
  ribi::OpenQuestionFactory::Create(
    const std::string& filename,
    const std::string& question,
    const std::vector<std::string>& answers
) const noexcept
{
  boost::shared_ptr<OpenQuestion> openquestion(
    new OpenQuestion(
      filename,question,answers
    )
  );
  return openquestion;
  //assert(!filename.empty() && "Filename must not be empty");
  //assert(FileExists(filename) == true && "File must exists");
  //assert(!question.empty() && "OpenQuestion must not be empty");
  //assert(!correct_answers.empty() && "Correct answer must not be empty");
}

std::vector<std::string>
  ribi::OpenQuestionFactory::GetInvalidOpenQuestionStrings() const noexcept
{
  return {
    "-,1+1=,2,3", //Incorrect options are
    "-,1+1=",   //No answer
    "-",        //No question
    "tmp.png",  //No question
    "",         //Nothing
    ",tmp.png,1+1=,2", //Start with comma
    "tmp.png,,1+1=,2", //Two consecutive comma's
    "tmp.png,1+1=,,2", //Two consecutive comma's
    "tmp.png,1+1=,2,", //Two consecutive comma's
    "tmp.png,1+1=,2,", //End with comma
    ",tmp.png,1+1=,2,", //Start and end with comma
    ",,tmp.png,1+1=,2,",
    ",tmp.png,,1+1=,2,",
    ",tmp.png,1+1=,,2,",
    ",tmp.png,1+1=,2,,",
    ",",
    ",,",
    ",,,",
    ",,,,",
    ",,,,,",
    ",,,,,,"
  };
}

std::vector<boost::shared_ptr<ribi::OpenQuestion>>
  ribi::OpenQuestionFactory::GetTestOpenQuestions() const noexcept
{
  std::vector<boost::shared_ptr<OpenQuestion>> v;
  for (const auto& s: GetValidOpenQuestionStrings())
  {
    v.push_back(Create(s));
  }
  return v;
}

std::vector<std::string>
  ribi::OpenQuestionFactory::GetValidOpenQuestionStrings() const noexcept
{
  return {
    "-,1+1=,2",
    "tmp.png,1+1=,2/Two",
    "-,1+1=,Two/2/two"
  };
}

std::string ribi::OpenQuestionFactory::GetVersion() noexcept
{
  return "1.0";
}

std::vector<std::string> ribi::OpenQuestionFactory::GetVersionHistory() noexcept
{
  return {
    "2014-06-05: version 1.0: initial version"
  };
}

void ribi::TestOpenQuestionFactory() noexcept
{
  
  OpenQuestionFactory f;
  try
  {
    const auto q = f.Create(f.GetExampleOpenQuestionString());
    assert(q);
  }
  catch (std::exception& e)
  {
    assert("OpenQuestionFactory::GetExampleOpenQuestion()"
        && "must yield a valid OpenQuestion");
  }
  //Test valid multiple choice questions for validity
  {
    for (const std::string& s: f.GetValidOpenQuestionStrings())
    {
      try
      {
        const auto q = f.Create(s);
        assert(q); //To make the compiler happy
        //OK
      }
      catch (std::exception& e)
      {
        assert(!"Valid questions must be accepted");
      }
    }
  }
  //Test invalid multiple choice questions for invalidity
  {
    for (const std::string& s: f.GetInvalidOpenQuestionStrings())
    {
      try
      {
        const auto q = f.Create(s);
        assert(!"Invalid questions must be rejected");
        assert(q); //To make the compiler happy
      }
      catch (std::exception& e)
      {
        //OK
      }
    }
  }
}
