#ifndef MULTIPLECHOICEQUESTION_H
#define MULTIPLECHOICEQUESTION_H

#include <string>
#include <vector>

#include "question.h"

namespace ribi {

///Class for a multiple choice question
struct MultipleChoiceQuestion : public Question
{
  ///Create a multiple choice question from a line of a Hometrainer file,
  ///for example '-,1+1=,2,1,3'
  ///Throws std::logic_error if this is not a multiple choice question
  explicit MultipleChoiceQuestion(const std::string& question);

  explicit MultipleChoiceQuestion(
    const std::string& filename,
    const std::string& question,
    const std::string& answer,
    const std::vector<std::string>& wrong_answers
  );

  ///Create a copy of the Question
  std::unique_ptr<Question> Clone() const override;

  ///Obtain the only correct answer
  const std::string& GetAnswer() const noexcept;

  ///Obtain an example multiple choice question
  static std::string GetExampleMultipleChoiceQuestion() noexcept { return "-,1+1=,2,0,4"; }

  ///Obtain the possible options to be chosen in a random order
  const std::vector<std::string>& GetOptions() const noexcept;

  ///Obtain the version
  static std::string GetVersion() noexcept;

  ///Obtain the version history
  static std::vector<std::string> GetVersionHistory() noexcept;

  ///Obtain the wrong answers
  const std::vector<std::string>& GetWrongAnswers() const noexcept { return m_wrong_answers; }

  ///How to display the question as multiple lines
  std::vector<std::string> ToLines() const noexcept override;

  ///Convert to std::string line
  std::string ToStr() const noexcept override;

  private:

  ///All the wrong answers
  std::vector<std::string> m_wrong_answers;

  ///All the options the user can choose from, which equals
  ///all wrong answers and the correct answer, shuffled randomly
  std::vector<std::string> m_options;
};

///Create the possible options to be chosen in a random order
std::vector<std::string> CreateMcqOptions(
  const std::vector<std::string>& wrong_answers,
  const std::string& answer) noexcept;

///The wrong answers are at indices 3 to SeperateHometrainerLine(input).size()
///Throws std::logic_error if there are no wrong answers
std::vector<std::string> ExtractMcqWrongAnswers(
  const std::string& input);

///Obtain invalid multiple choice questions
std::vector<std::string> GetInvalidMultipleChoiceQuestions() noexcept;

///Obtain valid multiple choice questions
std::vector<std::string> GetValidMultipleChoiceQuestions() noexcept;

void TestMultipleChoiceQuestion() noexcept;

} //~namespace ribi

#endif // MULTIPLECHOICEQUESTION_H
