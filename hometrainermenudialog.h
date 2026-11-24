#ifndef HOMETRAINERMENUDIALOG_H
#define HOMETRAINERMENUDIALOG_H

#include <string>
#include <vector>

namespace ribi {

///The logic behind the menu dialog
struct HometrainerMenuDialog final
{
  HometrainerMenuDialog();

  ///Create the example exercises in the current folder
  static void CreateExamples();

  ///Execute Hometrainer from the command line.
  ///The first element of argv is the program name.
  ///The return code is the error code given back to main
  int Execute(const std::vector<std::string>& argv) noexcept;

  ///Obtain information about this program, one line per element
  std::vector<std::string> GetAbout() const noexcept;

  ///Obtain an example exercise, one line per element
  std::vector<std::string> GetExample() const noexcept;

  ///Obtain the help text of the command line version, one line per element
  std::vector<std::string> GetHelp() const noexcept;

  std::string GetVersion() const noexcept;
  std::vector<std::string> GetVersionHistory() const noexcept;

  private:
  ///Execute Hometrainer from the command line, which may throw
  int ExecuteImpl(const std::vector<std::string>& argv);
};

///Convert the arguments of main to a std::vector
std::vector<std::string> ConvertArguments(const int argc, const char * const argv[]) noexcept;

void TestHometrainerMenuDialog() noexcept;

} //~namespace ribi

#endif // HOMETRAINERMENUDIALOG_H
