#include "hometrainertestscorer.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <sstream>

#include "hometrainerhelper.h"
#include "hometrainertestresults.h"

double ribi::HometrainerQuestionScore::GetFractionCorrect() const noexcept
{
  if (m_n_answered == 0) return 0.0;
  return static_cast<double>(m_n_correct) / static_cast<double>(m_n_answered);
}

ribi::HometrainerTestScorer::HometrainerTestScorer(const int key, const int penalty)
  : m_key{key},
    m_penalty{penalty},
    m_question_scores{},
    m_student_scores{}
{

}

void ribi::HometrainerTestScorer::Add(const std::string& filename)
{
  Add(GetFileBasename(RemoveExtension(filename)), FileToVector(filename));
}

void ribi::HometrainerTestScorer::Add(
  const std::string& name,
  const std::vector<std::string>& encrypted_lines
)
{
  HometrainerStudentScore score;
  score.m_name = name;
  score.m_n_correct = 0;
  score.m_n_questions = 0;
  score.m_grade = 1.0;
  score.m_comment = "";

  const std::vector<std::string> lines = DecryptTestResults(encrypted_lines, m_key);

  //Find the username
  std::string username;
  for (const auto& line: lines)
  {
    if (line.size() > 5 && line.substr(0, 5) == "NAME,")
    {
      username = ReplaceAll(line.substr(5), "{comma}", ",");
    }
  }
  if (username.empty())
  {
    score.m_comment = "Username not found, is the key correct?";
    m_student_scores.push_back(score);
    return;
  }
  if (username != name)
  {
    score.m_grade = 1.1;
    score.m_comment = "Filename differs from username '" + username + "'";
    m_student_scores.push_back(score);
    return;
  }

  for (const auto& line: lines)
  {
    if (line.size() > 5 && line.substr(0, 5) == "NAME,") continue;
    const auto i = line.rfind(',');
    if (i == std::string::npos) continue;
    const std::string answer = line.substr(i + 1);
    if (answer != "Y" && answer != "N") continue;
    const std::string question = ReplaceAll(line.substr(0, i), "{comma}", ",");
    const bool is_correct = answer == "Y";
    ++score.m_n_questions;
    if (is_correct) ++score.m_n_correct;
    ScoreQuestion(question, is_correct);
  }
  score.m_grade = CalculateGrade(score.m_n_correct, score.m_n_questions, m_penalty);
  m_student_scores.push_back(score);
}

double ribi::CalculateGrade(const int n_correct, const int n_questions, const int penalty) noexcept
{
  if (n_questions - penalty <= 0)
  {
    return n_questions > 0 && n_correct == n_questions ? 10.0 : 1.0;
  }
  const double fraction
    = static_cast<double>(n_correct - penalty)
    / static_cast<double>(n_questions - penalty);
  const double grade = 1.0 + (9.0 * fraction);
  return std::max(1.0, std::min(10.0, grade));
}

std::string ribi::FormatGrade(const double grade) noexcept
{
  std::stringstream s;
  s << std::fixed << std::setprecision(1) << grade;
  return s.str();
}

std::vector<ribi::HometrainerQuestionScore>
  ribi::HometrainerTestScorer::GetQuestionScores() const noexcept
{
  std::vector<HometrainerQuestionScore> v = m_question_scores;
  std::sort(v.begin(), v.end(),
    [](const auto& lhs, const auto& rhs) { return lhs.m_question < rhs.m_question; }
  );
  return v;
}

std::vector<std::string> ribi::HometrainerTestScorer::QuestionScoresToCsv() const noexcept
{
  std::vector<std::string> v;
  v.push_back("Question,Correct,Answered,Fraction correct");
  for (const auto& s: GetQuestionScores())
  {
    v.push_back(
      "\"" + ReplaceAll(s.m_question, "\"", "\"\"") + "\","
      + std::to_string(s.m_n_correct) + ","
      + std::to_string(s.m_n_answered) + ","
      + FormatGrade(s.GetFractionCorrect())
    );
  }
  return v;
}

