# GolfC

Ever wanted to minify your C code? Now you can. This repository provides a code-minifier for
programs written in C. Simply build the repository and run the executable `minifier` on
your source C file, and the program will produce a minified C file (`out.c` by default).
This can be adjusted with the `-o` flag to specify the name of the produced C file.

## Motivation

Why minify C code? Unlike Javascrpt and HTML where the source code must be sent over the web,
making code size actually important, source code size has no effect on C programs. After all,
everything gets compiled down into the same bytecode. However, some UC professors have been
known to offer EC assignments that ask their students to "golf" their code (use the minimum
number of bytes possible to implement their program). While this repository won't optimize
your code by changing its structure, it will at least minify any existing code by removing
any spaces and replacing repeated patterns with defines (when it's worth it).

## Design

The design is quite simple. First, we make a pass over the input and convert all variables
into minimum-size variables.

Then, we do a pass over this output to look for patterns of repeated tokens. If a pattern satisfies
the equation `N*L < 9+X+L`, where N is the number of appearances, L is the length of the pattern,
in bytes, and X is the length of the next available identifier, then a define is added to the file
and all occurrences of the pattern are replaced with the identifier assigned to the pattern.

Finally, we do a pass over the tokens to remove spaces where applicable.
