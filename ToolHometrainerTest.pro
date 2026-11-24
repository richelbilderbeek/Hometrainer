# Runs all tests, of both the console and desktop classes
include(ToolHometrainer.pri)
include(ToolHometrainerDesktop.pri)

TARGET = ToolHometrainerTest
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

SOURCES += testmain.cpp

# The tests use assert, so NDEBUG must never be defined
DEFINES -= NDEBUG

# Qt
QT += core gui widgets
