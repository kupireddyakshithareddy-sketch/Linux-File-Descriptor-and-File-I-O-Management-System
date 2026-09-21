# Linux File Descriptor and File I/O Management System

## 1. Introduction

The Linux File Descriptor and File I/O Management System is a C-based
Linux project that demonstrates how files are managed using file
descriptors and system calls.

The project performs basic file operations such as creating, opening,
reading, writing, appending, and closing files.

## 2. Objectives

- To understand file descriptors in Linux.
- To understand Linux file input and output operations.
- To use system calls for file management.
- To perform read and write operations on files.
- To understand how Linux handles files.

## 3. Technologies Used

- Ubuntu Linux
- C Programming
- GCC Compiler
- Linux System Calls

## 4. System Calls Used

### open()

The open() system call is used to open or create a file.

### read()

The read() system call is used to read data from a file.

### write()

The write() system call is used to write data into a file.

### close()

The close() system call is used to close an opened file.

## 5. Features

- Create and open a file
- Write data into a file
- Read data from a file
- Append data to a file
- Display file descriptor
- Close a file
- Exit the application

## 6. Project Working

The program provides a menu-driven interface.

The user can select an operation from the menu. Based on the selected
option, the program performs the corresponding file operation using
Linux system calls.

## 7. Compilation

Use the following command to compile the program:

gcc file_manager.c -o file_manager

## 8. Execution

Run the program using:

./file_manager

## 9. Sample File

The program creates a file named:

data.txt

The file is used to store the data entered by the user.

## 10. Advantages

- Simple and easy to use.
- Demonstrates Linux file management.
- Helps understand file descriptors.
- Demonstrates basic Linux system calls.
- Useful for understanding File I/O concepts.

## 11. Conclusion

This project demonstrates the basic concepts of Linux file management
using C programming. It provides practical understanding of file
descriptors and system calls such as open(), read(), write(), and
close().
