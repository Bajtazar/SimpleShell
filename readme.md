# Simple Shell

## Description

This shell has been writted as a semester project from the `UXP1A`. This program features a simple shell that:

* Can execute build-in commands
* Can execute /usr/bin/ programs
* Can execute local programs when the dot is placed before its name: `.name`
* Can create and use the local and environmental variables
* Can execute groups of commands in a subshell `( cd .. ; ls -l )`
* Can use pipses `ls -l | grep 'x' | wc -l`
* can redirect streams using the `>file`, `file<`, `stream>file`,  `stream>stream`, `stream>&stream` syntax (and its variations)

## Building

To build the console simply use cmake

```bash
cmake -S . -B build
cd build
make
```
