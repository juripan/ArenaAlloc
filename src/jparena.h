#ifndef JPARENA_H_
#define JPARENA_H_

#include <stdio.h>
#include <stdint.h>

#define ARENA(size_bytes) {.chunk_size = size_bytes, NULL, NULL}

// calls the arena alloc function with the size of the type supplied
#define ALLOC(a_ptr, t) arena_alloc(a_ptr, sizeof(t))

// prints an error message to stderr and kills the program
#define PANIC(msg) (fprintf(stderr, msg "\n"), exit(1))

typedef struct chunk_h Chunk;

typedef struct chunk_h {
    Chunk* next; // pointer to the next Chunk in the arena
    void* memory; // memory where the items are stored
    size_t bytes_reserved; // how much memory is used (in bytes)
} Chunk;

typedef struct jp_arena_h {
    size_t chunk_size; // maximum capacity of the Chunks (in bytes)
    Chunk *start; // pointer to the first Chunk
    Chunk *end; // pointer to the last Chunk
} JPArena;

/*
 * allocates memory of a desired size in a Chunk
 * if a chunk doesn't exist or all of them are too full then it creates a new one
 * if it can't allocate a Chunk or the size of the Chunk is too small it panics
 * returns the pointer to that part of memory
*/
void* arena_alloc(JPArena* arena, size_t size_bytes);

/*
 * prints the pointer, capacity and used capacity for every chunk in the arena
*/
void print_arena(const JPArena* arena);

/*
 * frees the whole arena chunk by chunk
 * sets its start and end pointers to NULL
*/
void arena_free(JPArena* arena);

/*
 * resets the used memory counter of every Chunk in the arena
 * doesn't free Chunks or their content
*/
void arena_reset(JPArena* const arena);

////////////////////
// IMPLEMENTATION ///////////////////////////////////////////////
////////////////////

/*
 * allocates a Chunk and its memory,
 * only used internally, shouldn't be called by the user
*/
Chunk* chunk_alloc__(JPArena* arena);

/*
 * frees a chunk and its memory
 * only used internally, shouldn't be called by the user
*/
void chunk_free__(Chunk* ch);

void* arena_alloc(JPArena* arena, size_t size_bytes){
    if(arena->chunk_size < size_bytes) PANIC("Chunk size too small for one item");
    
    if(arena->start == NULL){
        Chunk* chunk = chunk_alloc__(arena);
        chunk->bytes_reserved = size_bytes;
        arena->start = chunk;
        arena->end = chunk;
    } else if(arena->end->bytes_reserved + size_bytes > arena->chunk_size){
        Chunk* chunk = chunk_alloc__(arena);
        chunk->bytes_reserved = size_bytes;
        arena->end->next = chunk; // moves the chunk into the last pos
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
            tmp->memory, arena->chunk_size, tmp->bytes_reserved);
        printf("memory: ");
        unsigned char* mem = tmp->memory;
        for(size_t i  = 0; i < tmp->bytes_reserved; i++){
            printf("%02hhX ", mem[i]);
        }
        puts("");
        tmp = tmp->next;
    }
}

void arena_free(JPArena* const arena){
    Chunk* tmp = arena->start;
    while(tmp != NULL){
        Chunk* f = tmp;
        tmp = tmp->next;
        chunk_free__(f);
    }
    arena->start = NULL;
    arena->end = NULL;
}

void arena_reset(JPArena* const arena){
    Chunk* tmp = arena->start;
    while(tmp != NULL){
        tmp->bytes_reserved = 0;
        tmp = tmp->next;
    }
}

Chunk* chunk_alloc__(JPArena* arena){
    Chunk* chunk = malloc(sizeof(Chunk));
    if(chunk == NULL) PANIC("Chunk allocation failed");
    chunk->memory = malloc(arena->chunk_size);
    chunk->next = NULL;
    if(chunk->memory == NULL) PANIC("Chunk content allocation failed");
    return chunk;
}

void chunk_free__(Chunk* ch){
    free(ch->memory);
    free(ch);
}

#endif