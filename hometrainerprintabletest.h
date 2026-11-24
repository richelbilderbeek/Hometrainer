#ifndef HOMETRAINERPRINTABLETEST_H
#define HOMETRAINERPRINTABLETEST_H

#include <string>
#include <vector>

namespace ribi {

///The formats a printable test can be created in
enum class PrintableTestFormat
{
  html, ///A web page, that can be opened and printed by a browser or word processor
  text  ///Plain text
};

///Create a printable test (that is, a test on paper) from an exercise.
///The questions are numbered. For multiple choice questions, the possible
///answers are sorted. For open questions, a line to write on is added.
///
///If an answer code is given, each answer of a multiple choice question
///gets a letter. The letters of the correct answers spell the answer code,
///which makes grading easier. For example, the code 'HOMETRAINER' gives
///the correct answers 'H) ...', 'O) ...', 'M) ...', etc.
///
///'n_questions' is the maximum number of questions.
///If 'shuffle' is true, the questions are shuffled before selecting these
std::vector<std::string> CreatePrintableTest(
  const std::vector<std::string>& exercise_lines,
  const int n_questions,
  const PrintableTestFormat format,
  const std::string& answer_code = "",
  const bool shuffle = true
);

///Escape the characters that have a special meaning in HTML
std::string EscapeHtml(const std::string& s) noexcept;

///Obtain the file extension (including the dot) for a format
std::string GetFileExtension(const PrintableTestFormat format) noexcept;

void TestHometrainerPrintableTest() noexcept;

} //~namespace ribi

#endif // HOMETRAINERPRINTABLETEST_H
