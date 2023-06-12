# keikaku
A Scheme compiler.

This is work in progress project of a Scheme compiler. Right now only frontend part is considered to be quite mature and ready. You can try it by calling kei.cpp and passing --emis-ast option.
Although IR generation can be runned, it is not ready yet and probably buggy and error prone. You can still try to see the output. (With debug strings)
Codegen is definetly not ready, though I sketched out some things, for example registers allocation.
I am trying to resemble high-level architecture of LLVM, so that IR is generated from AST and machine code for (possibly) different architectures. 
The aim of this project is to learn compilers and Scheme is choosen mainly for simplicity of syntax.

## Installing and running

```
git clone https://github.com/houndlord/keikaku.git

$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
$ ./kei --h

# To build tests pass -DBUILD_TESTS = ON option to cmake:
$ cmake -DBUILD_TESTS=ON ..
$ cmake --build .
$ ctest
```
