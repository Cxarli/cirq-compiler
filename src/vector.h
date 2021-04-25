#ifndef VECTOR_H
#define VECTOR_H


#include <stdlib.h>
#include <stdbool.h>

#include "defines.h"


typedef struct vector {
	size_t amount;
	size_t size;
	void** items;
} vector_t;


// Loop over all items in the vector
#define VEC_EACH(vec, var) \
	for ( \
		size_t CONCAT(__i, __LINE__) = 0; \
		CONCAT(__i, __LINE__) < (vec).amount; \
		CONCAT(__i, __LINE__)++ \
	) \
		with(var = (vec).items[CONCAT(__i, __LINE__)])


// Loop over all items, but with given index variable
#define VEC_EACH_INDEX(vec, var, ix_var) \
	for (size_t ix_var = 0; ix_var < (vec).amount; ix_var++ ) \
		with(var = (vec).items[ix_var])


bool vector_push(vector_t *vec, void *item);
void *vector_pop(vector_t *vec);
void *vector_last(vector_t *vec);
bool vector_copy(vector_t *dest, vector_t *src);
bool vector_remove(vector_t *vec, void *item);


void vector_init(vector_t *vec, size_t size);
void vector_free(vector_t *vec);


#endif
