#ifndef JPARENA_H_
#define JPARENA_H_

#include<stdio.h>
#include <stdint.h>

typedef struct chunk_h Chunk;

typedef struct chunk_h {
    Chunk* next;
    void* memory;
    size_t bytes_reserved;
} Chunk;

typedef struct jp_arena_h {
    size_t chunk_size; // in bytes
    Chunk *start;
    Chunk *end;
} JPArena;

void* arena_alloc(JPArena* arena, size_t size_bytes);

void print_arena(const JPArena* arena);

void arena_free(JPArena* arena);
#endif