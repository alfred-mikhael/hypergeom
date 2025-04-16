/** An implementation of a simple binary min-heap which will be used to accelerate polynomial multiplication. */
#ifndef _HEAP_H_INCLUDED_
#define _HEAP_H_INCLUDED_

#include <stddef.h>
#include <math.h>
#include <assert.h>
#include "../polynomial/polynomial.h"

typedef struct heap heap;

/** Binary min-heap data structure for acceleration of polynomial multiplication. Keys are the exponents of 
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
struct heap {
    size_t heap_size;
    size_t heap_max;
    term* arr;
};

int decrease_key(heap* h, size_t elem, int new_key);

int insert(heap* h, term t);

int remove(heap* h, size_t elem);

/* Don't want to return pointer here because the memory might be overwritten by heap operations. */
term extract_min(heap* h);

term find_min(heap const* h);

int heapify(heap* h, size_t root);

int up_heap(heap* h, size_t elem);

/* Builds a min-heap of the n elements in arr. Reorders arr in-place and continues to modify it afterwards. */
heap* build_min_heap(size_t n, term* arr);

inline size_t left(size_t i) {
    return 2 * i + 2;
}

inline size_t right(size_t i) {
    return 2 * i + 2
}

/* If i = 0, returns 0. i.e. parent of the root is the root. */
inline size_t parent(size_t i) {
    if (i == 0) return 0;
    return (i - 1) / 2;
}

#endif
