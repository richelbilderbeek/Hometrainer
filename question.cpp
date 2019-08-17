#include "question.h"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iterator>
#include <string>
#include <stdexcept>

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
}

bool ribi::Question::IsCorrect(const std::string& s) const noexcept
{
  return std::find(m_correct_answers.begin(),m_correct_answers.end(),s) != m_correct_answers.end();
}

std::ostream& ribi::operator<<(std::ostream& os,const Question& question)
{
  const std::vector<std::string> v = question.ToLines();
  std::copy(v.begin(),v.end(),std::ostream_iterator<std::string>(os,"\n"));
  return os;
}
