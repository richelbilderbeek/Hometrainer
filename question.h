#ifndef QUESTION_H
#define QUESTION_H

#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

namespace ribi {

///Class for a question.
///A question only knows its question and correct answers and optionally
///an image. Derived classes need to maintain incorrect answers.
struct Question
{
  ///Throws std::logic_error if the question or the answers are empty
  explicit Question(
    const std::string& filename,
    const std::string& question,
    const std::vector<std::string>& correct_answers
  );
  virtual ~Question();

  ///Create a copy of the Question, depending on the derived class its type
  virtual std::unique_ptr<Question> Clone() const = 0;

  ///Obtain the correct answer(s)
  const std::vector<std::string>& GetCorrectAnswers() const noexcept { return m_correct_answers; }

  ///Obtain the possible media filename, which will be '-'
  ///(or another text without a dot) if none is used
  const std::string& GetFilename() const noexcept { return m_filename; }

  ///Obtain the question
  const std::string& GetQuestion() const noexcept { return m_question; }

  ///Does the question have an image?
  ///It has none if the filename has no dot, such as '-' 
  bool HasImage() const noexcept;

  ///Would a submitted std::string be the correct answer?
  ///Leading and trailing whitespace are ignored
  bool IsCorrect(const std::string& s) const noexcept;

  ///How to display the question as multiple lines
  virtual std::vector<std::string> ToLines() const noexcept = 0;

  ///Convert to std::string line, as read from file
  virtual std::string ToStr() const noexcept = 0;

  private:

  ///Every Question might have a media filename
  std::string m_filename;

  ///The question
  std::string m_question;

  ///The list of possible answers
  std::vector<std::string> m_correct_answers;
};

///Escape the commas in a field of a Hometrainer file,
///for example 'a,b' becomes 'a\,b'
std::string EscapeHometrainerField(const std::string& s) noexcept;

///Create a Question from a line of a Hometrainer file.
///Returns nullptr if the string cannot be converted to a question
std::unique_ptr<Question> CreateQuestion(const std::string& s) noexcept;

///Create the Questions from the lines of a Hometrainer file.
///Lines that are no question (such as the first line) are ignored.
///Image filenames are made relative to 'path'
std::vector<std::shared_ptr<const Question>> CreateQuestions(
  const std::vector<std::string>& lines,
  const std::string& path = ""
);

///Load the Questions from a Hometrainer file.
///Image filenames are relative to the folder of the file.
///Throws std::runtime_error if the file does not exist
std::vector<std::shared_ptr<const Question>> CreateQuestions(
  const std::string& filename
);

///Obtain a short and unique description of the question,
///as used in test results, for example 'brain.png-What is this?'
std::string GetQuestionId(const Question& question) noexcept;

std::ostream& operator<<(std::ostream& os,const Question& question);

void TestQuestion() noexcept;

} //~namespace ribi

#endif // QUESTION_H
