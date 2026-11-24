#include "multiplechoicequestion.h"

#include <algorithm>
#include <cassert>
#include <sstream>
#include <stdexcept>

#include "hometrainerhelper.h"

namespace ribi {

///Split a multiple choice question into its fields, checking it on the way.
///Throws std::logic_error if the question is invalid
std::vector<std::string> SeperateMcq(const std::string& question)
{
  if (question.empty())
  {
    throw std::logic_error("A multiple choice question must contain text");
  }
  const auto v = SeperateHometrainerLine(question);
  if (v.size() < 4)
  {
    throw std::logic_error(
      "A multiple choice question must contain at least four elements:"
      "[0] image,"
      "[1] question,"
      "[2] answer,"
      "[3-x] incorrect answers");
  }
  if (std::find(v.begin(), v.end(), "") != v.end())
  {
    throw std::logic_error("A multiple choice question cannot have empty elements");
  }
  return v;
}

} //~namespace ribi

ribi::MultipleChoiceQuestion::MultipleChoiceQuestion(const std::string& question)
  : MultipleChoiceQuestion(
      SeperateMcq(question).at(0),
      SeperateMcq(question).at(1),
      SeperateMcq(question).at(2),
      ExtractMcqWrongAnswers(question)
    )
{

}

ribi::MultipleChoiceQuestion::MultipleChoiceQuestion(
  const std::string& filename,
  const std::string& question,
  const std::string& answer,
  const std::vector<std::string>& wrong_answers)
: Question(filename,question, {answer} ),
  m_wrong_answers(wrong_answers),
  m_options(CreateMcqOptions(wrong_answers,answer))
{
  if (this->GetWrongAnswers().empty())
  {
    throw std::logic_error("a MultipleChoiceQuestion must have at least one incorrect option");
  }
  assert(!GetCorrectAnswers().empty() && "There must be a correct answer");
  assert(!GetWrongAnswers().empty() && "Wrong answers must not be empty");
}

std::unique_ptr<ribi::Question> ribi::MultipleChoiceQuestion::Clone() const
{
  return std::make_unique<MultipleChoiceQuestion>(
    this->GetFilename(),
    this->GetQuestion(),
    this->GetAnswer(),
    this->GetWrongAnswers());
}

const std::string& ribi::MultipleChoiceQuestion::GetAnswer() const noexcept
{
  assert(!GetCorrectAnswers().empty());
  return GetCorrectAnswers()[0];
}

std::vector<std::string> ribi::CreateMcqOptions(
  const std::vector<std::string>& wrong_answers,
  const std::string& answer) noexcept
{
  std::vector<std::string> v = wrong_answers;
  v.push_back(answer);
  std::shuffle(std::begin(v),std::end(v), GetRng());
  return v;
}

std::vector<std::string> ribi::ExtractMcqWrongAnswers(const std::string& input)
{
  const auto v = SeperateMcq(input);
  return std::vector<std::string>(v.begin() + 3, v.end());
}

std::vector<std::string> ribi::GetInvalidMultipleChoiceQuestions() noexcept
{
  return {
    "-,1+1=,2", //No incorrect options
    "-,1+1=",   //No answer
    "-",        //No question
    "tmp.png",  //No question
    "",         //Nothing
    ",tmp.png,1+1=,2,1", //Start with comma
    "tmp.png,,1+1=,2,1", //Two consecutive comma's
    "tmp.png,1+1=,,2,1", //Two consecutive comma's
    "tmp.png,1+1=,2,,1", //Two consecutive comma's
    "tmp.png,1+1=,2,1,", //End with comma
    ",tmp.png,1+1=,2,1,", //Start and end with comma
    ",,tmp.png,1+1=,2,1,",
    ",tmp.png,,1+1=,2,1,",
    ",tmp.png,1+1=,,2,1,",
    ",tmp.png,1+1=,2,,1,",
    ",tmp.png,1+1=,2,1,,",
    ",",
    ",,",
    ",,,",
    ",,,,",
    ",,,,,",
    ",,,,,,"
  };
}

const std::vector<std::string>& ribi::MultipleChoiceQuestion::GetOptions() const noexcept
{
  return m_options;
}

