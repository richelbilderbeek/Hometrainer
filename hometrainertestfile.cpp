#include "hometrainertestfile.h"

#include <algorithm>
#include <cassert>
#include <stdexcept>

#include "encranger.h"
#include "hometrainerhelper.h"
#include "question.h"

std::vector<std::string> ribi::CreateTest(
  const std::vector<std::string>& exercise_lines,
  const int n_questions,
  const int key
)
{
  if (n_questions < 1)
  {
    throw std::invalid_argument("A test must have at least one question");
  }
  std::vector<std::string> questions;
  std::copy_if(exercise_lines.begin(), exercise_lines.end(), std::back_inserter(questions),
    [](const std::string& line) { return CreateQuestion(line) != nullptr; }
  );
  if (questions.empty())
  {
    throw std::invalid_argument("The exercise has no questions");
  }
  std::shuffle(questions.begin(), questions.end(), GetRng());
  if (static_cast<int>(questions.size()) > n_questions)
  {
    questions.resize(static_cast<std::size_t>(n_questions));
  }
  const Encranger e(key);
  std::vector<std::string> v;
  v.push_back(e.Encrypt(GetHometrainerHeader()));
  for (const auto& line: questions) { v.push_back(e.Encrypt(line)); }
  return v;
}

std::vector<std::string> ribi::DecryptTest(
  const std::vector<std::string>& test_lines,
  const int key
)
{
  const Encranger e(key);
  std::vector<std::string> v;
  for (const auto& line: test_lines) { v.push_back(e.Deencrypt(line)); }
  if (v.empty() || !IsHometrainerHeader(v[0]))
  {
    throw std::invalid_argument("Incorrect key given, therefore the test is not loaded");
  }
  return v;
}

std::string ribi::GetHometrainerHeader() noexcept
{
  return "Hometrainer (C) 2009-2026 Richel Bilderbeek";
}

bool ribi::IsHometrainerHeader(const std::string& line) noexcept
{
  const std::string start{"Hometrainer"};
  return line.size() >= start.size() && line.substr(0, start.size()) == start;
}

std::vector<std::shared_ptr<const ribi::Question>> ribi::LoadTest(
  const std::string& filename,
  const int key
)
{
  return CreateQuestions(DecryptTest(FileToVector(filename), key), GetPath(filename));
}

void ribi::TestHometrainerTestFile() noexcept
{
  const std::vector<std::string> exercise = {
    "Hometrainer (C) 2009 Richel Bilderbeek",
    "-,1+1=,2",
    "-,1+2=,3,1,2",
    "-,2+2=,4/four",
    ""
  };
  assert(IsHometrainerHeader(exercise[0]));
  assert(IsHometrainerHeader("Hometrainer2 (C) 2009 Richel Bilderbeek"));
  assert(IsHometrainerHeader(GetHometrainerHeader()));
  assert(!IsHometrainerHeader("-,1+1=,2"));

  //A test can be decrypted with the correct key
  {
    const int key = 42;
    const auto test = CreateTest(exercise, 10, key);
    assert(test.size() == 4); //Header and three questions
    assert(test != exercise);
    const auto decrypted = DecryptTest(test, key);
    assert(decrypted[0] == GetHometrainerHeader());
    for (int i = 1; i != 4; ++i)
    {
      assert(std::count(exercise.begin(), exercise.end(), decrypted[static_cast<std::size_t>(i)]) == 1);
    }
    assert(CreateQuestions(decrypted).size() == 3);
  }
  //A test has at most n questions
  {
    const auto test = CreateTest(exercise, 2, 42);
    assert(test.size() == 3); //Header and two questions
  }
  //A test cannot be decrypted with the incorrect key
  {
    const auto test = CreateTest(exercise, 10, 42);
    try
    {
      DecryptTest(test, 43);
      assert(!"Should not get here");
    }
    catch (std::invalid_argument&)
    {
      //OK
    }
  }
  //An exercise without questions cannot be a test
  {
    try
    {
      CreateTest( { "Hometrainer" }, 10, 42);
      assert(!"Should not get here");
    }
    catch (std::invalid_argument&)
    {
      //OK
    }
  }
}