void ribi::HometrainerTestScorer::ScoreQuestion(const std::string& question, const bool is_correct)
{
  const auto i = std::find_if(m_question_scores.begin(), m_question_scores.end(),
    [question](const auto& s) { return s.m_question == question; }
  );
  if (i == m_question_scores.end())
  {
    HometrainerQuestionScore s;
    s.m_question = question;
    s.m_n_correct = is_correct ? 1 : 0;
    s.m_n_answered = 1;
    m_question_scores.push_back(s);
    return;
  }
  ++i->m_n_answered;
  if (is_correct) ++i->m_n_correct;
}

std::vector<std::string> ribi::HometrainerTestScorer::StudentScoresToCsv() const noexcept
{
  std::vector<std::string> v;
  v.push_back("#,Name,Correct,Questions,Score,Comment");
  int i = 1;
  for (const auto& s: m_student_scores)
  {
    v.push_back(
      std::to_string(i) + ","
      + "\"" + ReplaceAll(s.m_name, "\"", "\"\"") + "\","
      + std::to_string(s.m_n_correct) + ","
      + std::to_string(s.m_n_questions) + ","
      + FormatGrade(s.m_grade) + ","
      + "\"" + ReplaceAll(s.m_comment, "\"", "\"\"") + "\""
    );
    ++i;
  }
  return v;
}

void ribi::TestHometrainerTestScorer() noexcept
{
  //CalculateGrade
  {
    assert(CalculateGrade(0, 10, 0) == 1.0);
    assert(CalculateGrade(10, 10, 0) == 10.0);
    assert(CalculateGrade(5, 10, 0) == 5.5);
    assert(CalculateGrade(2, 10, 2) == 1.0);
    assert(CalculateGrade(0, 10, 2) == 1.0);
    assert(CalculateGrade(6, 10, 2) == 5.5);
    assert(CalculateGrade(1, 1, 1) == 10.0);
    assert(CalculateGrade(0, 0, 0) == 1.0);
    assert(FormatGrade(5.5) == "5.5");
    assert(FormatGrade(10.0) == "10.0");
  }
  const int key = 42;
  //Score two students
  {
    HometrainerTestResults r1("Alice");
    r1.Add("--1+1=", true);
    r1.Add("--2+2=", true);
    HometrainerTestResults r2("Bob");
    r2.Add("--1+1=", false);
    r2.Add("--2+2=", true);
    HometrainerTestScorer s(key);
    s.Add("Alice", r1.ToEncryptedLines(key));
    s.Add("Bob", r2.ToEncryptedLines(key));
    assert(s.GetStudentScores().size() == 2);
    assert(s.GetStudentScores()[0].m_name == "Alice");
    assert(s.GetStudentScores()[0].m_n_correct == 2);
    assert(s.GetStudentScores()[0].m_grade == 10.0);
    assert(s.GetStudentScores()[1].m_n_correct == 1);
    assert(s.GetStudentScores()[1].m_grade == 5.5);
    const auto q = s.GetQuestionScores();
    assert(q.size() == 2);
    assert(q[0].m_question == "--1+1=");
    assert(q[0].m_n_correct == 1);
    assert(q[0].m_n_answered == 2);
    assert(q[0].GetFractionCorrect() == 0.5);
    assert(q[1].GetFractionCorrect() == 1.0);
    assert(s.StudentScoresToCsv().size() == 3);
    assert(s.StudentScoresToCsv()[1] == "1,\"Alice\",2,2,10.0,\"\"");
    assert(s.QuestionScoresToCsv().size() == 3);
  }
  //A file of a student that is renamed is suspicious
  {
    HometrainerTestResults r("Alice");
    r.Add("--1+1=", true);
    HometrainerTestScorer s(key);
    s.Add("Bob", r.ToEncryptedLines(key));
    assert(s.GetStudentScores()[0].m_grade == 1.1);
    assert(!s.GetStudentScores()[0].m_comment.empty());
  }
  //A wrong key cannot find the username
  {
    HometrainerTestResults r("Alice");
    r.Add("--1+1=", true);
    HometrainerTestScorer s(key + 1);
    s.Add("Alice", r.ToEncryptedLines(key));
    assert(s.GetStudentScores()[0].m_grade == 1.0);
    assert(!s.GetStudentScores()[0].m_comment.empty());
  }
}
