#include "vector.h"

#include "assert.h"
#include "benchmark.h"


// NOTE: Don't add benchmarking here, because the benchmarking utility depends on these functions,
//       and we don't like stack overflow (the error; we do like the website)


bool vector_push(vector_t *vec, void *item) {
	assert(vec != NULL);
	// NOTE: Blocks vectors from containing NULL
	assert(item != NULL);

	// Check for overflow
	if (vec->amount == vec->size - 1) {
		fprintf(stderr, "Overflowing vector!");
		exit(1);
	}

	// Add item
	vec->items[vec->amount] = item;

	// Increase counter
	vec->amount++;

	return true;
}


void *vector_pop(vector_t *vec) {
	assert(vec != NULL);

	// Check for underflow
	if (vec->amount == 0) {
		return NULL;
	}

	// Get item
	void *item = vec->items[vec->amount - 1];

	// Decrease counter
	vec->amount--;

	return item;
}


void *vector_last(vector_t *vec) {
	assert(vec != NULL);

	// Check for underflow
	if (vec->amount == 0) {
		return NULL;
	}

	// Get item
	void *item = vec->items[vec->amount - 1];

	return item;
}


bool vector_copy(vector_t *dest, vector_t *src) {
	FUNC_START();

	bool success = true;

	VEC_EACH(*src, void *item) {
		success &= vector_push(dest, item);
	}

	FUNC_END();
	return success;
}


bool vector_remove(vector_t *vec, void *item) {
	FUNC_START();

	// Get index of item
	size_t i = 0;
	for (; i < vec->amount; i++) {
		if (vec->items[i] == item) {
			break;
		}
	}

	// Check if index is in bounds
	if (i == vec->amount) {
		fprintf(stderr, "Failed to remove item from vector, since it's not in the vector");
		FUNC_END();
		return false;
	}

	// Shift all items after the given item 1 to the left
	for (size_t j = i + 1; j < vec->amount + 1; j++) {
		vec->items[j - 1] = vec->items[j];
	}

	// Decrease amount
	vec->amount--;

	FUNC_END();
	return true;
}



void vector_init(vector_t *vec, size_t size) {
	vec->amount = 0;
	vec->size = size;
	vec->items = malloc(size * sizeof(void*));

	// Set all fields to zero (clear array)
	memset(vec->items, 0, size * sizeof(void*));
}


void vector_free(vector_t *vec) {
	assert(vec != NULL);

	free(vec->items);
}
