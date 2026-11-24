#include "hometrainerresources.h"

#include <cassert>
#include <stdexcept>

#include <QDirIterator>
#include <QFile>

#include "hometrainerhelper.h"
#include "question.h"

ribi::HometrainerResources::HometrainerResources()
{
  for (const QString& folder: { QString(":/resources"), QString(":/docs") })
  {
    QDirIterator i(folder);
    while (i.hasNext())
    {
      const QString resource_filename = i.next();
      const QString filename = i.fileName();
      if (QFile::exists(filename)) continue;
      QFile::copy(resource_filename, filename);
      if (!QFile::exists(filename))
      {
        throw std::runtime_error(
          "HometrainerResources: cannot create file '" + filename.toStdString() + "'"
        );
      }
      //A file copied from a resource is read-only
      QFile::setPermissions(filename, QFile::ReadOwner | QFile::WriteOwner | QFile::ReadGroup | QFile::ReadOther);
    }
  }
}

std::vector<std::string> ribi::HometrainerResources::GetExercises() const noexcept
{
  return {
    "ToolHometrainerExerciseClouds.txt",
    "ToolHometrainerExerciseMultiplication.txt",
    "ToolHometrainerExerciseTest.txt",
    "ToolHometrainerExerciseTtBasis.txt",
    "ToolHometrainerExerciseTtGeluid.txt",
    "ToolHometrainerExerciseTtLicht.txt",
    "ToolHometrainerExerciseWijkenArnhem.txt"
  };
}

void ribi::TestHometrainerResources() noexcept
{
  const HometrainerResources r;
  for (const auto& filename: r.GetExercises())
  {
    assert(IsRegularFile(filename));
    assert(!CreateQuestions(filename).empty());
  }
  assert(IsRegularFile(r.GetQuestion()));
  assert(IsRegularFile(r.GetQuestionMark()));
  assert(IsRegularFile(r.GetWelcome()));
}
