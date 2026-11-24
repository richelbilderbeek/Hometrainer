#include "hometrainerprintabletest.h"

#include <algorithm>
#include <cassert>
#include <memory>
#include <stdexcept>

#include "hometrainerhelper.h"
#include "multiplechoicequestion.h"
#include "openquestion.h"
#include "question.h"

namespace ribi {

///Give each answer a letter, where the correct answer gets 'correct_letter'
///and the wrong answers get other letters, preferably from the answer code
std::vector<std::string> AddLetters(
  const std::string& answer,
  const std::vector<std::string>& wrong_answers,
  const char correct_letter,
  const std::string& answer_code)
{
  std::string candidates;
  for (const char c: answer_code + "ABCDEFGHIJKLMNOPQRSTUVWXYZ")
  {
    if (c != correct_letter && candidates.find(c) == std::string::npos) candidates += c;
  }
  //Prefer the letters from the answer code, in a random order
  const auto n_from_code = std::count_if(candidates.begin(), candidates.end(),
    [answer_code](const char c) { return answer_code.find(c) != std::string::npos; }
  );
  std::shuffle(candidates.begin(), candidates.begin() + n_from_code, GetRng());
  std::shuffle(candidates.begin() + n_from_code, candidates.end(), GetRng());

  std::vector<std::string> v;
  v.push_back(std::string(1, correct_letter) + ") " + answer);
  for (std::size_t i = 0; i != wrong_answers.size(); ++i)
  {
    const char c = i < candidates.size() ? candidates[i] : '?';
    v.push_back(std::string(1, c) + ") " + wrong_answers[i]);
  }
  return v;
}

} //~namespace ribi

std::vector<std::string> ribi::CreatePrintableTest(
  const std::vector<std::string>& exercise_lines,
  const int n_questions,
  const PrintableTestFormat format,
  const std::string& answer_code_original,
  const bool shuffle
)
{
  if (n_questions < 1)
  {
    throw std::invalid_argument("A printable test must have at least one question");
  }
  //Only keep the letters of the answer code
  std::string answer_code;
  for (const char c: ToUpper(answer_code_original))
  {
    if (c >= 'A' && c <= 'Z') answer_code += c;
  }

  std::vector<std::unique_ptr<Question>> questions;
  for (const auto& line: exercise_lines)
  {
    auto q = CreateQuestion(line);
    if (q) questions.push_back(std::move(q));
  }
  if (questions.empty())
  {
    throw std::invalid_argument("The exercise has no questions");
  }
  if (shuffle) std::shuffle(questions.begin(), questions.end(), GetRng());
  if (static_cast<int>(questions.size()) > n_questions)
  {
    questions.resize(static_cast<std::size_t>(n_questions));
  }

  const bool html = format == PrintableTestFormat::html;
  const auto esc = [html](const std::string& s) { return html ? EscapeHtml(s) : s; };

  std::vector<std::string> v;
  if (html)
  {
    v.push_back("<!DOCTYPE html>");
    v.push_back("<html>");
    v.push_back("<head>");
    v.push_back("<meta charset=\"utf-8\">");
    v.push_back("<title>Hometrainer test</title>");
    v.push_back("<style>body { font-family: sans-serif; font-size: 14pt; } img { max-width: 100%; } li { list-style-type: none; }</style>");
    v.push_back("</head>");
    v.push_back("<body>");
  }
  int i = 1;
  for (const auto& q: questions)
  {
    if (q->HasImage())
    {
      v.push_back(html
        ? "<p><img src=\"" + EscapeHtml(q->GetFilename()) + "\" alt=\"" + EscapeHtml(q->GetFilename()) + "\"></p>"
        : "[Image: " + q->GetFilename() + "]"
      );
    }
    const std::string question_line = std::to_string(i) + ". " + esc(q->GetQuestion());
    v.push_back(html ? "<p>" + question_line + "</p>" : question_line);

    const MultipleChoiceQuestion * const mcq = dynamic_cast<const MultipleChoiceQuestion*>(q.get());
    if (mcq)
    {
      std::vector<std::string> answers;
      if (answer_code.empty())
      {
        answers = mcq->GetWrongAnswers();
        answers.push_back(mcq->GetAnswer());
      }
      else
      {
        const char correct_letter = answer_code[static_cast<std::size_t>(i - 1) % answer_code.size()];
        answers = AddLetters(mcq->GetAnswer(), mcq->GetWrongAnswers(), correct_letter, answer_code);
      }
      std::sort(answers.begin(), answers.end());
      if (html) v.push_back("<ul>");
      for (const auto& answer: answers)
      {
        v.push_back(html ? "<li>&#9744; " + EscapeHtml(answer) + "</li>" : "O " + answer);
      }
      if (html) v.push_back("</ul>");
    }
    else
    {
      v.push_back(html ? "<p>" + std::string(40, '_') + "</p>" : std::string(40, '_'));
    }
    if (!html) v.push_back("");
    ++i;
  }
  if (html)
  {
    v.push_back("</body>");
    v.push_back("</html>");
  }
  return v;
}

std::string ribi::EscapeHtml(const std::string& s) noexcept
{
  std::string t;
  for (const char c: s)
  {
    switch (c)
    {
      case '&': t += "&amp;"; break;
      case '<': t += "&lt;"; break;
      case '>': t += "&gt;"; break;
      case '"': t += "&quot;"; break;
      default: t += c;
    }
  }
  return t;
}

std::string ribi::GetFileExtension(const PrintableTestFormat format) noexcept
{
  return format == PrintableTestFormat::html ? ".html" : ".txt";
}

void ribi::TestHometrainerPrintableTest() noexcept
{
  const std::vector<std::string> exercise = {
    "Hometrainer (C) 2009 Richel Bilderbeek",
    "-,1+1=,2/two",
    "brain.png,Which organ is this?,Brain,Liver,Heart"
  };
  assert(EscapeHtml("<b>&\"") == "&lt;b&gt;&amp;&quot;");
  assert(GetFileExtension(PrintableTestFormat::html) == ".html");
  //Text
  {
    const auto v = CreatePrintableTest(exercise, 10, PrintableTestFormat::text, "", false);
    const std::vector<std::string> expected = {
      "1. 1+1=",
      std::string(40, '_'),
      "",
      "[Image: brain.png]",
      "2. Which organ is this?",
      "O Brain",
      "O Heart",
      "O Liver",
      ""
    };
    assert(v == expected);
  }
  //Text with an answer code
  {
    const auto v = CreatePrintableTest(exercise, 10, PrintableTestFormat::text, "xy", false);
    //The correct answer of question 2 gets the second letter of the code
    assert(std::count(v.begin(), v.end(), "O Y) Brain") == 1);
  }
  //HTML
  {
    const auto v = CreatePrintableTest(exercise, 10, PrintableTestFormat::html, "", false);
    assert(v.front() == "<!DOCTYPE html>");
    assert(v.back() == "</html>");
    assert(std::count(v.begin(), v.end(), "<p>1. 1+1=</p>") == 1);
    assert(std::count(v.begin(), v.end(), "<li>&#9744; Brain</li>") == 1);
  }
  //Maximum number of questions
  {
    const auto v = CreatePrintableTest(exercise, 1, PrintableTestFormat::text);
    assert(std::count_if(v.begin(), v.end(), [](const auto& s) { return s.size() > 1 && s.substr(0,2) == "1."; }) == 1);
    assert(std::count_if(v.begin(), v.end(), [](const auto& s) { return s.size() > 1 && s.substr(0,2) == "2."; }) == 0);
  }
}
