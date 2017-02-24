# C++14
CONFIG += c++14
QMAKE_CXX = g++-5
QMAKE_LINK = g++-5
QMAKE_CC = gcc-5
QMAKE_CXXFLAGS += -Wall -Wextra -std=c++14


# Debug and release mode

CONFIG += debug_and_release
CONFIG(debug, debug|release) {
  message(Debug mode)
}

CONFIG(release, debug|release) {
  message(Release mode)
  DEFINES += NDEBUG
}

# Qt
QT += core gui

# Wt
LIBS += -lwt -lwthttp

# Prevents this error:
#/usr/include/boost/math/constants/constants.hpp:277: error: unable to find numeric literal operator 'operator""Q'
#   BOOST_DEFINE_MATH_CONSTANT(half, 5.000000000000000000000000000000000000e-01, "5.00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e-01")
#   ^
QMAKE_CXXFLAGS += -fext-numeric-literals

include(../RibiLibraries/BoostAll.pri)

include(../RibiClasses/CppAbout/CppAbout.pri)
include(../RibiClasses/CppFileIo/CppFileIo.pri)
include(../RibiClasses/CppHelp/CppHelp.pri)
include(../RibiClasses/CppMenuDialog/CppMenuDialog.pri)

include(../RibiLibraries/GeneralWeb.pri)

include(../RibiClasses/CppCanvas/CppCanvas.pri)
include(../RibiClasses/CppContainer/CppContainer.pri)
include(../RibiClasses/CppFuzzy_equal_to/CppFuzzy_equal_to.pri)
include(../RibiClasses/CppImageCanvas/CppImageCanvas.pri)
include(../RibiClasses/CppTribool/CppTribool.pri)

include(CppWtExercise/CppWtExercise.pri)
include(CppWtMultipleChoiceQuestionDialog/CppWtMultipleChoiceQuestionDialog.pri)
include(CppWtOpenQuestionDialog/CppWtOpenQuestionDialog.pri)
include(CppWtQuestionDialog/CppWtQuestionDialog.pri)
include(../RibiClasses/CppWtSelectFileDialog/CppWtSelectFileDialog.pri)

include(ToolHometrainerWebsite.pri)

SOURCES += wtmain.cpp
