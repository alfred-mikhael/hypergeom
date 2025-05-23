#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "./heap.h"

struct heap {
    size_t heap_size;
    size_t heap_max;
    term* arr;
};

int increase_key(heap* h, size_t elem, int new_key) {
    h->arr[elem].exp = new_key;
    return up_heap(h, elem);
}

int heap_insert(heap* h, term t) {
    if (h->heap_size < h->heap_max) {
        h->arr[h->heap_size] = t;
        up_heap(h, h->heap_size);
        h->heap_size += 1;
        return 0;
    } else {
        /* If at max memory, double the size of the list. This gives O(log n) amortized insertion. */
        term* tmp = reallocarray(h->arr, 2 * h->heap_max, sizeof(term)); 
        if (!tmp) {
            perror("Could not allocate more memory in exponent heap");
            exit(EXIT_FAILURE);
        }
        h->arr = tmp; 
        h->heap_max = 2 * h->heap_max;
        return heap_insert(h, t);
    }
}

int heap_remove(heap* h, size_t elem) {
    if (elem > h->heap_size) return 1;

    if (elem == h->heap_size) {
        h->heap_size -= 1;
        return 0;
    } 
    h->arr[elem] = h->arr[h->heap_size - 1];
    h->heap_size -= 1;
    heapify(h, elem);

    /* In order to avoid wasting memory, make sure that heap is always 1/4 full. */
    /* Except in the case when heap_size is very small, when it is not worth it. */
    if (h->heap_size > 20 && h->heap_size < (int) (0.25 * h->heap_max)) {
        h->heap_max = (int) (0.5 * h-> heap_max);
        term* tmp = realloc(h->arr, h->heap_max); 
        if (!tmp) {
            perror("Error in realloc in heap_remove");
            exit(EXIT_FAILURE);
        }
        h->arr = tmp;
    }
    return 0;
}

/* Don't want to return pointer here because the memory might be overwritten by heap operations. */
term extract_max(heap* h) {
    assert(h->heap_size > 0);
    term max = h->arr[0];

    h->arr[0] = h->arr[h->heap_size - 1];
    h->heap_size -= 1;

    /* In order to avoid wasting memory, make sure that heap is always 1/4 full */
    if (h->heap_size > 20 && h->heap_size < (int) (0.25 * h->heap_max)) {
        h->heap_max = (int) (0.5 * h-> heap_max);
        term* tmp = realloc(h->arr, h->heap_max);
        if (!tmp) {
            perror("Could not resize exponent heap after removal");
            exit(EXIT_FAILURE);
        }
        h->arr = tmp;
    }

    heapify(h, 0);
    return max;
}

term find_max(const heap* const h) {
    assert(h->heap_size > 0);
    return h->arr[0];
}

int heapify(heap* h, size_t root) {
    size_t l = left(root);
    size_t r = right(root);
    size_t largest = root;

    if (l < h->heap_size && h->arr[l].exp > h->arr[largest].exp) {
        largest = l;
    }
    if (r < h->heap_size && h->arr[r].exp > h->arr[largest].exp) {
        largest = r;
    }
    if (largest != root) {
        term t = h->arr[root];
        h->arr[root] = h->arr[largest];
        h->arr[largest] = t;
        return heapify(h, largest);
    }
    return 0;
}

int up_heap(heap* h, size_t elem) {
    if (elem == 0) return 0;

    term t = h->arr[elem];

    size_t curr = elem;
    size_t p = parent(curr);

    while (h->arr[p].exp < t.exp && curr > 0) {
        h->arr[curr] = h->arr[p];
        curr = p;
        p = parent(curr);
    }
    h->arr[curr] = t;
    return 0;
}

/* Builds a max-heap of the n elements in arr. Reorders arr in-place and continues to modify it afterwards. */
heap* build_max_heap(size_t n, term* arr) {
    heap* h = malloc(sizeof(heap));
    if (!h) perror("Couldn't allocate memory in build_min_heap");
    
    *h = (heap){.heap_max = n, .heap_size = n, .arr = arr};

    for (size_t i = (size_t)(n / 2); i > 0; i--) {
        heapify(h, i);
    }
    // avoids overflow error by decrementing size_t to 0
    heapify(h, 0);
    return h;
}

term* heap_sort(size_t n, term* arr) {
    heap* h = build_max_heap(n, arr);
    term* out = malloc(n * sizeof(term));
    size_t i = 0;
    term t;
    while(!is_empty(h)) {
        t = extract_max(h);
        out[i].coeff = t.coeff;
        out[i].exp = t.exp;
        i++;
    }
    free_heap(h);
    return out;
}

void free_heap(heap* h) {
    free(h->arr);
    h->arr = 0;
    free(h);
    h = 0;
}

inline size_t left(size_t i) {
    return 2 * i + 1;
}

inline size_t right(size_t i) {
    return 2 * i + 2;
}

/* If i = 0, returns 0. i.e. parent of the root is the root. */
inline size_t parent(size_t i) {
    if (i == 0) return 0;
    return (i - 1) / 2;
}

inline int is_empty(const heap* const h) {
    return (h->heap_size == 0);
}