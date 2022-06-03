# Unix-based Shell Application
## Overview
This is a program that allows for communication with the OS kernel through the use of command-line arguments.

## Motivation
This program was directly inspired by the Unix shell (Bash), and was created to demonstrate my knowledge of operating systems (on a fundamental level) to future/potential employers.

## How to use?

This program can be run using the following command structure:
```
./pipe cmd1 args1... + cmd2 args2...
```

It will only accept arguments that a Unix shell can also accept. For example:
```
./pipe ls -li -l + grep a
```

## License

This repository is available under the MIT license. See the LICENSE file for more info.
