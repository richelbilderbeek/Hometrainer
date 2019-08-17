#ifndef EXERCISE_H
#define EXERCISE_H

#include <string>
#include <vector>

namespace ribi {

///A Exercise is a collection of questions
struct Exercise
{
  ///Construct a Exercise from file
  ///The file needs to contain at least one question
  ///Throws std::logic_error if file does not exist
  ///Throws std::runtime_error if file does not contain a single question
  explicit Exercise(const std::string& filename);

  ///Read the current question
  std::string GetCurrentQuestion() const noexcept;

  ///Get the number of questions, will be at least one
  int GetNumberOfQuestions() const noexcept;

  ///Obtain this class its version
  static std::string GetVersion() noexcept;

  ///Obtain this class its version history
  static std::vector<std::string> GetVersionHistory() noexcept;

  ///Go to the next question
  void Next() noexcept;

  private:

  ///An iterator pointing to the current question
  std::vector<std::string>::iterator m_current;

  ///The questions
  std::vector<std::string> m_questions;
};

} //~namespace ribi

#endif // EXERCISE_H