std::vector<std::string> ribi::GetValidMultipleChoiceQuestions() noexcept
{
  return {
    "-,1+1=,2,1",
    "tmp.png,1+1=,2,1",
    "-,1+1=,2,1,0,3,4,5,6,7,8,9,0",
    "-,1\\,5 + 1\\,5 =,3,2\\,5,3\\,5"
  };
}

std::string ribi::MultipleChoiceQuestion::GetVersion() noexcept
{
  return "1.3";
}

std::vector<std::string> ribi::MultipleChoiceQuestion::GetVersionHistory() noexcept
{
  return {
    "2011-06-27: version 1.0: initial version",
    "2011-09-16: version 1.1: allow parsing from std::string",
    "2013-09-26: version 1.2: added testing",
    "2026-11-24: version 1.3: allow escaped commas"
  };
}

void ribi::TestMultipleChoiceQuestion() noexcept
{
  //The example must be valid
  {
    const MultipleChoiceQuestion q(MultipleChoiceQuestion::GetExampleMultipleChoiceQuestion());
    assert(q.IsCorrect("2"));
  }
  //Test valid multiple choice questions for validity
  for (const std::string& s: GetValidMultipleChoiceQuestions())
  {
    try
    {
      const MultipleChoiceQuestion q(s);
    }
    catch (std::exception&)
    {
      assert(!"Valid questions must be accepted");
    }
  }
  //Test invalid multiple choice questions for invalidity
  for (const std::string& s: GetInvalidMultipleChoiceQuestions())
  {
    try
    {
      const MultipleChoiceQuestion q(s);
      assert(!"Invalid questions must be rejected");
    }
    catch (std::exception&)
    {
      //OK
    }
  }
  //Test simple get/set
  {
    const std::string filename = "-";
    const std::string question = "1+1=";
    const std::string answer = "2";
    const std::vector<std::string> wrong_answers { "chicken", "cow" };
    const MultipleChoiceQuestion q(filename,question,answer,wrong_answers);
    assert(q.GetFilename() == filename);
    assert(q.GetQuestion() == question);
    assert(q.GetAnswer() == answer);
    assert(q.GetWrongAnswers() == wrong_answers);
    assert(q.GetOptions().size() == wrong_answers.size() + 1); //The options are the incorrect answers and the correct answer(s)
    assert(q.IsCorrect(answer));
    assert(!q.IsCorrect(wrong_answers.at(0)));
    assert(!q.IsCorrect(wrong_answers.at(1)));
  }
  //Test conversion std::string to MultipleChoiceQuestion and back
  for (const std::string& s: GetValidMultipleChoiceQuestions())
  {
    const MultipleChoiceQuestion q(s);
    assert(s == q.ToStr());
    assert(s == q.Clone()->ToStr());
  }
  //Test that ToLines always yields the same result
  for (const std::string& s: GetValidMultipleChoiceQuestions())
  {
    const MultipleChoiceQuestion q(s);
    assert(q.ToLines() == q.ToLines());
  }
}

std::vector<std::string> ribi::MultipleChoiceQuestion::ToLines() const noexcept
{
  std::vector<std::string> question_lines;
  question_lines.push_back(this->GetQuestion());
  question_lines.push_back("");
  int i=0;
  for (const std::string& s: GetOptions())
  {
    std::stringstream t;
    t << '[' << i << "] " << s;
    question_lines.push_back(t.str());
    ++i;
  }

  const int screen_rows { 23 };
  const int question_rows { static_cast<int>(question_lines.size()) };
  const int n_rows { std::max(1, screen_rows - question_rows) };

  std::vector<std::string> lines;
  if (HasImage())
  {
    lines = ImageToAsciiArt(GetFilename(), 78, n_rows);
  }
  std::copy(question_lines.begin(),question_lines.end(),std::back_inserter(lines));
  return lines;
}

std::string ribi::MultipleChoiceQuestion::ToStr() const noexcept
{
  std::string s
    = EscapeHometrainerField(this->GetFilename())
    + "," + EscapeHometrainerField(this->GetQuestion())
    + "," + EscapeHometrainerField(this->GetAnswer());
  for (const std::string& t: this->GetWrongAnswers()) { s += "," + EscapeHometrainerField(t); }
  return s;
}
