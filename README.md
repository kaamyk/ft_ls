# ft_ls
> An home-made version of the famous *ls* unix command.

## Summary:
- [Goals](#goals)
	- [General](#general)
	- [In details](#in-details)
- [Constraints](#constraints)
	- [Allowed functions](#allowed-functions)
- [Theory](#theory)
- [In practice](#in-practice)

## Goals:
### General:
- **Recreate** the *ls* Unix command in C.
- Learn **new function** from operating system's API associated with data structures.
- Think about the code structure **before starting** due to certains flags (-R: recursive).

### In details:
Must recreate the behaviour of the *ls* command with and without the following options: -l, -R, -a, -r, -t.

The project must be written in **C**.

The overall display must be **as indentical** as possible to the original ls.

It is **not required** to manage the **columns display** when the *-l* option is not in the arguments.

No error or unexpected behaviour is to be observed (i.e no memory leaks, pendant fds, segfaults, etc.). If unsure handle as *ls* would.

## Constraints:

### Allowed functions:
The allowed functions are the followings:

## Theory:

### Syntax:
The *ls* command can be written with the following different forms:
- ls
- ls -lRa
- ls -l -R -a
- ls dirname
- ls -l dirname
- ls -lRa dirname
- ls -l dirname -R
- ls -R dir1 dir2 -a

With all these possibilities we will need a **parser**. It will read and store the needed informations for the program to run.

### Modularity:
With the information given by the parser, for performance reasons, the code needs to adapt and not run code or system calls if not needed.<br>
We will then take special care to **separate** the code (in particular options) into clearly **defined modules**.

### Options:
Here is a detailed list of the options required:
- **-l**: use a long listing format. One line for each item with informations about rights, type, owner, group,  size, last modified and the filename.
- **-R**: list subdirectories recursively.
- **-a**: do not ignore entries starting with '.'.
- **-r**: reverser order while sorting.
- **-t**: srt by time, newest first.

## In practice: