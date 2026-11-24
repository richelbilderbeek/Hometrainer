# Settings shared by all Hometrainer projects

CONFIG += c++17

# High warning levels
# Qt does not go well with -Weffc++
!msvc {
  QMAKE_CXXFLAGS += -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Werror

  # The test functions only use some variables in an assert,
  # which are unused when NDEBUG is defined.
  # Also prevents Qt from failing with this error:
  # qrc_[*].cpp:400:44: error: 'qInitResources_[*]__init_variable__' defined but not used
  QMAKE_CXXFLAGS += -Wno-unused-variable -Wno-unused-but-set-variable
}
msvc {
  QMAKE_CXXFLAGS += /W3 /utf-8
}

# In release mode, define NDEBUG
CONFIG(release, debug|release) {
  DEFINES += NDEBUG
}

# In debug mode, turn on gcov and UBSAN
CONFIG(debug, debug|release) {
  !msvc {
    # gcov
    QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
    LIBS += -lgcov

    # UBSAN
    QMAKE_CXXFLAGS += -fsanitize=undefined
    QMAKE_LFLAGS += -fsanitize=undefined
    LIBS += -lubsan
  }
}
