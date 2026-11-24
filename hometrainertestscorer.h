#ifndef HOMETRAINERTESTSCORER_H
#define HOMETRAINERTESTSCORER_H

#include <string>
#include <vector>

namespace ribi {

///The score of one student
struct HometrainerStudentScore
{
  ///The name of the student, as obtained from the filename
  std::string m_name;

  ///The number of correctly answered questions
  int m_n_correct;

  ///The number of questions answered
  int m_n_questions;

  ///The grade, from 1.0 to 10.0
  double m_grade;

  ///Comment, for example if the results are suspicious
  std::string m_comment;
};

///The score of one question, over all students
struct HometrainerQuestionScore
{
  ///The question its ID, see GetQuestionId
  std::string m_question;

  ///The number of students that answered this question correctly
  int m_n_correct;

  ///The number of students that answered this question
  int m_n_answered;

  ///The fraction of students that answered this question correctly
  double GetFractionCorrect() const noexcept;
};

///Grades the results of students doing a test
struct HometrainerTestScorer
{
  ///The key must be the same key as used to create the test.
  ///The penalty is the number of questions that is
  ///expected to be answered correctly by guessing
  explicit HometrainerTestScorer(const int key, const int penalty = 0);

  ///Add the test results of a student, as saved by Hometrainer.
  ///The name of the student is the filename without path and extension.
  ///Throws std::runtime_error if the file cannot be read
  void Add(const std::string& filename);

  ///Add the encrypted test results of a student
  void Add(const std::string& name, const std::vector<std::string>& encrypted_lines);

  int GetKey() const noexcept { return m_key; }
  int GetPenalty() const noexcept { return m_penalty; }

  ///Obtain the scores per question, sorted by question
  std::vector<HometrainerQuestionScore> GetQuestionScores() const noexcept;

  ///Obtain the scores per student, in the order they are added
  const std::vector<HometrainerStudentScore>& GetStudentScores() const noexcept { return m_student_scores; }

  ///Convert the question scores to comma-seperated lines,
  ///including a header line
  std::vector<std::string> QuestionScoresToCsv() const noexcept;

  ///Convert the student scores to comma-seperated lines,
  ///including a header line
  std::vector<std::string> StudentScoresToCsv() const noexcept;

  private:
  int m_key;
  int m_penalty;
  std::vector<HometrainerQuestionScore> m_question_scores;
  std::vector<HometrainerStudentScore> m_student_scores;

  void ScoreQuestion(const std::string& question, const bool is_correct);
};

///Calculate a grade from 1.0 to 10.0, where the penalty is the number of
///questions that is expected to be answered correctly by guessing:
///
///  grade = 1 + 9 * (n_correct - penalty) / (n_questions - penalty)
///
///The grade is kept between 1.0 and 10.0
double CalculateGrade(const int n_correct, const int n_questions, const int penalty) noexcept;

///Format a number with one digit behind the decimal point
std::string FormatGrade(const double grade) noexcept;

void TestHometrainerTestScorer() noexcept;

} //~namespace ribi

#endif // HOMETRAINERTESTSCORER_H
