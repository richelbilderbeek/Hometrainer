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
  ///An open question has multiple possible answers
  ///Will throw if there is no question or if there are no answers
  explicit OpenQuestion(
    const std::string& filename = "",
    const std::string& question = "",
    const std::vector<std::string>& answers = {}
  );

  ///How to display the question as multiple lines
  std::vector<std::string> ToLines() const noexcept override;

  ///Convert to std::string line, as read from file
  std::string ToStr() const noexcept override;
};

void TestOpenQuestion() noexcept;

} //~namespace ribi

#endif // OPENQUESTION_H
