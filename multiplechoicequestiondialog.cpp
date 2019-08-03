#pragma GCC diagnostic push


#include "multiplechoicequestiondialog.h"

#include <cassert>
#include <boost/lexical_cast.hpp>
#include <boost/scoped_ptr.hpp>

#include "multiplechoicequestion.h"
#include "question.h"


#pragma GCC diagnostic pop

ribi::MultipleChoiceQuestionDialog::MultipleChoiceQuestionDialog(
  const boost::shared_ptr<const MultipleChoiceQuestion> question)
  : QuestionDialog{},
    m_signal_mc_question_changed{},
    m_answer_in_progress{-1},
    m_question(question)
{
  assert(GetQuestion());
  assert(!HasSubmitted());
}

ribi::MultipleChoiceQuestionDialog::MultipleChoiceQuestionDialog(const std::string& question)
  : QuestionDialog{},
    m_signal_mc_question_changed{},
    m_answer_in_progress{-1},
    m_question(new MultipleChoiceQuestion(question))
{
  assert(!HasSubmitted());
  assert(GetQuestion());
}

boost::shared_ptr<ribi::MultipleChoiceQuestion>
  ribi::MultipleChoiceQuestionDialog::CreateDefaultQuestion() noexcept
{
  return boost::shared_ptr<MultipleChoiceQuestion>(
    new MultipleChoiceQuestion(
      "*",
      "1+2=",
      "3",
      {"1","2","4","5"}
    )
  );
}

ribi::MultipleChoiceQuestionDialog::~MultipleChoiceQuestionDialog() noexcept
{

}

boost::shared_ptr<const ribi::Question> ribi::MultipleChoiceQuestionDialog::GetQuestion() const noexcept
{
  return m_question;
}

std::string ribi::MultipleChoiceQuestionDialog::GetVersion() noexcept
{
  return "1.1";
}

std::vector<std::string> ribi::MultipleChoiceQuestionDialog::GetVersionHistory() noexcept
{
  return {
    "2011-06-29: version 1.0: initial version",
    "2013-10-24: version 1.1: added testing"
  };
}

void ribi::MultipleChoiceQuestionDialog::Submit(const std::string& s)
{
  if (HasSubmitted())
  {
    throw std::logic_error("Cannot submit a second answer");
  }
  assert(!HasSubmitted());
  try
  {
    const int index = boost::lexical_cast<int>(s);
    const int sz = this->GetMultipleChoiceQuestion()->GetOptions().size();
    if (index < 0)
    {
      throw std::logic_error("Must submit a positive index to a multiple choice question dialog");
    }
    if (index >= sz)
    {
      throw std::logic_error("Must submit an existing index to a multiple choice question dialog");
    }
    //The real (that is non-index) answer
    const std::string t = this->GetMultipleChoiceQuestion()->GetOptions()[index];
    this->SetIsCorrect(GetQuestion()->IsCorrect(t));
  }
  catch (boost::bad_lexical_cast&)
  {
    throw std::logic_error("Must submit an index to a multiple choice question dialog");
  }
}

void ribi::TestMultipleChoiceQuestionDialog() noexcept
{
  //Test setting the multiple choice questions
  for(const std::string& s: GetValidMultipleChoiceQuestions())
  {
    const boost::shared_ptr<MultipleChoiceQuestion> q {
      new MultipleChoiceQuestion(s)
    };
    assert(q);
    #if __cplusplus >= 201402L //C++17
    const auto d = std::make_unique<MultipleChoiceQuestionDialog>(q);
    #else
    const boost::scoped_ptr<MultipleChoiceQuestionDialog> d {
      new MultipleChoiceQuestionDialog(q)
    };
    #endif
    assert(d);
    assert(!d->HasSubmitted() );
  }
  //Test submitting correct and incorrect answers to this dialog
  {
    const std::vector<std::string> valid {
      GetValidMultipleChoiceQuestions()
    };
    for (const std::string& s: valid)
    {
      //Create a question
      const boost::shared_ptr<const MultipleChoiceQuestion> question {
        new MultipleChoiceQuestion(s)
      };

      //Obtain the shuffled possibilities
      const std::vector<std::string> options = question->GetOptions();
      assert(options == question->GetOptions()
        && "The possibilities must be shuffled exactly once");

      //Submit correct answer to this dialog
      {
        boost::scoped_ptr<MultipleChoiceQuestionDialog> dialog {
          new MultipleChoiceQuestionDialog(
            question
          )
        };
        assert(!dialog->HasSubmitted());

        const std::string answer = question->GetAnswer();
        assert(!question->GetWrongAnswers().empty());

        const int index = std::distance(options.begin(),std::find(options.begin(),options.end(),answer));
        assert(index >= 0);
        assert(index < static_cast<int>(options.size()));

        dialog->Submit(boost::lexical_cast<std::string>(index));

        assert(dialog->HasSubmitted());
        assert(dialog->IsAnswerCorrect());
      }
      //Submit incorrect answer to this dialog
      {
        boost::scoped_ptr<MultipleChoiceQuestionDialog> dialog {
          new MultipleChoiceQuestionDialog(
            question
          )
        };
        assert(!dialog->HasSubmitted());

        assert(!question->GetWrongAnswers().empty());
        const std::string wrong_answer = question->GetWrongAnswers().at(0);

        const int index = std::distance(options.begin(),std::find(options.begin(),options.end(),wrong_answer));
        assert(index >= 0);
        assert(index < static_cast<int>(options.size()));

        dialog->Submit(boost::lexical_cast<std::string>(index));

        assert(dialog->HasSubmitted());
        assert(!dialog->IsAnswerCorrect());
      }
    }
  }
}

std::string ribi::MultipleChoiceQuestionDialog::ToStr() const noexcept
{
  std::stringstream s;
  s << m_question->ToStr();
  return s.str();
}
