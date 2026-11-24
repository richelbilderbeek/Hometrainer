#ifndef HOMETRAINERTESTRESULTS_H
#define HOMETRAINERTESTRESULTS_H

#include <string>
#include <vector>

namespace ribi {

struct Question;

///The results of a student doing a test.
///These results are saved encrypted, so that the student
///cannot read or modify them. Use HometrainerTestScorer
///to grade these results
struct HometrainerTestResults
{
  ///Throws std::invalid_argument if the username is empty
  explicit HometrainerTestResults(const std::string& username);

  ///Add the answer to a question
  void Add(const Question& question, const bool is_correct);

  ///Add the answer to a question, by its ID, see GetQuestionId
  void Add(const std::string& question_id, const bool is_correct);

  ///The number of answers given
  int GetNumberOfAnswers() const noexcept { return static_cast<int>(m_answers.size()); }

  ///The number of correct answers given
  int GetNumberCorrect() const noexcept;

  const std::string& GetUsername() const noexcept { return m_username; }

  ///Convert to plaintext lines.
  ///The first line is 'NAME,[username]',
  ///the other lines are '[question ID],[Y or N]'
  ///Commas in the username and question IDs are written as '{comma}'
  std::vector<std::string> ToLines() const noexcept;

  ///Convert to encrypted lines, in which the lines are obscured
  ///by asterisks and shuffled
  std::vector<std::string> ToEncryptedLines(const int key) const noexcept;

  private:

  ///The question IDs and if the student answered these correctly
  std::vector<std::pair<std::string, bool>> m_answers;

  std::string m_username;
};

///Decrypt the lines of test results:
///remove the obscuring asterisks and the empty lines
std::vector<std::string> DecryptTestResults(
  const std::vector<std::string>& encrypted_lines,
  const int key
) noexcept;

void TestHometrainerTestResults() noexcept;

} //~namespace ribi

#endif // HOMETRAINERTESTRESULTS_H
