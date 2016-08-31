//---------------------------------------------------------------------------
/*
MultipleChoiceQuestionDialog, dialog for MultipleChoiceQuestion
Copyright (C) 2011-2015 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppMultipleChoiceQuestionDialog.htm
//---------------------------------------------------------------------------
#ifndef MULTIPLECHOICEQUESTIONDIALOG_H
#define MULTIPLECHOICEQUESTIONDIALOG_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "questiondialog.h"
#pragma GCC diagnostic pop

namespace ribi {

struct MultipleChoiceQuestion;
struct Question;

struct MultipleChoiceQuestionDialog final : public QuestionDialog
{
  explicit MultipleChoiceQuestionDialog(const std::string& question);

  ///Will work if question is not nullptr
  explicit MultipleChoiceQuestionDialog(const boost::shared_ptr<const MultipleChoiceQuestion> question);

  ~MultipleChoiceQuestionDialog() noexcept;

  ///The answer the user has typed in so far
  int GetAnswerInProgress() const noexcept { return m_answer_in_progress; }

  boost::shared_ptr<const MultipleChoiceQuestion> GetMultipleChoiceQuestion() const noexcept { return m_question; }


  boost::shared_ptr<const Question> GetQuestion() const noexcept;

  ///Obtain the version
  static std::string GetVersion() noexcept;

  ///Obtain the version history
  static std::vector<std::string> GetVersionHistory() noexcept;

  ///The answer the user has selected
  ///Used for synching between multiple QtMultipleChoiceQuestionDialogs displaying
  ///the same MultipleChoiceQuestionDialog
  void SetAnswerInProgress(const int index) noexcept;

  ///Submit an answer
  ///For a multiple choice question, s will be the index of the answer
  void Submit(const std::string& s);

  std::string ToStr() const noexcept;

  mutable boost::signals2::signal<void (MultipleChoiceQuestionDialog*)> m_signal_mc_question_changed;

  private:
  int m_answer_in_progress;

  const boost::shared_ptr<const MultipleChoiceQuestion> m_question;

  ///Create a default Question
  //static MultipleChoiceQuestion * CreateDefaultQuestion();
  static boost::shared_ptr<MultipleChoiceQuestion> CreateDefaultQuestion() noexcept;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

} //~namespace ribi

#endif // MULTIPLECHOICEQUESTIONDIALOG_H