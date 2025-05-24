/** An implementation of a simple binary max-heap which will be used to accelerate polynomial multiplication. */
#ifndef _HEAP_H_INCLUDED_
#define _HEAP_H_INCLUDED_

#include <stddef.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include "../polynomial/sum.h"

typedef struct heap heap;

/** Binary max-heap data structure for acceleration of polynomial multiplication. Keys are the exponents of 
 * monomial terms. Maintains its own dynamic array which will resize as necessary. Supports all the usual heap 
 * operations (perhaps amortized because of the dynamic array).
 * 
 * -------------- Members --------------------- 
 * size_t heap_size:  the current number of elements in the heap.
 * size_t heap_max:   the current amount of room in the heap. i.e. the current size of its dynamic array.
 * term* arr:         an array of monomial terms, satisfying the min-heap property; the exponent of the parent
 *                    is less than or equal to the exponent of all its descendants.
 * 
*/
struct heap;

int increase_key(heap* h, size_t elem, int new_key);

int heap_insert(heap* h, term t);

int heap_remove(heap* h, size_t elem);

/* Don't want to return pointer here because the memory might be overwritten by heap operations. */
term extract_max(heap* h);

term find_max(const heap* const h);

int heapify(heap* h, size_t root);

int up_heap(heap* h, size_t elem);

/* Builds a min-heap of the n elements in arr. Reorders arr in-place and continues to modify it afterwards. */
heap* build_max_heap(size_t n, term* arr);

term* heap_sort(size_t n, term* arr);

void free_heap(heap* h);

inline int is_empty(const heap* const h);

#endif
