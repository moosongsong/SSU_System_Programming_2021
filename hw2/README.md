# Assignment #2

## ELF File

- ELF Magic number (4 bytes)
- 0x 7F 45 4C 46 (0x7F ELF )
- Tools for handling ELF files
- readelf
- objdump
- objcopy
- nm

## Goal
- Write a program that modifies an ELF file’s “rodata” section.
- First, you need to implement a function to check whether the file is
really an ELF file or not.
- Second, the program must modify the .rodata section as follows.
  1. Find a string “software”
  2. Modify the string to “hackers!”
- so that when the elf file execute, it uses the modified string

## Requirements

- You MUST provide only one ‘.c’ file and one ‘.h’ file
  - student-ID#.c à 20212021.c
  - student-ID#.h à 20212021.h
- You MUST provide simple and proper comments on your code
- Your program name MUST be your student ID#.
- The program MUST take a file path to an ELF file as an argument of the
main function so that it can execute like `./20212021 /home/my_elf_file`

## Editro
- You will be given source code of a program “editro” that prints basic
information of an ELF file and the exact location of the rodata section.
- Use this program to complete the assignments
- NOTE THAT
  - You MUST modify the Makefile to change the names of source code
  files and the binary output

![image](https://user-images.githubusercontent.com/50647845/152655755-2dc1ba28-5e2b-47f1-a5e0-d2d10253d170.png)

