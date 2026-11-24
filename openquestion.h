#ifndef OPENQUESTION_H
#define OPENQUESTION_H

#include <string>
#include <vector>

#include "question.h"

namespace ribi {

///class for an open question
struct OpenQuestion : public Question
{
  ///An open question has multiple possible answers
  ///Will throw if there is no question or if there are no answers.
  ///Use OpenQuestionFactory to create an OpenQuestion from a std::string
  explicit OpenQuestion(
    const std::string& filename,
    const std::string& question,
    const std::vector<std::string>& answers
  );

  ///Create a copy of the Question
  std::unique_ptr<Question> Clone() const override;

  ///How to display the question as multiple lines
  std::vector<std::string> ToLines() const noexcept override;

  ///Convert to std::string line, as read from file
  std::string ToStr() const noexcept override;
};

bool operator==(const OpenQuestion& lhs, const OpenQuestion& rhs) noexcept;
bool operator!=(const OpenQuestion& lhs, const OpenQuestion& rhs) noexcept;

void TestOpenQuestion() noexcept;

} //~namespace ribi

#endif // OPENQUESTION_H
