#include "encranger.h"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <numeric>
#include <random>

ribi::Encranger::Encranger(const int key)
  : m_characters(CreateEncrangerCharacters()),
    m_key{key},
    m_table(CreateEncrangerTable(key, static_cast<int>(m_characters.size())))
{

}

std::vector<char> ribi::CreateEncrangerCharacters() noexcept
{
  const std::string s =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "!@#$%^&*()"
    "~`-_=+[]{};:'\"<>,./?\\| "
  ;
  return std::vector<char>(s.begin(), s.end());
}

std::vector<int> ribi::CreateEncrangerTable(const int key, const int sz) noexcept
{
  assert(sz > 0);
  std::vector<int> v(static_cast<std::size_t>(sz));
  std::iota(v.begin(), v.end(), 0);

  //Shuffle with an own Fisher-Yates shuffle,
  //as std::shuffle and std::uniform_int_distribution
  //give different results on different standard libraries
  std::mt19937 rng(static_cast<std::uint32_t>(key));
  for (int i = sz - 1; i > 0; --i)
  {
    const int j = static_cast<int>(rng() % static_cast<std::uint32_t>(i + 1));
    std::swap(v[static_cast<std::size_t>(i)], v[static_cast<std::size_t>(j)]);
  }
  return v;
}

std::string ribi::Encranger::Deencrypt(std::string s) const noexcept
{
  const std::size_t n = m_table.size();
  for (std::size_t i = 0; i != s.size(); ++i)
  {
    s[i] = Deencrypt(s[i], m_table[i % n]);
  }
  return s;
}

char ribi::Encranger::Deencrypt(const char c, const int d) const noexcept
{
  const int i = GetIndex(c);
  if (i == -1) return c;
  const int n_chars = static_cast<int>(m_characters.size());
  const int i_new = (i - d + n_chars) % n_chars;
  assert(i_new >= 0);
  assert(i_new < n_chars);
  return m_characters[static_cast<std::size_t>(i_new)];
}

std::string ribi::Encranger::Encrypt(std::string s) const noexcept
{
  const std::size_t n = m_table.size();
  for (std::size_t i = 0; i != s.size(); ++i)
  {
    s[i] = Encrypt(s[i], m_table[i % n]);
  }
  return s;
}

char ribi::Encranger::Encrypt(const char c, const int d) const noexcept
{
  const int i = GetIndex(c);
  if (i == -1) return c;
  const int n_chars = static_cast<int>(m_characters.size());
  const int i_new = (i + d) % n_chars;
  assert(i_new >= 0);
  assert(i_new < n_chars);
  return m_characters[static_cast<std::size_t>(i_new)];
}

int ribi::Encranger::GetIndex(const char c) const noexcept
{
  const auto i = std::find(m_characters.begin(), m_characters.end(), c);
  if (i == m_characters.end()) return -1;
  return static_cast<int>(std::distance(m_characters.begin(), i));
}

void ribi::TestEncranger() noexcept
{
  //Encryption and decryption are each other's inverse
  {
    const std::vector<std::string> texts = {
      "",
      "Hometrainer (C) 2009 Richel Bilderbeek",
      "-,1+1=,2/two,3\\,5",
      "Unknown characters are kept: \xC3\xA9\t"
    };
    for (const int key: { 0, 1, 42, 12345, -1 })
    {
      const Encranger e(key);
      for (const auto& text: texts)
      {
        assert(e.Deencrypt(e.Encrypt(text)) == text);
      }
    }
  }
  //Encryption changes the text
  {
    const Encranger e(42);
    const std::string s = "Hometrainer (C) 2009 Richel Bilderbeek";
    assert(e.Encrypt(s) != s);
  }
  //Different keys give different encryptions
  {
    const std::string s = "Hometrainer (C) 2009 Richel Bilderbeek";
    assert(Encranger(1).Encrypt(s) != Encranger(2).Encrypt(s));
  }
  //Decrypting with the wrong key does not give the original text
  {
    const std::string s = "Hometrainer (C) 2009 Richel Bilderbeek";
    assert(Encranger(2).Deencrypt(Encranger(1).Encrypt(s)) != s);
  }
  //The encryption is the same on all platforms
  {
    const std::vector<int> v = CreateEncrangerTable(42, 10);
    std::vector<int> w = v;
    std::sort(w.begin(), w.end());
    assert(w == std::vector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
    assert(v == CreateEncrangerTable(42, 10));
  }
}
