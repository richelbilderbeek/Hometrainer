#ifndef HOMETRAINERHELPER_H
#define HOMETRAINERHELPER_H

#include <random>
#include <string>
#include <vector>

namespace ribi {

///Remove the path from a filename.
///Both slashes and backslashes are seen as path seperators
///For example, 'C:\\my_folder\\my_file.txt' becomes 'my_file.txt'
std::string GetFileBasename(const std::string& filename) noexcept;

///Obtain the path of a filename, without a trailing path seperator.
///Both slashes and backslashes are seen as path seperators.
///Will return an empty string if there is no path
///For example, '/home/me/my_file.txt' becomes '/home/me'
std::string GetPath(const std::string& filename) noexcept;

///The random number generator used by Hometrainer.
///It is seeded randomly at its first use
std::mt19937& GetRng() noexcept;

///Convert an image to ASCII art, that fits within max_cols columns
///and max_rows rows. Returns an empty std::vector if the image
///cannot be loaded
std::vector<std::string> ImageToAsciiArt(
  const std::string& filename,
  const int max_cols = 78,
  const int max_rows = 20
) noexcept;

///Is the file a regular file that exists?
bool IsRegularFile(const std::string& filename) noexcept;

///Read a file into a std::vector, one line per element.
///A carriage return at the end of a line is removed.
///Throws std::runtime_error if the file cannot be read
std::vector<std::string> FileToVector(const std::string& filename);

///Join a path and a filename, using a slash as a path seperator.
///If the path is empty or the filename is an absolute path,
///the filename is returned unmodified
std::string JoinPath(const std::string& path, const std::string& filename) noexcept;

///Remove the extension from a filename,
///for example, 'C:\\my_folder\\my_file.txt' becomes 'C:\\my_folder\\my_file'
std::string RemoveExtension(const std::string& filename) noexcept;

///Replace all occurrences of 'replace_what' by 'replace_with_what'
std::string ReplaceAll(
  std::string s,
  const std::string& replace_what,
  const std::string& replace_with_what
) noexcept;

///Replace all occurrences of 'replace_what' by 'replace_with_what',
///for every element
std::vector<std::string> ReplaceAll(
  std::vector<std::string> v,
  const std::string& replace_what,
  const std::string& replace_with_what
) noexcept;

///Save a std::vector to file, one line per element.
///Throws std::runtime_error if the file cannot be written
void SaveContainer(const std::vector<std::string>& v, const std::string& filename);

///Split a std::string on every seperator.
///For example, 'a,b,c' with ',' becomes {'a','b','c'}
///An empty string results in one empty element
std::vector<std::string> SeperateString(
  const std::string& input,
  const char seperator
) noexcept;

///Split a line of a Hometrainer file on its commas.
///A comma that is preceded by a backslash ('\,') is not used to split,
///and becomes a regular comma in the result.
///For example, 'a,b\\,c,d' becomes {'a','b,c','d'}
std::vector<std::string> SeperateHometrainerLine(const std::string& input) noexcept;

///Convert a std::string to uppercase
std::string ToUpper(std::string s) noexcept;

///Remove whitespace from both sides of a std::string
std::string Trim(const std::string& s) noexcept;

void TestHometrainerHelper() noexcept;

} //~namespace ribi

#endif // HOMETRAINERHELPER_H
