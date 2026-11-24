include(ToolHometrainer.pri)
include(ToolHometrainerConsole.pri)

TARGET = ToolHometrainerConsole
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

SOURCES += main.cpp

# Qt
# 'gui' is needed for QImage, used to display images as ASCII art
QT += core gui
