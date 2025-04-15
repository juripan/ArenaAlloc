# JPArena

Simple arena allocator written in C.  
This arena is intended to be used  as global structs, no heap allocated struct shenanigans.

## Quick start
just include jparena.h into your c file and start coding
```C
#include "jparena.h"
```

## Extra features
By default any function in the header file that can fail will throw an error message and exit if an error occurs.  
You can change that by defining 'FAIL_SOFT' before including the file like this:
```C
#define FAIL_SOFT
#include "jparena.h"
```
By defining 'FAIL_SOFT' you will make it so all functions that can fail will instead return NULL so you can handle the error yourself.
