#include <stdio.h>
#include <stdlib.h>
#include "../../util/heap.h"
#include "../../polynomial/polynomial.h"

int main() {
    term* terms = calloc(10, sizeof(term));
    
    for (size_t i = 0; i < 10; i++) {
        terms[i].coeff = 1;
        terms[i].exp = i;
    }

    heap* h = build_min_heap(10, terms);
    term t = find_min(h);
    printf("The minimum term has exponent %d \n", t.exp);

    for (int i = 0; i < 10; i++) {
        t = extract_min(h);
        printf("Extracted term with exponent %d \n", t.exp);    
    }

    heap_insert(h, t);
    term t2 = {.coeff = 1, .exp = 5};
    heap_insert(h, t2);
    printf("Inserting monomial %ld x^%d into heap\n", t2.coeff, t2.exp);
    
    t = find_min(h);
    printf("Extracted term with exponent %d \n", t.exp);    

    heap_remove(h, 0);
    heap_remove(h, 0);

    for (size_t i = 6; i < 100; i++) {
        heap_insert(h, (term){.coeff = 1, .exp = i});
        if (i % 10 == 0) {
            printf("heap size is %ld, heap max is %ld\n", h->heap_size, h->heap_max);
        }
    }
    decrease_key(h, 94, 2);
    t = extract_min(h);
    printf("Extracted term with exponent %d \n", t.exp);

    free_heap(h);

    return 0;
}
