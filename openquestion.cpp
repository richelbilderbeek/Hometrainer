#include "openquestion.h"

#include <cassert>
#include <string>
#include <fstream>
#include <stdexcept>

#include <boost/algorithm/string/split.hpp>
#include <boost/scoped_ptr.hpp>

#include "imagecanvas.h"
#include "openquestionfactory.h"

ribi::OpenQuestion::OpenQuestion(
  const std::string& filename,
  const std::string& question,
  const std::vector<std::string>& answers)
  : Question(filename,question, answers )
{
  //assert(!filename.empty() && "Filename must not be empty");
  //assert(FileExists(filename) == true && "File must exists");
  //assert(!question.empty() && "OpenQuestion must not be empty");
  //assert(!correct_answers.empty() && "Correct answer must not be empty");
}

ribi::Question * ribi::OpenQuestion::Clone() const noexcept
{
  return new OpenQuestion(
    this->GetFilename(),
    this->GetQuestion(),
    this->GetCorrectAnswers()
  );
}

//const std::vector<std::string>& ribi::OpenQuestion::GetAnswers() const noexcept
//{
//  this->GetCorrectAnswers()
//}

void ribi::TestOpenQuestion() noexcept
{
  //Test simple get/set with single answer
  {
    const std::string filename = "-";
    const std::string question = "1+1=";
    const std::string answer_1 = "2";
    const std::vector<std::string> answers { answer_1 };
    const auto q = OpenQuestionFactory().Create(filename,question,answers);
    assert(q->GetFilename() == filename);
    assert(q->GetQuestion() == question);
    assert(q->GetCorrectAnswers() == answers);
    assert(q->IsCorrect(answer_1));
    assert(!q->IsCorrect("3"));
    assert(!q->IsCorrect(filename));
    assert(!q->IsCorrect(question));
  }
  //Test simple get/set with two answers
  {
    const std::string filename = "-";
    const std::string question = "1+1=";
    const std::string answer_1 { "2" };
    const std::string answer_2 { "Two" };
    const std::vector<std::string> answers { answer_1, answer_2 };
    const auto q = OpenQuestionFactory().Create(filename,question,answers);
    assert(q->GetFilename() == filename);
    assert(q->GetQuestion() == question);
    assert(q->GetCorrectAnswers() == answers);
    assert(q->IsCorrect(answer_1));
    assert(q->IsCorrect(answer_2));
    assert(!q->IsCorrect( answer_1 + "/" + answer_2));
    assert(!q->IsCorrect(filename));
    assert(!q->IsCorrect(question));
  }
  //Test conversion std::string to OpenQuestion and back
  {
    const auto valid = OpenQuestionFactory().GetValidOpenQuestionStrings();
    for (const std::string& s: valid)
    {
      const auto q = OpenQuestionFactory().Create(s);
      assert(s == q->ToStr());
    }
  }
  //Test that ToLines always yields the same result
  {
    const auto valid = OpenQuestionFactory().GetValidOpenQuestionStrings();
    for (const std::string& s: valid)
    {
      const auto q = OpenQuestionFactory().Create(s);
      const auto v = q->ToLines();
      const auto w = q->ToLines();
      assert(v == w);
    }
  }
}

std::vector<std::string> ribi::OpenQuestion::ToLines() const noexcept
{
  std::vector<std::string> v;

  const int screen_rows { 23 };
  const int question_rows { 1 };
  const int n_rows { screen_rows - question_rows };
  if (!GetFilename().empty())
  {
    int n_cols = 78;

    while (1)
    {
      const boost::shared_ptr<ImageCanvas> canvas {
        new ImageCanvas(GetFilename(),n_cols)
      };
      if (canvas->GetHeight() > n_rows)
      {
        --n_cols;
      }
      else
      {
        v = canvas->ToStrings();
        break;
      }
      if (n_cols == 5) break;
    }
  }
  v.push_back(this->GetQuestion()); //The one question_row
  return v;
}

std::string ribi::OpenQuestion::ToStr() const noexcept
{
  //Concatenate the correct answer
  assert(!GetCorrectAnswers().empty());
  std::string correct_answers_str;

  for (const std::string s: this->GetCorrectAnswers()) { correct_answers_str += s + "/"; }
  assert(!correct_answers_str.empty());
  correct_answers_str.resize(correct_answers_str.size() - 1);

  std::string s
    = this->GetFilename()
    + "," + this->GetQuestion()
    + "," + correct_answers_str;
  return s;
}
