#ifndef QUESTION_H
#define QUESTION_H

#include <string>
#include <vector>

namespace ribi {

///Class for a question.
///A question only knows its question and correct answers and optionally
///an image. Derived classes need to maintain incorrect answers.
struct Question
{
  explicit Question(
    const std::string& filename,
    const std::string& question,
    const std::vector<std::string>& correct_answers
  );
  virtual ~Question();

  ///Obtain the correct answer(s)
  const std::vector<std::string>& GetCorrectAnswers() const noexcept { return m_correct_answers; }

  ///Obtain the possible media filename, which will be empty
  ///if none is used
  const std::string& GetFilename() const noexcept { return m_filename; }

  ///Obtain the question
  const std::string& GetQuestion() const noexcept { return m_question; }

  ///Would a submitted std::string be the correct answer?
  bool IsCorrect(const std::string& s) const noexcept;

  ///How to display the question as multiple lines
  virtual std::vector<std::string> ToLines() const noexcept = 0;

  ///Convert to std::string line, as read from file
  virtual std::string ToStr() const noexcept = 0;

  private:

  ///Every Question might have a media filename
  const std::string m_filename;

  ///The question
  const std::string m_question;

  ///The list of possible answers
  const std::vector<std::string> m_correct_answers;
};

std::ostream& operator<<(std::ostream& os,const Question& question);

} //~namespace ribi

#endif // QUESTION_H
