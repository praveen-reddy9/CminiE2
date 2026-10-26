# C Preprocessor Mini Project

A college-level C project that demonstrates a small source-to-source preprocessor.

## Implemented
- Removes `//` and `/* ... */` comments.
- Preserves comment-looking text inside string and character literals.
- Stores and expands simple object-like `#define NAME value` macros.
- Writes a `.i` file from a `.c` input file.
- Leaves undefined identifiers unchanged.
- Preserves newlines from block comments.

Function-like macros and header-file inclusion are intentionally not implemented because they are outside this project's required scope.

## Build
```bash
make
```

## Run
```bash
./cpre tests/basic.c
```
This creates `tests/basic.i`.

## Clean
```bash
make clean
```
