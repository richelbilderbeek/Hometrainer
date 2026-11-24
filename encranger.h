#ifndef ENCRANGER_H
#define ENCRANGER_H

#include <string>
#include <vector>

namespace ribi {

///Encranger stands for 'ENCryption by RAndom Number GEneratoR'.
///It is a simple encryption, used to prevent students from
///reading the answers of a test or changing their test results.
///
///The key is the seed of a random number generator that creates
///a table of shifts. Characters that are not in the character set
///(for example, letters with accents) are not encrypted.
///The encryption is the same on all operating systems
struct Encranger
{
  explicit Encranger(const int key);

  ///Decrypt an encrypted std::string
  std::string Deencrypt(std::string s) const noexcept;

  ///Encrypt a std::string
  std::string Encrypt(std::string s) const noexcept;

  ///Obtain the characters that are encrypted
  const std::vector<char>& GetCharacters() const noexcept { return m_characters; }

  ///Obtain the key
  int GetKey() const noexcept { return m_key; }

  private:

  ///The characters that are encrypted
  const std::vector<char> m_characters;

  ///The key used
  const int m_key;

  ///The table of shifts, created from the key
  const std::vector<int> m_table;

  char Deencrypt(const char c, const int d) const noexcept;
  char Encrypt(const char c, const int d) const noexcept;

  ///Obtain the index of a character, -1 if the character is not encrypted
  int GetIndex(const char c) const noexcept;
};

///Create the characters that are encrypted
std::vector<char> CreateEncrangerCharacters() noexcept;

///Create the table of shifts from a key
std::vector<int> CreateEncrangerTable(const int key, const int sz) noexcept;

void TestEncranger() noexcept;

} //~namespace ribi

#endif // ENCRANGER_H
