#include "hometrainermenudialog.h"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <stdexcept>

#include "hometrainerhelper.h"
#include "hometrainermaindialog.h"
#include "hometrainerprintabletest.h"
#include "hometrainerresources.h"
#include "hometrainertestfile.h"
#include "hometrainertestscorer.h"
#include "question.h"

namespace ribi {

///The values of the command line arguments
struct HometrainerArguments
{
  std::string m_command;
  std::vector<std::string> m_filenames;
  std::string m_code;
  std::string m_format{"html"};
  int m_key{0};
  bool m_has_key{false};
  std::string m_name;
  int m_n_questions{1000000};
  std::string m_output;
  int m_penalty{0};
};

///Convert a std::string to an int.
///Throws std::invalid_argument if this is impossible
int ToInt(const std::string& s, const std::string& what)
{
  try
  {
    std::size_t n_chars_read = 0;
    const int i = std::stoi(s, &n_chars_read);
    if (n_chars_read == s.size()) return i;
  }
  catch (std::exception&)
  {
    //Will throw below
  }
  throw std::invalid_argument("The " + what + " must be a whole number, instead of '" + s + "'");
}

///Parse the command line arguments.
///Throws std::invalid_argument if this fails
HometrainerArguments ParseArguments(const std::vector<std::string>& argv)
{
  const std::vector<std::string> commands = {
    "-a", "--about",
    "-h", "--help",
    "-v", "--version",
    "-e", "--example",
    "-f", "--file",
    "--create-test",
    "--take-test",
    "--score-tests",
    "--create-printable-test"
  };
  HometrainerArguments a;
  const int argc = static_cast<int>(argv.size());
  for (int i = 1; i < argc; ++i)
  {
    const std::string& arg = argv[static_cast<std::size_t>(i)];
    const auto next = [&]()
    {
      if (i + 1 >= argc) throw std::invalid_argument("Option '" + arg + "' needs a value");
      ++i;
      return argv[static_cast<std::size_t>(i)];
    };
    if (std::count(commands.begin(), commands.end(), arg))
    {
      if (!a.m_command.empty()) throw std::invalid_argument("Only one command can be given");
      a.m_command = arg;
    }
    else if (arg == "-k" || arg == "--key") { a.m_key = ToInt(next(), "key"); a.m_has_key = true; }
    else if (arg == "-n" || arg == "--questions") { a.m_n_questions = ToInt(next(), "number of questions"); }
    else if (arg == "-o" || arg == "--output") { a.m_output = next(); }
    else if (arg == "-p" || arg == "--penalty") { a.m_penalty = ToInt(next(), "penalty"); }
    else if (arg == "--name") { a.m_name = next(); }
    else if (arg == "--code") { a.m_code = next(); }
    else if (arg == "--format") { a.m_format = next(); }
    else if (!arg.empty() && arg[0] == '-') { throw std::invalid_argument("Unknown option '" + arg + "'"); }
    else { a.m_filenames.push_back(arg); }
  }
  //A filename only means: do that exercise
  if (a.m_command.empty() && !a.m_filenames.empty()) a.m_command = "--file";
  return a;
}

///Get the one input filename, throws std::invalid_argument if there is none
std::string GetOneFilename(const HometrainerArguments& a)
{
  if (a.m_filenames.size() != 1)
  {
    throw std::invalid_argument("Please supply exactly one filename");
  }
  if (!IsRegularFile(a.m_filenames[0]))
  {
    throw std::invalid_argument("File '" + a.m_filenames[0] + "' not found");
  }
  return a.m_filenames[0];
}

///Check that the key is given, throws std::invalid_argument if not
void CheckKey(const HometrainerArguments& a)
{
  if (!a.m_has_key) throw std::invalid_argument("Please supply a key, using '--key'");
}

///Check that the output is given, throws std::invalid_argument if not
void CheckOutput(const HometrainerArguments& a)
{
  if (a.m_output.empty()) throw std::invalid_argument("Please supply an output filename, using '--output'");
}

///Show lines on the screen
void Show(const std::vector<std::string>& v)
{
  for (const auto& s: v) { std::cout << s << '\n'; }
}

} //~namespace ribi

ribi::HometrainerMenuDialog::HometrainerMenuDialog()
{

}

std::vector<std::string> ribi::ConvertArguments(const int argc, const char * const argv[]) noexcept
{
  std::vector<std::string> v;
  for (int i = 0; i != argc; ++i) { v.push_back(argv[i]); }
  return v;
}

void ribi::HometrainerMenuDialog::CreateExamples()
{
  const HometrainerResources r;
}

