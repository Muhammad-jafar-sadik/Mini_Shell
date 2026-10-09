# Mini Shell in C

A simple Linux command-line shell implemented in C to learn Linux system calls and process management.

## Features
- Execute Linux commands using `fork()` and `execvp()`
- Change directories using `chdir()`
- Input redirection using `<`
- Output redirection using `>`
- Pipe commands using `|`
- Wait for child processes using `wait()`
- Basic error handling

## Technologies
- C programming
- Linux system calls
- Process management
- File descriptors and pipes

## Build
```bash
gcc lipractice.c -o myshell
```

## Run
```bash
./myshell
```

## Example Commands
```bash
ls
pwd
cd /tmp
ls > output.txt
cat < output.txt
ls | wc -l
```

## Learning Outcomes
This project helped me understand process creation, command execution, file descriptor redirection, pipes, and inter-process communication in Linux.

## Limitations
This is a learning project supporting basic commands, simple input/output redirection, and a basic two-command pipe. It does not implement every feature of Bash.
