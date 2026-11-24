#include "question.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <ostream>
#include <stdexcept>
#include <string>

#include "hometrainerhelper.h"
#include "multiplechoicequestion.h"
#include "openquestion.h"
#include "openquestionfactory.h"

ribi::Question::Question(
    const std::string& filename,
    const std::string& question,
    const std::vector<std::string>& correct_answers
)
: m_filename(filename),
  m_question(question),
  m_correct_answers(correct_answers)
{
  if (m_question.empty())
  {
    throw std::logic_error("A Question must contain a question");
  }
  if (m_correct_answers.empty())
  {
    throw std::logic_error("A question must have at least one answer");
  }
  for (const auto& s: m_correct_answers)
  {
    if (s.empty())
    {
      throw std::logic_error("A question must not have an empty answer");
    }
  }
}

ribi::Question::~Question()
{

}

std::unique_ptr<ribi::Question> ribi::CreateQuestion(
  const std::string& s
) noexcept
{
  try
  {
    return OpenQuestionFactory().Create(s).Clone();
  }
  catch (std::exception&)
  {
    //Question could not be parsed
  }
  try
  {
    return std::make_unique<MultipleChoiceQuestion>(s);
  }
  catch (std::exception&)
  {
    //Question could not be parsed
  }
  return nullptr;
}

std::vector<std::shared_ptr<const ribi::Question>> ribi::CreateQuestions(
  const std::vector<std::string>& lines,
  const std::string& path
)
{
  std::vector<std::shared_ptr<const Question>> v;
  for (const std::string& line: lines)
  {
    std::unique_ptr<Question> q = CreateQuestion(line);
    if (!q) continue;
    if (q->HasImage() && !path.empty())
    {
      //Re-create the question with the full path to the image
      auto fields = SeperateHometrainerLine(line);
      fields[0] = JoinPath(path, fields[0]);
      std::string s;
      for (const auto& field: fields) { s += EscapeHometrainerField(field) + ","; }
      s.pop_back();
      q = CreateQuestion(s);
      assert(q);
    }
    v.push_back(std::move(q));
  }
  return v;
}

std::vector<std::shared_ptr<const ribi::Question>> ribi::CreateQuestions(
  const std::string& filename
)
{
  return CreateQuestions(FileToVector(filename), GetPath(filename));
}

std::string ribi::EscapeHometrainerField(const std::string& s) noexcept
{
  return ReplaceAll(s, ",", "\\,");
}

std::string ribi::GetQuestionId(const Question& question) noexcept
{
  //Use '-' instead of comma, like Hometrainer 2 did
  return GetFileBasename(question.GetFilename()) + "-" + question.GetQuestion();
}

bool ribi::Question::HasImage() const noexcept
{
  //Like Hometrainer 2: a filename without a dot (such as '-') means no image
  return m_filename.find('.') != std::string::npos;
}

bool ribi::Question::IsCorrect(const std::string& s) const noexcept
{
  const std::string t = Trim(s);
  return std::find(m_correct_answers.begin(),m_correct_answers.end(),t) != m_correct_answers.end();
}

std::ostream& ribi::operator<<(std::ostream& os,const Question& question)
{
  const std::vector<std::string> v = question.ToLines();
  std::copy(v.begin(),v.end(),std::ostream_iterator<std::string>(os,"\n"));
  return os;
}

void ribi::TestQuestion() noexcept
{
  //CreateQuestion
  {
    assert(CreateQuestion("-,1+1=,2"));
    assert(dynamic_cast<OpenQuestion*>(CreateQuestion("-,1+1=,2").get()));
    assert(dynamic_cast<MultipleChoiceQuestion*>(CreateQuestion("-,1+1=,2,3").get()));
    assert(!CreateQuestion("Hometrainer (C) 2009 Richel Bilderbeek"));
    assert(!CreateQuestion(""));
  }
  //Escaped commas
  {
    const auto q = CreateQuestion("-,Name a number between 1\\,5 and 2\\,5,2");
    assert(q);
    assert(q->GetQuestion() == "Name a number between 1,5 and 2,5");
    assert(q->ToStr() == "-,Name a number between 1\\,5 and 2\\,5,2");
  }
  //Whitespace around answers is ignored
  {
    const auto q = CreateQuestion("-,1+1=,2");
    assert(q->IsCorrect(" 2 "));
  }
  //CreateQuestions skips the header and makes images relative
  {
    const std::vector<std::string> lines = {
      "Hometrainer (C) 2009 Richel Bilderbeek",
      "brain.png,What is this?,Brain",
      "-,1+1=,2,1,3",
      ""
    };
    const auto v = CreateQuestions(lines, "/my_folder");
    assert(v.size() == 2);
    assert(v[0]->GetFilename() == "/my_folder/brain.png");
    assert(v[1]->GetFilename() == "-");
    assert(GetQuestionId(*v[0]) == "brain.png-What is this?");
  }
}