int ribi::HometrainerMenuDialog::Execute(const std::vector<std::string>& argv) noexcept
{
  try
  {
    return ExecuteImpl(argv);
  }
  catch (std::exception& e)
  {
    std::cout << "Error: " << e.what() << '\n'
      << "Use 'Hometrainer --help' for help\n";
    return 1;
  }
}

int ribi::HometrainerMenuDialog::ExecuteImpl(const std::vector<std::string>& argv)
{
  const HometrainerArguments a = ParseArguments(argv);
  if (a.m_command.empty() || a.m_command == "-h" || a.m_command == "--help")
  {
    Show(GetHelp());
    return a.m_command.empty() ? 1 : 0;
  }
  if (a.m_command == "-a" || a.m_command == "--about")
  {
    Show(GetAbout());
    return 0;
  }
  if (a.m_command == "-v" || a.m_command == "--version")
  {
    std::cout << GetVersion() << '\n';
    return 0;
  }
  if (a.m_command == "-e" || a.m_command == "--example")
  {
    CreateExamples();
    std::cout << "Created example exercises in the current folder\n";
    return 0;
  }
  if (a.m_command == "-f" || a.m_command == "--file")
  {
    HometrainerMainDialog d(GetOneFilename(a));
    d.Execute();
    return 0;
  }
  if (a.m_command == "--create-test")
  {
    CheckKey(a);
    CheckOutput(a);
    const auto v = CreateTest(FileToVector(GetOneFilename(a)), a.m_n_questions, a.m_key);
    SaveContainer(v, a.m_output);
    std::cout << "Created test with " << (v.size() - 1) << " questions in '" << a.m_output << "'\n";
    return 0;
  }
  if (a.m_command == "--take-test")
  {
    CheckKey(a);
    if (Trim(a.m_name).empty()) throw std::invalid_argument("Please supply your full name, using '--name'");
    const std::string filename = GetOneFilename(a);
    HometrainerTestSession session;
    session.m_username = Trim(a.m_name);
    session.m_key = a.m_key;
    session.m_results_filename = a.m_output.empty() ? session.m_username + ".txt" : a.m_output;
    HometrainerMainDialog d(LoadTest(filename, a.m_key), session);
    d.Execute();
    return 0;
  }
  if (a.m_command == "--score-tests")
  {
    CheckKey(a);
    if (a.m_filenames.empty()) throw std::invalid_argument("Please supply one or more test results");
    HometrainerTestScorer scorer(a.m_key, a.m_penalty);
    for (const auto& filename: a.m_filenames) { scorer.Add(filename); }
    Show(scorer.StudentScoresToCsv());
    std::cout << '\n';
    Show(scorer.QuestionScoresToCsv());
    if (!a.m_output.empty())
    {
      SaveContainer(scorer.StudentScoresToCsv(), a.m_output);
      std::cout << "\nSaved student scores to '" << a.m_output << "'\n";
    }
    return 0;
  }
  if (a.m_command == "--create-printable-test")
  {
    CheckOutput(a);
    if (a.m_format != "html" && a.m_format != "text")
    {
      throw std::invalid_argument("The format must be 'html' or 'text', instead of '" + a.m_format + "'");
    }
    const auto v = CreatePrintableTest(
      FileToVector(GetOneFilename(a)),
      a.m_n_questions,
      a.m_format == "html" ? PrintableTestFormat::html : PrintableTestFormat::text,
      a.m_code
    );
    SaveContainer(v, a.m_output);
    std::cout << "Created printable test in '" << a.m_output << "'\n";
    return 0;
  }
  assert(!"Should not get here");
  Show(GetHelp());
  return 1;
}

std::vector<std::string> ribi::HometrainerMenuDialog::GetAbout() const noexcept
{
  std::vector<std::string> v = {
    "Hometrainer, version " + GetVersion(),
    "Copyright (C) 2009-2026 Richel Bilderbeek",
    "",
    "Hometrainer is a tool to make exercises and tests",
    "with open questions and multiple choice questions,",
    "with or without images, to practice, to examine and to grade.",
    "",
    "This program is free software: you can redistribute it and/or modify",
    "it under the terms of the GNU General Public License as published by",
    "the Free Software Foundation, either version 3 of the License, or",
    "(at your option) any later version.",
    "",
    "Source code: https://github.com/richelbilderbeek/Hometrainer",
    "Website: https://richelbilderbeek.github.io/Hometrainer/",
    "",
    "Version history:"
  };
  for (const auto& s: GetVersionHistory()) { v.push_back(" - " + s); }
  return v;
}

