


#include "multiplechoicequestiondialog.h"

#include <cassert>
#include <boost/lexical_cast.hpp>
#include <boost/scoped_ptr.hpp>

#include "multiplechoicequestion.h"
#include "question.h"




ribi::MultipleChoiceQuestionDialog::MultipleChoiceQuestionDialog(
  const MultipleChoiceQuestion& question)
  : QuestionDialog{},
    m_answer_in_progress{-1},
    m_question(question)
{
  assert(!HasSubmitted());
}

ribi::MultipleChoiceQuestionDialog::MultipleChoiceQuestionDialog(const std::string& question)
  : QuestionDialog{},
    m_answer_in_progress{-1},
    m_question{question}
{
  assert(!HasSubmitted());
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

ribi::MultipleChoiceQuestionDialog::~MultipleChoiceQuestionDialog()
{

}

const ribi::Question& ribi::MultipleChoiceQuestionDialog::GetQuestion() const noexcept
{
  return m_question;
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
    const auto index = boost::lexical_cast<size_t>(s);
    const auto sz = GetMultipleChoiceQuestion().GetOptions().size();
    if (index >= sz)
    {
      throw std::logic_error("Must submit an existing index to a multiple choice question dialog");
    }
    //The real (that is non-index) answer
    const std::string t = GetMultipleChoiceQuestion().GetOptions()[index];
    this->SetIsCorrect(GetQuestion().IsCorrect(t));
  }
  catch (const boost::bad_lexical_cast&)
  {
    throw std::logic_error("Must submit an index to a multiple choice question dialog");
  }
}

void ribi::TestMultipleChoiceQuestionDialog() noexcept
{
  //Test setting the multiple choice questions
  for(const std::string& s: GetValidMultipleChoiceQuestions())
  {
    MultipleChoiceQuestion q(s);
    MultipleChoiceQuestionDialog d(q);
    assert(!d.HasSubmitted() );
  }
  //Test submitting correct and incorrect answers to this dialog
  {
    const std::vector<std::string> valid {
      GetValidMultipleChoiceQuestions()
    };
    for (const std::string& s: valid)
    {
      //Create a question
      const MultipleChoiceQuestion question(s);

      //Obtain the shuffled possibilities
      const std::vector<std::string> options = question.GetOptions();
      assert(options == question.GetOptions()
        && "The possibilities must be shuffled exactly once");

      //Submit correct answer to this dialog
      {
        boost::scoped_ptr<MultipleChoiceQuestionDialog> dialog {
          new MultipleChoiceQuestionDialog(
            question
          )
        };
        assert(!dialog->HasSubmitted());

        const std::string answer = question.GetAnswer();
        assert(!question.GetWrongAnswers().empty());

        const auto index = std::distance(options.begin(),std::find(options.begin(),options.end(),answer));
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

        assert(!question.GetWrongAnswers().empty());
        const std::string wrong_answer = question.GetWrongAnswers().at(0);

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
  s << m_question.ToStr();
  return s.str();
}
