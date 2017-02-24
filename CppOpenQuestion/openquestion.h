#ifndef OPENQUESTION_H
#define OPENQUESTION_H

#include <string>
#include <vector>

#include "question.h"

namespace ribi {

struct OpenQuestionFactory;

///class for an open question
struct OpenQuestion : public Question
{
  ///Create a copy of the Question, depending on the derived class its type
  Question * Clone() const noexcept;

  static std::string GetVersion() noexcept;
  static std::vector<std::string> GetVersionHistory() noexcept;

  ///How to display the question as multiple lines
  std::vector<std::string> ToLines() const noexcept;

  ///Convert to std::string line, as read from file
  std::string ToStr() const noexcept;

  private:
  ///Throws nothing or std::out_of_range or std::runtime_error
  explicit OpenQuestion(const std::string& question);

  ///An open question has multiple possible answers
  ///Will throw if there is no question or if there are no answers
  OpenQuestion(
    const std::string& filename,
    const std::string& question,
    const std::vector<std::string>& answers
  );
  friend class OpenQuestionFactory;

  friend void boost::checked_delete<>(OpenQuestion *);
  friend void boost::checked_delete<>(const OpenQuestion *);
  ~OpenQuestion() noexcept {}

  ///The wrong answers are at indices 2 to SeperateString(input,',').size()
  static std::vector<std::string> ExtractAnswers(
    const std::string& input);

};

void TestOpenQuestion() noexcept;

} //~namespace ribi

#endif // OPENQUESTION_H
