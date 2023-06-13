# keikaku
Keikaku: A Compiler for Scheme

Keikaku is a work-in-progress compiler for the Scheme programming language. The project's main objective is to learn and understand compiler design. Scheme is primarily chosen for its simplicity in syntax.

Currently, the frontend of the compiler is considerably mature and ready for use. It can be experimented with using the 'kei.cpp' and the '--emit-ast' option to observe the abstract syntax tree (AST).

The intermediate representation (IR) generation is available but in an early development stage, possibly containing bugs and errors. You can still try this feature and observe the output, complete with debug strings.

The code generation module is not yet ready, although some preliminary work, like register allocation, has been sketched out.

The high-level architecture of the project is inspired by the LLVM compiler infrastructure, transforming the AST into an IR, and ultimately machine code that can target various architectures.

## Installing and usage
Clone the repository:
```
git clone https://github.com/houndlord/keikaku.git
```
Build the project:
```
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
```
Run kei:
```
$ ./kei --h
```
```
# To build tests pass -DBUILD_TESTS = ON option to cmake:
$ cmake -DBUILD_TESTS=ON ..
$ cmake --build .
$ ctest
```
