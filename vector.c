/* 
 * Machine Problem #2: Simple Shell 
 * CS 241, Spring 2011
 *
 * Implementation for the vector type.
 */

#include <stdlib.h>

#include "vector.h"

#define INIT_VEC_SIZE 1024


void vector_init(vector_t* v) {
    v->buffer = (void **) malloc(INIT_VEC_SIZE * sizeof(void *));
    v->size = 0;
    v->allocSize = INIT_VEC_SIZE;
}

void vector_destroy(vector_t* v) {
    free(v->buffer);
}

void vector_append(vector_t* v, void *item) {
    if(v->size == v->allocSize) {
        v->allocSize = v->allocSize*2;
        v->buffer = (void **) realloc(v->buffer, v->allocSize);
    }

    v->buffer[v->size] = item;
    v->size++;
}

void *vector_at(vector_t* v, unsigned int idx) {
    if(idx >= v->size) {
        return NULL;
    }

    return v->buffer[idx];
}

unsigned int vector_size(vector_t* v) {
    return v->size;
}
