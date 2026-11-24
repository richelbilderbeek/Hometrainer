#ifndef HOMETRAINERTESTFILE_H
#define HOMETRAINERTESTFILE_H

#include <memory>
#include <string>
#include <vector>

namespace ribi {

struct Question;

///Create a test from an exercise.
///A test is an exercise of which the questions are shuffled,
///of which at most 'n_questions' are kept, and of which all lines are
///encrypted with the key. The first line of the test is the
///Hometrainer header, which is used to check the key when loading the test.
///Throws std::invalid_argument if the exercise has no questions
///or if 'n_questions' is less than one
std::vector<std::string> CreateTest(
  const std::vector<std::string>& exercise_lines,
  const int n_questions,
  const int key
);

///Decrypt the lines of a test.
///Throws std::invalid_argument if the key is incorrect
std::vector<std::string> DecryptTest(
  const std::vector<std::string>& test_lines,
  const int key
);

///Obtain the first line of a Hometrainer file
std::string GetHometrainerHeader() noexcept;

///Is the line a Hometrainer header?
///All header lines start with 'Hometrainer', for example
///'Hometrainer (C) 2009 Richel Bilderbeek' and
///'Hometrainer2 (C) 2009 Richel Bilderbeek'
bool IsHometrainerHeader(const std::string& line) noexcept;

///Load the questions of a test from file.
///Image filenames are relative to the folder of the file.
///Throws std::runtime_error if the file does not exist
///Throws std::invalid_argument if the key is incorrect
std::vector<std::shared_ptr<const Question>> LoadTest(
  const std::string& filename,
  const int key
);

void TestHometrainerTestFile() noexcept;

} //~namespace ribi

#endif // HOMETRAINERTESTFILE_H
