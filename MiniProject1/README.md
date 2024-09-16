# Shell in C

## Introduction

This project is a custom-built Unix-like shell implemented in C, designed to provide a powerful and efficient command-line interface for users to interact with the operating system. 
It supports a wide range of essential shell operations, including 
- executing external programs
- managing background and foreground processes
- handling I/O redirection
 -navigating the file system
 
The shell also incorporates advanced features like 
- signal handling
- process control
- the ability to manage multiple pipelines

Making it a versatile tool for both everyday use and complex system tasks.

## Running

### Build and run the shell:

```
make
./main
```

## Functionality

### `hop` Command

Command -> `hop <path/multiple path/no path>`

The `hop` command changes the directory that the shell is currently in and prints the full path of the working directory after changing. 

#### Special Paths
- Supports `.`, `..`, `~`, `-`

#### Path Types
- Supports reletive and absolute paths

#### Assumptions
- Paths and names are not to contain any whitespace characters.
- If no argument is present, `hop` should change to the home directory.
- If more than one argument is provided, `hop` execute sequentially with each argument, from left to right.
- `hop .`, `hop -`, and `hop ~` all change the previous working directory.


### `reveal` Command

Command -> `reveal <flags> <path>`

The `reveal` command lists all files and directories in the specified directories in lexicographic order. By default, it does not show hidden files. The command should support the following flags:

- **`-l`**: Displays extra information about the files.
- **`-a`**: Displays all files, including hidden files.

#### Special Paths
- Supports `.`, `..`, `~`, `-`

#### Path Types
- Supports reletive and absolute paths

#### Color Coding:
- **Green** for executables
- **White** for files
- **Blue** for directories

#### Assumptions
- If no argument is given, it should reveal the contents of the current working directory.
- Multiple paths are not given.
- Print a list of file/folder names separated by newline characters.

### `log` Command

Command -> `log`
Command -> `log purge`
Command -> `log execute <index>`

#### **`log`**:
  - Stores and outputs the last 15 commands entered by the user, including their arguments.
  - Do not store a command if it is exactly the same as the previously entered command.

#### **`log purge`**:
  - Clears all commands currently stored in the log.

#### **`log execute <index>`**:
  - Executes the command at the specified index in the log, with indices ordered from most recent to oldest.

#### Assumptions
- The `log` command itself or any command containing `log` will not be stored.
- Erroneous commands are be stored.
- Command after `log execute` will be stored in the log if it is not the same as the last command.
- Commands like `hop ..` and `hop    ..` will be considered different

### `proclore` Command

Command -> `proclore <processId>`

The `proclore` command is used to obtain information regarding a specific process. 

- **Process ID (pid)**
- **Process Status (R/R+/S/S+/Z)**
- **Process Group**
- **Virtual Memory Usage**
- **Executable Path of the Process**

#### Process States:
- **R/R+**: Running
- **S/S+**: Sleeping in an interruptible wait
- **Z**: Zombie

#### Assumptions 
- If no argument is provided, it prints information about the shell itself.

### `seek` Command

Command -> `seek <flags> <search> <target_directory>`

The `seek` command is used to search for a file or directory within a specified target directory. 

#### Flags:

- **`-d`**: Search only for directories, ignoring files even if the name matches.
- **`-f`**: Search only for files, ignoring directories even if the name matches.
- **`-e`**: This flag is effective only when a single file or directory with the specified name is found. If only one file is found, print its path. If only one directory is found, change the current working directory to it. This flag can be used in combination with `-d` and `-f`.

#### Assumptions:
- If no matching files or directories are found, print `No match found!`
- If both `-d` and `-f` flags are used together, print `Invalid flags!`
- If with `-e` the directory lacks execute permission or the file lacks read permission, output `Missing permissions for task!`.
- The command returns a list of relative paths (from the target directory) for all matching files and directories, with files displayed in green and directories in blue, each separated by a newline character.

### `.myshrc` File

#### Aliases

Format -> `alias <name>=<command>` or `<name>=<command>`

- These aliases are automatically loaded into the shell when it starts and can be used as shortcuts for frequently used commands.

