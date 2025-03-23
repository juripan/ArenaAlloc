#include <stdio.h>
#include <stdlib.h>

#include "jparena.h"

// prints an error message to stderr and kills the program
#define PANIC(msg) fprintf(stderr, msg "\n"); exit(1)


/*
 * allocates a Chunk and its memory,
 * only used internally, shouldn't be called by the user
*/
Chunk* chunk_alloc(JPArena* arena){
    Chunk* chunk = malloc(sizeof(Chunk));
    if(chunk == NULL){
        PANIC("Chunk allocation failed");
    }
    chunk->memory = malloc(arena->chunk_size);
    chunk->next = NULL;
    if(chunk->memory == NULL){
        PANIC("Chunk content allocation failed");
    }
    return chunk;
}

void* arena_alloc(JPArena* arena, size_t size_bytes){
    if(arena->chunk_size < size_bytes){
        PANIC("Chunk size too small for one item");
    }
    if(arena->start == NULL){
        Chunk* chunk = chunk_alloc(arena);
        chunk->bytes_reserved = size_bytes;
        arena->start = chunk;
        arena->end = chunk;
    } else if(arena->end->bytes_reserved + size_bytes > arena->chunk_size){
        Chunk* chunk = chunk_alloc(arena);
        chunk->bytes_reserved = size_bytes;
        arena->end->next = chunk; //moves the chunk into the last pos
        arena->end = arena->end->next; // updates the pointer to the new last chunk
    } else {
        arena->end->bytes_reserved += size_bytes;
    }
    return (char*)arena->end->memory + arena->end->bytes_reserved - size_bytes;
}

void print_arena(const JPArena* arena){
    Chunk* tmp = arena->start;
    if(tmp == NULL) puts("All chunks freed");
    while(tmp != NULL){
        printf("block start: %p, capacity: %ld bytes, used: %ld bytes\n", 
            tmp->memory, tmp->bytes_reserved, arena->chunk_size);
        printf("memory: ");
        unsigned char* mem = tmp->memory;
        for(size_t i  = 0; i < tmp->bytes_reserved; i++){
            printf("%02hhX ", mem[i]);
        }
        puts("");
        tmp = tmp->next;
    }
}

/*
 * frees a chunk and its memory
 * only used internally, shouldn't be called by the user
*/
void chunk_free(Chunk* ch){
    free(ch->memory);
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
