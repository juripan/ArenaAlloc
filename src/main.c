#include <stdio.h>
#include <stdlib.h>

#include "jparena.h"

int main(){
    JPArena x = {0};
    arena_alloc(&x, 56);
    arena_alloc(&x, 8);
    arena_alloc(&x, 57);
    print_arena(&x);
    arena_free(&x);
    print_arena(&x);
    return 0;
}