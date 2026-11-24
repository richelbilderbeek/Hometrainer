#include "hometrainerhelper.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <fstream>
#include <stdexcept>

#include <QFileInfo>
#include <QImage>

namespace ribi {

///Find the index of the last path seperator, returns std::string::npos if
///there is none
std::string::size_type FindLastPathSeperator(const std::string& filename) noexcept
{
  return filename.find_last_of("/\\");
}

} //~namespace ribi

std::string ribi::GetFileBasename(const std::string& filename) noexcept
{
  const auto i = FindLastPathSeperator(filename);
  if (i == std::string::npos) return filename;
  return filename.substr(i + 1);
}

std::string ribi::GetPath(const std::string& filename) noexcept
{
  const auto i = FindLastPathSeperator(filename);
  if (i == std::string::npos) return "";
  return filename.substr(0, i);
}

std::mt19937& ribi::GetRng() noexcept
{
  static std::mt19937 rng{std::random_device{}()};
  return rng;
}

std::vector<std::string> ribi::ImageToAsciiArt(
  const std::string& filename,
  const int max_cols,
  const int max_rows
) noexcept
{
  const QImage original(QString::fromStdString(filename));
  if (original.isNull() || max_cols < 1 || max_rows < 1) return {};

  //A character is about twice as high as it is wide
  const double aspect_ratio = 0.5
    * static_cast<double>(original.height())
    / static_cast<double>(original.width());
  int n_cols = max_cols;
  int n_rows = std::max(1, static_cast<int>(aspect_ratio * n_cols));
  if (n_rows > max_rows)
  {
    n_rows = max_rows;
    n_cols = std::max(1, static_cast<int>(static_cast<double>(n_rows) / aspect_ratio));
  }
  const QImage image = original.scaled(n_cols, n_rows, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

  //From dark to light
  const std::string palette = "@%#*+=-:. ";
  const int n_shades = static_cast<int>(palette.size());
  std::vector<std::string> v;
  for (int y = 0; y != image.height(); ++y)
  {
    std::string line;
    for (int x = 0; x != image.width(); ++x)
    {
      const int grey = qGray(image.pixel(x, y));
      const int index = std::min(n_shades - 1, grey * n_shades / 256);
      line += palette[static_cast<std::size_t>(index)];
    }
    v.push_back(line);
  }
  return v;
}

bool ribi::IsRegularFile(const std::string& filename) noexcept
{
  if (filename.empty()) return false;
  const QFileInfo f(QString::fromStdString(filename));
  return f.exists() && f.isFile();
}

std::vector<std::string> ribi::FileToVector(const std::string& filename)
{
  if (!IsRegularFile(filename))
  {
    throw std::runtime_error("File '" + filename + "' does not exist");
  }
  std::ifstream f(filename.c_str());
  if (!f.is_open())
  {
    throw std::runtime_error("File '" + filename + "' cannot be read");
  }
  std::vector<std::string> v;
  std::string s;
  while (std::getline(f, s))
  {
    if (!s.empty() && s.back() == '\r') s.pop_back();
    v.push_back(s);
  }
  return v;
}

std::string ribi::JoinPath(const std::string& path, const std::string& filename) noexcept
{
  if (path.empty() || filename.empty()) return filename;
  if (filename[0] == '/' || filename[0] == '\\') return filename;
  if (filename.size() > 1 && filename[1] == ':') return filename; //C:\\file.txt
  if (filename.size() > 1 && filename[0] == ':' && filename[1] == '/') return filename; //Qt resource
  return path + "/" + filename;
}

std::string ribi::RemoveExtension(const std::string& filename) noexcept
{
  const auto dot = filename.rfind('.');
  const auto sep = FindLastPathSeperator(filename);
  if (dot == std::string::npos) return filename;
  if (sep != std::string::npos && dot < sep) return filename;
  return filename.substr(0, dot);
}

std::string ribi::ReplaceAll(
  std::string s,
  const std::string& replace_what,
  const std::string& replace_with_what
) noexcept
{
  if (replace_what.empty()) return s;
  std::string::size_type pos = 0;
  while ((pos = s.find(replace_what, pos)) != std::string::npos)
  {
    s.replace(pos, replace_what.size(), replace_with_what);
    pos += replace_with_what.size();
  }
  return s;
}

std::vector<std::string> ribi::ReplaceAll(
  std::vector<std::string> v,
  const std::string& replace_what,
  const std::string& replace_with_what
) noexcept
{
  for (auto& s: v) { s = ReplaceAll(s, replace_what, replace_with_what); }
  return v;
}

void ribi::SaveContainer(const std::vector<std::string>& v, const std::string& filename)
{
  std::ofstream f(filename.c_str());
  if (!f.is_open())
  {
    throw std::runtime_error("File '" + filename + "' cannot be written to");
  }
  for (const auto& s: v) { f << s << '\n'; }
}

std::vector<std::string> ribi::SeperateString(
  const std::string& input,
  const char seperator
) noexcept
{
  std::vector<std::string> v;
  std::string::size_type begin = 0;
  while (1)
  {
    const auto end = input.find(seperator, begin);
    if (end == std::string::npos)
    {
      v.push_back(input.substr(begin));
      break;
    }
    v.push_back(input.substr(begin, end - begin));
    begin = end + 1;
  }
  return v;
}

std::vector<std::string> ribi::SeperateHometrainerLine(const std::string& input) noexcept
{
  const std::string placeholder{"\x01"};
  const std::string s = ReplaceAll(input, "\\,", placeholder);
  return ReplaceAll(SeperateString(s, ','), placeholder, ",");
}

std::string ribi::ToUpper(std::string s) noexcept
{
  std::transform(s.begin(), s.end(), s.begin(),
    [](const char c) { return static_cast<char>(std::toupper(static_cast<unsigned char>(c))); }
  );
  return s;
}

std::string ribi::Trim(const std::string& s) noexcept
{
  const auto is_space = [](const char c) { return std::isspace(static_cast<unsigned char>(c)) != 0; };
  const auto begin = std::find_if_not(s.begin(), s.end(), is_space);
  const auto end = std::find_if_not(s.rbegin(), s.rend(), is_space).base();
  if (begin >= end) return "";
  return std::string(begin, end);
}

void ribi::TestHometrainerHelper() noexcept
{
  assert(GetFileBasename("Test.txt") == "Test.txt");
  assert(GetFileBasename("C:\\Test.txt") == "Test.txt");
  assert(GetFileBasename("C:\\Test\\Test.txt") == "Test.txt");
  assert(GetFileBasename("/home/me/Test.txt") == "Test.txt");

  assert(GetPath("f.txt").empty());
  assert(GetPath("C:\\f.txt") == "C:");
  assert(GetPath("C:\\games\\f.txt") == "C:\\games");
  assert(GetPath("/home/me/f.txt") == "/home/me");

  assert(JoinPath("", "f.txt") == "f.txt");
  assert(JoinPath("/home/me", "f.txt") == "/home/me/f.txt");
  assert(JoinPath("/home/me", "/tmp/f.txt") == "/tmp/f.txt");
  assert(JoinPath("/home/me", "C:\\f.txt") == "C:\\f.txt");

  assert(RemoveExtension("Test.txt") == "Test");
  assert(RemoveExtension("C:\\Test.txt") == "C:\\Test");
  assert(RemoveExtension("C:\\Test\\Test.txt") == "C:\\Test\\Test");
  assert(RemoveExtension("my.folder/Test") == "my.folder/Test");

  assert(ReplaceAll("aXbXc", "X", "YY") == "aYYbYYc");
  assert(ReplaceAll("aaa", "a", "aa") == "aaaaaa");

  assert(SeperateString("a,b,c", ',') == std::vector<std::string>({"a", "b", "c"}));
  assert(SeperateString("a,,c", ',') == std::vector<std::string>({"a", "", "c"}));
  assert(SeperateString("", ',') == std::vector<std::string>({""}));

  assert(SeperateHometrainerLine("a,b\\,c,d") == std::vector<std::string>({"a", "b,c", "d"}));

  assert(ToUpper("abC1") == "ABC1");
  assert(Trim("  a b  ") == "a b");
  assert(Trim("   ").empty());
}
