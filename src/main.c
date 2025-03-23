#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "jparena.h"

int main(){
    JPArena arena = {80, 0, 0};
    
    char* p = arena_alloc(&arena, sizeof(char));
    *p = 'a';
    int32_t* num = arena_alloc(&arena, sizeof(int32_t));
    *num = 456565;
    char* c = arena_alloc(&arena, sizeof(char));
    *c = 'D';
    
    print_arena(&arena);
    printf("%c, %d, %c\n", *p, *num, *c);
    
    arena_reset(&arena);
    char* c2 = arena_alloc(&arena, sizeof(char));
    *c2 = 'F';
    printf("%c, %d, %c\n", *p, *num, *c);
    
    arena_free(&arena);
    return 0;
}