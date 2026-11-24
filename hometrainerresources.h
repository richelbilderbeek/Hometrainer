#ifndef HOMETRAINERRESOURCES_H
#define HOMETRAINERRESOURCES_H

#include <string>
#include <vector>

namespace ribi {

struct HometrainerResources
{
  ///Creates the resources (the example exercises, their images and
  ///the manual) from the resource file in the current folder.
  ///Existing files are not overwritten.
  ///Throws a std::runtime_error if one of these cannot be created
  HometrainerResources();

  ///Obtain the filenames of the example exercises
  std::vector<std::string> GetExercises() const noexcept;

  std::string GetQuestion() const noexcept { return "ToolHometrainerQuestion.png"; }
  std::string GetExerciseClouds() const noexcept { return "ToolHometrainerExerciseClouds.txt"; }
  std::string GetExerciseTest() const noexcept { return "ToolHometrainerExerciseTest.txt"; }
  std::string GetQuestionMark() const noexcept { return "ToolHometrainerQuestionmark.png"; }
  std::string GetWelcome() const noexcept { return "ToolHometrainerWelcome.png"; }
};

void TestHometrainerResources() noexcept;

} //~namespace ribi

#endif // HOMETRAINERRESOURCES_H
