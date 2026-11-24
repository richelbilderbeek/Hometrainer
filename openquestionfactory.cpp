#include "openquestionfactory.h"

#include <algorithm>
#include <cassert>
#include <stdexcept>

#include "hometrainerhelper.h"
#include "openquestion.h"

ribi::OpenQuestionFactory::OpenQuestionFactory()
{

}

ribi::OpenQuestion ribi::OpenQuestionFactory::Create(const std::string& s) const
{
  if (s.empty())
  {
    throw std::logic_error("An open question must contain text");
  }
  const auto v = SeperateHometrainerLine(s);
  if (v.size() != 3)
  {
    throw std::logic_error("An open question has exactly three comma-seperated elements");
  }
  if (std::find(v.begin(), v.end(), "") != v.end())
  {
    throw std::logic_error("An open question cannot have empty elements");
  }
  const auto filename = v[0];
  const auto question = v[1];
  const auto answers = SeperateString(v[2],'/');
  return Create(filename,question,answers);
}

ribi::OpenQuestion ribi::OpenQuestionFactory::Create(
    const std::string& filename,
    const std::string& question,
    const std::vector<std::string>& answers
) const
{
  return OpenQuestion(filename,question,answers);
}

std::vector<std::string>
  ribi::OpenQuestionFactory::GetInvalidOpenQuestionStrings() const noexcept
{
  return {
    "-,1+1=,2,3", //Incorrect options are for multiple choice questions
    "-,1+1=",   //No answer
    "-",        //No question
    "tmp.png",  //No question
    "",         //Nothing
    "-,1+1=,2/", //Empty answer
    ",tmp.png,1+1=,2", //Start with comma
    "tmp.png,,1+1=,2", //Two consecutive comma's
    "tmp.png,1+1=,,2", //Two consecutive comma's
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

std::vector<ribi::OpenQuestion>
  ribi::OpenQuestionFactory::GetTestOpenQuestions() const noexcept
{
  std::vector<OpenQuestion> v;
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
    "-,1+1=,Two/2/two",
    "-,1\\,5 + 1\\,5 =,3"
  };
}

void ribi::TestOpenQuestionFactory() noexcept
{
  const OpenQuestionFactory f;
  //The example must be valid
  {
    const auto q = f.Create(f.GetExampleOpenQuestionString());
    assert(q.IsCorrect("2"));
  }
  //Test valid open questions for validity
  for (const std::string& s: f.GetValidOpenQuestionStrings())
  {
    try
    {
      f.Create(s);
    }
    catch (std::exception&)
    {
      assert(!"Valid questions must be accepted");
    }
  }
  //Test invalid open questions for invalidity
  for (const std::string& s: f.GetInvalidOpenQuestionStrings())
  {
    try
    {
      f.Create(s);
      assert(!"Invalid questions must be rejected");
    }
    catch (std::exception&)
    {
      //OK
    }
  }
}
