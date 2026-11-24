#include "openquestion.h"

#include <cassert>
#include <string>
#include <stdexcept>

#include "hometrainerhelper.h"
#include "openquestionfactory.h"

ribi::OpenQuestion::OpenQuestion(
  const std::string& filename,
  const std::string& question,
  const std::vector<std::string>& answers)
  : Question(filename,question, answers )
{

}

std::unique_ptr<ribi::Question> ribi::OpenQuestion::Clone() const
{
  return std::make_unique<OpenQuestion>(
    this->GetFilename(),
    this->GetQuestion(),
    this->GetCorrectAnswers()
  );
}

bool ribi::operator==(const OpenQuestion& lhs, const OpenQuestion& rhs) noexcept
{
  return lhs.ToStr() == rhs.ToStr();
}

bool ribi::operator!=(const OpenQuestion& lhs, const OpenQuestion& rhs) noexcept
{
  return !(lhs == rhs);
}

void ribi::TestOpenQuestion() noexcept
{
  //Test simple get/set with single answer
  {
    const std::string filename = "-";
    const std::string question = "1+1=";
    const std::string answer_1 = "2";
    const std::vector<std::string> answers { answer_1 };
    const auto q = OpenQuestionFactory().Create(filename,question,answers);
    assert(q.GetFilename() == filename);
    assert(q.GetQuestion() == question);
    assert(q.GetCorrectAnswers() == answers);
    assert(q.IsCorrect(answer_1));
    assert(!q.IsCorrect("3"));
    assert(!q.IsCorrect(filename));
    assert(!q.IsCorrect(question));
  }
  //Test simple get/set with two answers
  {
    const std::string filename = "-";
    const std::string question = "1+1=";
    const std::string answer_1 { "2" };
    const std::string answer_2 { "Two" };
    const std::vector<std::string> answers { answer_1, answer_2 };
    const auto q = OpenQuestionFactory().Create(filename,question,answers);
    assert(q.GetFilename() == filename);
    assert(q.GetQuestion() == question);
    assert(q.GetCorrectAnswers() == answers);
    assert(q.IsCorrect(answer_1));
    assert(q.IsCorrect(answer_2));
    assert(!q.IsCorrect( answer_1 + "/" + answer_2));
    assert(!q.IsCorrect(filename));
    assert(!q.IsCorrect(question));
  }
  //Test conversion std::string to OpenQuestion and back
  {
    const auto valid = OpenQuestionFactory().GetValidOpenQuestionStrings();
    for (const std::string& s: valid)
    {
      const auto q = OpenQuestionFactory().Create(s);
      assert(s == q.ToStr());
    }
  }
  //Test that ToLines always yields the same result
  {
    const auto valid = OpenQuestionFactory().GetValidOpenQuestionStrings();
    for (const std::string& s: valid)
    {
      const auto q = OpenQuestionFactory().Create(s);
      const auto v = q.ToLines();
      const auto w = q.ToLines();
      assert(v == w);
    }
  }
  //Clone
  {
    const auto q = OpenQuestionFactory().Create("-,1+1=,2/two");
    const auto r = q.Clone();
    assert(r->ToStr() == q.ToStr());
  }
}

std::vector<std::string> ribi::OpenQuestion::ToLines() const noexcept
{
  const int screen_rows { 23 };
  const int question_rows { 1 };
  std::vector<std::string> v;
  if (HasImage())
  {
    v = ImageToAsciiArt(GetFilename(), 78, screen_rows - question_rows);
  }
  v.push_back(this->GetQuestion()); //The one question_row
  return v;
}

std::string ribi::OpenQuestion::ToStr() const noexcept
{
  //Concatenate the correct answer
  assert(!GetCorrectAnswers().empty());
  std::string correct_answers_str;

  for (const std::string& s: this->GetCorrectAnswers()) { correct_answers_str += s + "/"; }
  assert(!correct_answers_str.empty());
  correct_answers_str.pop_back();

  std::string s
    = EscapeHometrainerField(this->GetFilename())
    + "," + EscapeHometrainerField(this->GetQuestion())
    + "," + EscapeHometrainerField(correct_answers_str);
  return s;
}
