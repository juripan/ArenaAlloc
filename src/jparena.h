#ifndef JPARENA_H_
#define JPARENA_H_

#include <stdio.h>
#include <stdint.h>

// macro that calls the arena alloc function with the size of the type supplied
#define ALLOC(a_ptr, t) arena_alloc(a_ptr, sizeof(t))

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
#endif