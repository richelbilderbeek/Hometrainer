# Command line

Everything Hometrainer can do, can also be done from the command line,
using `ToolHometrainerConsole`:

```text
ToolHometrainerConsole --help
```

Images are shown as ASCII art.

## Commands

Command                   | Description
--------------------------|----------------------------------------------------
`--about`                 | Show information about Hometrainer
`--help`                  | Show the help
`--version`               | Show the version
`--example`               | Create example exercises in the current folder
`--file`                  | Do an exercise
`--create-test`           | Create an encrypted test from an exercise
`--take-test`             | Do a test, the results are saved encrypted
`--score-tests`           | Grade the results of one or more tests
`--create-printable-test` | Create a test on paper from an exercise

## Options

Option                 | Description
-----------------------|---------------------------------------------------------
`--key [number]`       | The key used to encrypt a test
`--questions [number]` | The maximum number of questions in a test
`--output [filename]`  | The file to save to
`--penalty [number]`   | Number of questions expected to be correct by guessing
`--name [full name]`   | The full name of the student doing a test
`--code [letters]`     | Answer code for multiple choice questions on paper
`--format [html/text]` | The format of a test on paper, `html` by default

## Examples

Do an exercise:

```text
ToolHometrainerConsole my_exercise.txt
```

Create a test with 10 questions:

```text
ToolHometrainerConsole --create-test my_exercise.txt --key 1234 --questions 10 --output my_test.txt
```

Do a test:

```text
ToolHometrainerConsole --take-test my_test.txt --key 1234 --name "John Doe"
```

Score tests:

```text
ToolHometrainerConsole --score-tests "John Doe.txt" "Jane Doe.txt" --key 1234 --output scores.csv
```

Create a test on paper:

```text
ToolHometrainerConsole --create-printable-test my_exercise.txt --code HOMETRAINER --output my_test.html
```
