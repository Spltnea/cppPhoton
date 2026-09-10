# The Photon Programming Language

Photon is a programming language mainly aimed towards low level to freestanding usages <br>
It is the result of my dreams of ever making a programming language

## Why am i doing this project

I was always fascinated on how a computer worked, i started out writting stuff in Java (mostly to mod Minecraft which ended badly lmao) <br>
Programming languages were magical to me until i heard about C and C++ vaguely a while ago <br>

I got into low level programming really not that long ago (May of this year until i had to stop from chronical stress) and i started understanding vaguely how a programming language worked, and to some extent, how a computer really works <br>

This project will help me deeply understand the process, the quirks and shenanigans that is low level developping and i hope i'll take good lessons from it, i know it is weird to learn this way by making something complicated, but i'm all up for it !

This project went through multiple versions before sticking with this C++ version, i had done it in Python, C#, Rust and C, none of those convinced me (this has been going for one year now)

## Compiling your own version of the language
To compile your own version of the language, you must be on a linux distribution or a macOS System (use a WSL Subsystem on Windows)

### 1 : Install the toolchain
Run the following command at the root directory using administrator privileges <br>

```bash
# Ubuntu | Debian Based Systems
sudo apt update
sudo apt install -y llvm-dev clang cmake build-essential

# Fedora Based Systems
sudo dnf install -y llvm-devel llvm-static clang lld cmake gcc-c++

# Arch Linux Systems
sudo pacman -S llvm clang cmake base-devel

# macOS (Homebrew)
brew install llvm cmake
# Note: Homebrew does not symlink LLVM into PATH by default, so tell CMake where it is:
export LLVM_DIR=$(brew --prefix llvm)/lib/cmake/llvm
```

### 2 : Check the proper installation
Check the proper installation using the following commands : <br>

1. `llvm-config --version` to check the LLVM Version Installed
2. `clang --version` to check the Clang Version
3. `lld --version` to check LLD Version
4. `cmake --version` to check CMake Version

### 3 : Compile the project
Building the project involves a two step process i will later automate using scripts

```bash
# At the parent directory (cppPhoton/), execute
cmake -B build

# Build the compiler
cmake --build build

# Test
./build/cppPhoton <path/fileName.pho>
```

## Roadmap
The project will follow this roadmap, changes are subject to be made at any time

<details open>
<summary><b> A : Making a turing complete language </b></summary>

| Task                        | Status                               |
| ------------------------    | ------------------------------------ |
| Toolchain Configuration     | **DONE**                             |
| Preprocessor Implementation | **DONE** - *Comments Stripping Only* |
| Lexer Implementation        | **WORK IN PROGRESS**                 |
| Parser Implementation       | **TODO**                             |
| Semantic Analysis           | **TODO**                             |
| LLVM Generation             | **TODO**                             |

I will add more entries here later

## Releases and Updates
This section will be updated when the compiler will see its first iteration <br>
Look for the `changelog/devlogs` directory to see what am i working on and how i tackle things <br>

## Documentation
Pre-Release Documentation will be contained in the `documentation` folder <br>
It will be divided into multiple components : <br>
1. `documentation/modules` This is where every module of the compiler will be explained in detail (code snippets, diagrams, and such)
2. `documentation/concepts` Here i will share syntax concepts, mechanics, and so on, i might also place abstract algorithms on here, think of this as my ideas box i want to share
3. `documentation/manual` I will write here the manual of this language

## AI Involvement
I should mention that AI has been slightly used for the following things : <br>
1. Compacting documentation to narrow down informations i need
2. Small code suggestions and optimizations (whether i refine and implement them or not)
3. Configuration quirks fixing when i cannot find those

The AI is not aware of this project and is instead giving me abstract examples i rewrite, refactor, and use for this code, otherwise, about 95% of the code is human written, i always resort to trial and error, debugging, hand writting concepts, my personal knowledge (albeit limited to basic concepts), and online documentation from the following sites before resorting to AI (and you should do that too): <br>

1. https://www.cppreference.com/
2. https://stackoverflow.com/questions
3. https://www.w3schools.com/cpp/
4. https://www.geeksforgeeks.org/cpp/c-plus-plus/

I do not condone vibe coding (copy pasting code threw by an AI and calling it a day), use AI only to narrow down informations you really need instead of looking for hours on end to pages of documentation (even tho i still recommend doing this as you can learn a lot of stuff along the way) <br>

Practicing, trial and error, looking at already made projects and having documentation aside is the best way (for me) to learn something specific about coding and really everything

## Endnote
As much as i'm having fun doing this project as a begginer (or not), i hope it will illuminate some heads and inspire them to do the same thing, talking to a processor is really fun <br>
Thank you for following me on this journey and i shall thank myself for making this dream possible now that i have everything i need to do so :) <br>

- Splittine
