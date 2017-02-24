#ifndef HOMETRAINERMENUDIALOG_H
#define HOMETRAINERMENUDIALOG_H

#include "about.h"
#include "menudialog.h"

namespace ribi {

///The logic behind the menu dialog
struct HometrainerMenuDialog final : public MenuDialog
{
  HometrainerMenuDialog();

  ///Create the example exercises
  static void CreateExamples() noexcept;

  ///Execute Hometrainer from the command line
  ///The return code is the error code given back to main
  int ExecuteSpecific(const std::vector<std::string>& argv) noexcept override;

  About GetAbout() const noexcept override;
  Help GetHelp() const noexcept override;
  std::string GetVersion() const noexcept override;
  std::vector<std::string> GetVersionHistory() const noexcept override;
};

void TestHometrainerMenuDialog() noexcept;

} //~namespace ribi

#endif // HOMETRAINERMENUDIALOG_H
