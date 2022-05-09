BryanScript
===========

Welcome! BryanScript is an experiment; I've been a hobbyist game developer for a while and BryanScript is my path the learn a bunch of stuff I've wanted to learn for a long time 😃

I've wanted to learn about language, os, and game engine development. BryanScript is my first step on that journey.

The idea is simple: take things I like/love from other languages (the dynamism/flexibility of JavaScript, the whitespace sensitivity of CoffeeScript/Python, etc) and see if they can be merged into one thing.

I also want to build a comprehensive toolchain; a test runner, simple ways to instrument code, and lots ... and I mean *lots* ... of debugging support.

Starting point
==============

Right now this repo is where I'm at working through the excellent book [Crafting Interpreters](http://craftinginterpreters.com). This is the bytecode vm for that book's language, Lox.

Once I finish the book I'd like to update this vm in the following ways:

1. Add tests for everything (I imagine this will encompass a significant amount of refactoring)
2. Add support for `switch` statements (using hash table, unless I find a better option in the mean time)
3. Add dependency support (I'm super excited about this as I want to model it after the dependency specs from my little Node.js module, [Mädūl](https://github.com/bsgbryan/madul).
4. Add `async`/`await` support
5. Add prefix and postfix incrementors/decrementors(`++`/`--`)

I want to add multithreading support (both to the language and to the vm for compilation/execution) but, frankly, that terrifies me; so I'm going to hold off thinking about it too much for now 😅

Next steps
==========

In addition to the bytecode vm, I'd like to implement native support for:

1. WebAssembly
2. ARM64 assembly
3. x86_64 assembly

Build & run
===========

prerequisites
-------------

All you need is a machine with gcc.

steps
-----

1. clone the repo
2. In the cloned repo's root directory, enter `gcc memory.c value.c chunk.c debug.c vm.c scanner.c compiler.c object.c table.c main.c -o clox -Wall` (this compiles all the source files and generates the `clox` executable binary file)
3. Enter `./clox ./scripts/globals.lox` (or any of the files in the `scripts` directory)

Feel free to create and execute your own files too!