##### Assumptions:
- `<name>` in aliases are single word command.
- IO redirection does not work for aliases.

#### Functions

Format -> 
```
  <functionName>()
  {
    <Whatever Function Does>
  }
```

- These functions are automatically loaded into the shell when it starts.
- Aliases can be used inside functions.

##### Assumptions:

- The curly brackets follow the order shown. Hence the format shown below does not work.
```
  <functionName>() {
    <Whatever Function Does>
  }
``` 
- IO redirection does not work for functions.

### IO Redirection

Format -> `<Command> < <inputFleName> > <outputFleName>` OR `<Command> < <inputFleName> >> <appendFileName>`

#### Types:

- `>`: Redirects output to the specified file. If the file exists, it is overwritten; otherwise, it is created with permissions `0644`.
- `>>`: Appends output to the specified file if it exists. If it does not exist, the file is created with permissions `0644`.
- `<`: Redirects input from the specified file. An error message, `No such input file found!`, is displayed if the input file does not exist.

#### Redirection Combinations:

- `>` and `<`: Can be used together to redirect both input and output.
- `>>` and `<`: Can also be used together for appending output while reading from a file.

#### Assumptions:

- Not handled multiple Inputs or Outputs.
- The name following `<`, `>` or `>>` is considered as the filename.
- Commands like `< <inputFleName> <Command>` also works as expected (as they work in shell).

### Pipes

Format -> `<Command1> | <Command2>`

- `|`: Used to pass the output from the command on the left to the input of the command on the right.
- Commands are executed sequentially from left to right when pipes are used.

#### Error Handling:

- Invalid use of pipe: An error message `Invalid use of pipe` is displayed if:
-   There is nothing on the left or right of a pipe (e.g., `| cmd` or `cmd |`).
-   There are multiple consecutive pipes (e.g., `cmd || cmd`).

#### Assumptions:
- IO Redirection works only with `First Command for Input` and `Last Command for Output`.
- BackGround works only for Last Command.

### `activities` Command

Format -> `activities`

Output -> `<pid> : <command name> - <State>`

- **Command Name**
- **Process ID (pid)**
- **State**: Whether the process is running or stopped

- This feature allows you to view a list of all the processes currently running that were spawned by your shell.

### Signal Handling

Format -> `ping <pid> <signal_number>`

- `<pid>`: The process ID to which the signal is to be sent.
- `<signal_number>`: The signal number to send.

- The `ping` command is used to send signals to processes based on their `pid`.

#### Keyboard Input Signal Handling

- `Ctrl - C`: Sends the SIGINT signal to interrupt any currently running foreground process. If no foreground process is running, it has no effect.
- `Ctrl - D`: Logs out of the shell after killing all background processes. This action does not affect the terminal.
- `Ctrl - Z`: Pushes the currently running foreground process (if any) to the background and changes its state to "Stopped". If no foreground process is running, it has no effect.

##### Assumptions:

- `Ctrl - D` does not work if `<Something typed><Ctrl - D>` is given.

### `fg` and `bg` Commands

Format -> `fg <pid>` and `bg <pid>`

- The `fg` command brings a specified background process (either running or stopped) to the foreground, giving it control of the terminal.
- The `bg` command changes the state of a stopped background process to "running" (in the background).

### `neonate` Command

Format -> `neonate -n <timeInterval>`

- The neonate command prints the process ID (PID) of the most recently created process on the system at regular intervals, specified by the user.
- The command runs until the user presses the 'x' key to stop it.
- The command retrieves the most recent process ID from the `/proc/loadavg` file

# Assumptions:

- `Ctrl - C`, `Ctrl - D` and `Ctrl - Z` doesnot work while `neonate` is running.

### `iMan` Command

Format -> `iMan <CommandName>`

- The `iMan` command fetches man pages from the internet using sockets and outputs the content to the terminal (default website : http://man.he.net/).

#### Assumptions:

- If there is more than one argument provided, only the first argument is considered, and the rest are ignored (e.g., `iMan sleep extra` is treated as `iMan sleep`).