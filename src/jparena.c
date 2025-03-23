#include <stdio.h>
#include <stdlib.h>

#include "jparena.h"

#define PANIC(msg) fprintf(stderr, msg); exit(1)

//TODO: make chunks into bigger slices of memory so theres no need to malloc multiple small chunks
void* arena_alloc(JPArena* arena, size_t size_bytes){
    Chunk* chunk = malloc(sizeof(Chunk));
    if(chunk == NULL){
        PANIC("Chunk allocation failed");
    }
    chunk->item = malloc(size_bytes);
    chunk->next = NULL;
    if(chunk->item == NULL){
        PANIC("Chunk content allocation failed");
    }

    if(arena->start == NULL){
        arena->start = chunk;
        arena->end = chunk;
    } else {
        arena->end->next = chunk; //moves the chunk into the last pos
        arena->end = arena->end->next; // updates the pointer to the new last chunk
    }
    return chunk->item;
}

void print_arena(const JPArena* arena){
    Chunk* tmp = arena->start;
    if(tmp == NULL) puts("---Empty---");
    while(tmp != NULL){
        printf("pointer: %p\n", tmp->item);
        tmp = tmp->next;
    }
}

void chunk_free(Chunk* ch){
    free(ch->item);
    free(ch);
}

void arena_free(JPArena* const arena){
    Chunk* tmp = arena->start;
    while(tmp != NULL){
        Chunk* f = tmp;
        tmp = tmp->next;
        chunk_free(f);
    }
    arena->start = NULL;
    arena->end = NULL;
}
