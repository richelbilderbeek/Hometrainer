# Build

Hometrainer is written in C++ and uses Qt.
The Windows version is built automatically and can be
[downloaded from the start page](README.md#run-or-download).

To build Hometrainer yourself, install Qt 6 and a C++17 compiler, then:

```bash
git clone https://github.com/richelbilderbeek/Hometrainer
cd Hometrainer
qmake ToolHometrainerDesktop.pro
make
```

Project                       | Description
------------------------------|------------------------------------
`ToolHometrainerDesktop.pro`  | The desktop version
`ToolHometrainerConsole.pro`  | The command line version
`ToolHometrainerTest.pro`     | The tests

The tests are run like this:

```bash
qmake ToolHometrainerTest.pro
make
./ToolHometrainerTest
```
