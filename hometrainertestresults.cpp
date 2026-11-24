#include "hometrainertestresults.h"

#include <algorithm>
#include <cassert>
#include <stdexcept>

#include "encranger.h"
#include "hometrainerhelper.h"
#include "openquestionfactory.h"
#include "openquestion.h"
#include "question.h"

ribi::HometrainerTestResults::HometrainerTestResults(const std::string& username)
  : m_answers{},
    m_username{Trim(username)}
{
  if (m_username.empty())
  {
    throw std::invalid_argument("A username must not be empty");
  }
}

void ribi::HometrainerTestResults::Add(const Question& question, const bool is_correct)
{
  Add(GetQuestionId(question), is_correct);
}

void ribi::HometrainerTestResults::Add(const std::string& question_id, const bool is_correct)
{
  m_answers.push_back(std::make_pair(question_id, is_correct));
}

int ribi::HometrainerTestResults::GetNumberCorrect() const noexcept
{
  return static_cast<int>(std::count_if(m_answers.begin(), m_answers.end(),
    [](const auto& p) { return p.second; }
  ));
}

std::vector<std::string> ribi::HometrainerTestResults::ToLines() const noexcept
{
  std::vector<std::string> v;
  v.push_back("NAME," + ReplaceAll(m_username, ",", "{comma}"));
  for (const auto& p: m_answers)
  {
    v.push_back(ReplaceAll(p.first, ",", "{comma}") + (p.second ? ",Y" : ",N"));
  }
  return v;
}

std::vector<std::string> ribi::HometrainerTestResults::ToEncryptedLines(const int key) const noexcept
{
  std::vector<std::string> v = ToLines();

  //Obscure all lines by a total of nine asterisks
  std::uniform_int_distribution<int> d(0, 6);
  for (auto& s: v)
  {
    const int n_ast = d(GetRng());
    s = std::string(static_cast<std::size_t>(1 + n_ast), '*')
      + s
      + std::string(static_cast<std::size_t>(8 - n_ast), '*');
  }

  //Shuffle the obscured lines at random
  std::shuffle(v.begin(), v.end(), GetRng());

  //Encode lines
  const Encranger e(key);
  for (auto& s: v) { s = e.Encrypt(s); }
  return v;
}

std::vector<std::string> ribi::DecryptTestResults(
  const std::vector<std::string>& encrypted_lines,
  const int key
) noexcept
{
  const Encranger e(key);
  std::vector<std::string> v;
  for (const auto& line: encrypted_lines)
  {
    std::string s = e.Deencrypt(line);
    //Remove the obscuring asterisks
    const auto begin = s.find_first_not_of('*');
    if (begin == std::string::npos) continue;
    const auto end = s.find_last_not_of('*');
    s = s.substr(begin, end - begin + 1);
    if (s.empty()) continue;
    v.push_back(s);
  }
  return v;
}

void ribi::TestHometrainerTestResults() noexcept
{
  //An empty username is not allowed
  {
    try
    {
      HometrainerTestResults r(" ");
      assert(!"Should not get here");
    }
    catch (std::invalid_argument&)
    {
      //OK
    }
  }
  //Plaintext lines
  {
    HometrainerTestResults r("John Doe");
    r.Add(OpenQuestionFactory().Create("brain.png,What is this\\, really?,Brain"), true);
    r.Add(OpenQuestionFactory().Create("-,1+1=,2"), false);
    assert(r.GetNumberOfAnswers() == 2);
    assert(r.GetNumberCorrect() == 1);
    const std::vector<std::string> expected = {
      "NAME,John Doe",
      "brain.png-What is this{comma} really?,Y",
      "--1+1=,N"
    };
    assert(r.ToLines() == expected);
  }
  //Encrypted lines can be decrypted
  {
    HometrainerTestResults r("John Doe");
    r.Add(OpenQuestionFactory().Create("-,1*1=,1"), true);
    const auto encrypted = r.ToEncryptedLines(42);
    assert(encrypted.size() == 2);
    auto decrypted = DecryptTestResults(encrypted, 42);
    std::sort(decrypted.begin(), decrypted.end());
    auto expected = r.ToLines();
    std::sort(expected.begin(), expected.end());
    assert(decrypted == expected);
  }
}
