#ifndef MULTIPLECHOICEQUESTION_H
#define MULTIPLECHOICEQUESTION_H

#include <string>
#include <vector>

#include "question.h"

namespace ribi {

///Class for a multiple choice question
struct MultipleChoiceQuestion : public Question
{
  ///Throws nothing or std::out_of_range or std::runtime_error
  explicit MultipleChoiceQuestion(const std::string& question);

  explicit MultipleChoiceQuestion(
    const std::string& filename,
    const std::string& question,
    const std::string& answer,
    const std::vector<std::string>& wrong_answers
  );

  ///Create a copy of the Question, depending on the derived class its type
  Question * Clone() const noexcept;

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
  std::vector<std::string> ToLines() const noexcept;

  ///Convert to std::string line
  std::string ToStr() const noexcept;

  private:
  friend void boost::checked_delete<>(MultipleChoiceQuestion *);
  ~MultipleChoiceQuestion() noexcept {}

  ///All the wrong answers
  const std::vector<std::string> m_wrong_answers;

  ///All the options the user can choose from, which equals
  ///all wrong answers and the correct answer, shuffled randomly
  const std::vector<std::string> m_options;

  ///Create the possible options to be chosen in a random order
  static std::vector<std::string> CreateOptions(
    const std::vector<std::string>& wrong_answers,
    const std::string& answer) noexcept;

  ///The options (correct + wrong answers) are at indices 2 to SeperateString(input,',').size()
  static std::vector<std::string> ExtractOptions(
    const std::string& input);

  ///The wrong answers are at indices 3 to SeperateString(input,',').size()
  static std::vector<std::string> ExtractWrongAnswers(
    const std::string& input);


};

///Obtain valid multiple choice question
std::vector<std::string> GetInvalidMultipleChoiceQuestions() noexcept;

///Obtain valid multiple choice question
std::vector<std::string> GetValidMultipleChoiceQuestions() noexcept;

void TestMultipleChoiceQuestion() noexcept;

} //~namespace ribi

#endif // MULTIPLECHOICEQUESTION_H
