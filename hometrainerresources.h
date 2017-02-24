#ifndef HOMETRAINERRESOURCES_H
#define HOMETRAINERRESOURCES_H

#include <string>

namespace ribi {

struct HometrainerResources
{
  ///Creates the resources from the resource file
  ///Throws a std::runtime_error if one of these is not found
  HometrainerResources();
  std::string GetQuestion() const noexcept { return "ToolHometrainerQuestion.png"; }
  std::string GetExerciseClouds() const noexcept { return "ToolHometrainerExerciseClouds.txt"; }
  std::string GetExerciseTest() const noexcept { return "ToolHometrainerExerciseTest.txt"; }
  std::string GetQuestionMark() const noexcept { return "ToolHometrainerQuestionmark.png"; }
  std::string GetWelcome() const noexcept { return "ToolHometrainerWelcome.png"; }
};

} //~namespace ribi

#endif // HOMETRAINERRESOURCES_H