std::vector<std::string> ribi::HometrainerMenuDialog::GetExample() const noexcept
{
  return {
    "Hometrainer (C) 2009-2026 Richel Bilderbeek",
    "Humanbody.png,Which line points to the liver?,1,2,3,4",
    "Humanbody.png,Which line points to the liver?,1/one/One",
    "Humanbody.png,Which organ does line 1 point to?,The liver/the liver/Liver/liver",
    "-,In which organ is ethanol detoxified?,The liver/the liver/Liver/liver",
    "-,What is 1\\,5 + 1\\,5?,3"
  };
}

std::vector<std::string> ribi::HometrainerMenuDialog::GetHelp() const noexcept
{
  return {
    "Hometrainer, tool to make exercises and tests",
    "",
    "Usage:",
    "  Hometrainer [command] [options] [filename(s)]",
    "",
    "Commands:",
    "  -a, --about                    show information about Hometrainer",
    "  -h, --help                     show this help",
    "  -v, --version                  show the version",
    "  -e, --example                  create example exercises in the current folder",
    "  -f, --file                     do an exercise",
    "      --create-test              create an encrypted test from an exercise",
    "      --take-test                do a test, the results are saved encrypted",
    "      --score-tests              grade the results of one or more tests",
    "      --create-printable-test    create a test on paper from an exercise",
    "",
    "Options:",
    "  -k, --key [number]             the key used to encrypt a test",
    "  -n, --questions [number]       the maximum number of questions in a test",
    "  -o, --output [filename]        the file to save to",
    "  -p, --penalty [number]         number of questions expected to be correct by guessing",
    "      --name [full name]         the full name of the student doing a test",
    "      --code [letters]           answer code for multiple choice questions on paper",
    "      --format [html or text]    the format of a printable test, 'html' by default",
    "",
    "Examples:",
    "  Hometrainer my_exercise.txt",
    "  Hometrainer --file my_exercise.txt",
    "  Hometrainer --create-test my_exercise.txt --key 1234 --questions 10 --output my_test.txt",
    "  Hometrainer --take-test my_test.txt --key 1234 --name \"John Doe\"",
    "  Hometrainer --score-tests \"John Doe.txt\" \"Jane Doe.txt\" --key 1234 --output scores.csv",
    "  Hometrainer --create-printable-test my_exercise.txt --code HOMETRAINER --output my_test.html"
  };
}

std::string ribi::HometrainerMenuDialog::GetVersion() const noexcept
{
  return "4.0";
}

std::vector<std::string> ribi::HometrainerMenuDialog::GetVersionHistory() const noexcept
{
  return {
    "2009-xx-xx: Version 0.9: initial version (called HomeTrainer)",
    "2009-04-28: Version 1.0: renamed 'HomeTrainer' to 'Hometrainer 2'",
    "2009-05-09: Version 1.1",
    "2009-08-23: Version 1.2",
    "2009-08-29: Version 1.3",
    "2009-09-20: Version 1.4",
    "2009-10-29: Version 1.5",
    "2009-11-03: Version 1.6",
    "2010-01-07: Version 1.7",
    "2010-01-28: Version 1.8: complete suite: exercises, tests, grading and tests on paper",
    "2011-10-16: Version 2.0: initial website version, renamed 'Hometrainer 2' to 'Hometrainer'",
    "2013-10-23: Version 3.0: major architectural rewrite, added console version",
    "2026-11-24: Version 4.0: ported the complete suite of version 1.8 to Qt, removed website version"
  };
}

void ribi::TestHometrainerMenuDialog() noexcept
{
  HometrainerMenuDialog d;
  assert(!d.GetAbout().empty());
  assert(!d.GetHelp().empty());
  assert(!d.GetVersion().empty());
  assert(CreateQuestions(d.GetExample()).size() == 5);
  assert(d.Execute( { "Hometrainer", "--version" } ) == 0);
  assert(d.Execute( { "Hometrainer", "--nonsense" } ) == 1);
  assert(d.Execute( { "Hometrainer", "--create-test" } ) == 1);

  //Create a test, then score empty results
  {
    const std::string exercise = "hometrainer_menu_test_exercise_tmp.txt";
    const std::string test = "hometrainer_menu_test_test_tmp.txt";
    const std::string printable = "hometrainer_menu_test_printable_tmp.html";
    SaveContainer(d.GetExample(), exercise);
    assert(d.Execute( { "Hometrainer", "--create-test", exercise, "--key", "42", "--questions", "3", "--output", test } ) == 0);
    assert(LoadTest(test, 42).size() == 3);
    assert(d.Execute( { "Hometrainer", "--create-printable-test", exercise, "--code", "abc", "--output", printable } ) == 0);
    assert(IsRegularFile(printable));
    std::remove(exercise.c_str());
    std::remove(test.c_str());
    std::remove(printable.c_str());
  }
}
