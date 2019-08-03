


#include "exercise.h"

#include <fstream>
#include <stdexcept>


#include <boost/numeric/conversion/cast.hpp>

#include "fileio.h"
#include "multiplechoicequestion.h"
#include "multiplechoicequestiondialog.h"
#include "openquestion.h"
#include "openquestiondialog.h"
#include "openquestiondialogfactory.h"
//

#include <QFile>



ribi::Exercise::Exercise(const std::string& filename)
  : m_current{},
    m_questions{}
{
  assert(QFile::exists(filename.c_str()));
  if (!QFile::exists(filename.c_str()))
  {
    throw std::logic_error("File does not exist");
  }
  const std::vector<std::string> v {
    ribi::fileio::FileIo().FileToVector(filename)
  };
  m_questions.reserve(v.size());
  for(const std::string& s: v)
  {
    try
    {
      boost::shared_ptr<QuestionDialog> tmp
        = OpenQuestionDialogFactory().Create(s);
      m_questions.push_back(s);
      continue;
    }
    catch (std::exception&)
    {
      //No problem
    }
    try
    {
      boost::shared_ptr<QuestionDialog> tmp(new MultipleChoiceQuestionDialog(s));
      m_questions.push_back(s);
      continue;
    }
    catch (std::exception&)
    {
      //No problem
    }
  }

  if (m_questions.empty())
  {
    throw std::runtime_error("No questions found in loading the Exercise");
  }
  assert(!m_questions.empty());

  //Shuffle the questions at start
  std::random_shuffle(m_questions.begin(),m_questions.end());
  m_current = m_questions.begin();
  assert(m_current != m_questions.end());
}

std::string ribi::Exercise::GetCurrentQuestion() const noexcept
{
  assert(m_current != m_questions.end());
  return *m_current;
}

int ribi::Exercise::GetNumberOfQuestions() const noexcept
{
  assert(!m_questions.empty());
  return boost::numeric_cast<int>(m_questions.size());
}

std::string ribi::Exercise::GetVersion() noexcept
{
  return "1.1";
}

std::vector<std::string> ribi::Exercise::GetVersionHistory() noexcept
{
  return {
    "2011-09-26: Version 1.0: initial version",
    "2011-10-30: Version 1.1: shuffle questions at start"
  };
}

void ribi::Exercise::Next() noexcept
{
  ++m_current;
  if (m_current == m_questions.end())
  {
    std::random_shuffle(m_questions.begin(),m_questions.end());
    m_current = m_questions.begin();
  }
}

