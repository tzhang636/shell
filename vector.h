/* 
 * Machine Problem #2: Simple Shell 
 * CS 241, Spring 2011
 *
 * Interface for the vector type.
 */

#ifndef __VECTOR_H_
#define __VECTOR_H_

typedef struct _vector{
	void** buffer;
	unsigned int size;
	unsigned int allocSize;
} vector_t;

void vector_init(vector_t *v);
void vector_destroy(vector_t *v);
void vector_append(vector_t *v, void *item);
void *vector_at(vector_t *v, unsigned int idx);
unsigned int vector_size(vector_t* v);

#endif
