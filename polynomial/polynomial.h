#ifndef POLYNOMIAL_H_INCLUDED
#define POLYNOMIAL_H_INCLUDED

#include <stddef.h>

typedef struct term term;

typedef struct polynomial polynomial;

struct term {
    int coeff;
    int exp;
};

struct polynomial {
    size_t n;
    struct term* terms;
};

polynomial init_polynomial(size_t num_terms, int const coeffs[num_terms], int const degs[num_terms]);

polynomial add(polynomial const p1, polynomial const p2);

polynomial negate(polynomial const p);

void display(const polynomial* const p);

void free_polynomial(polynomial* p);

long cont(const polynomial* p);

polynomial prim(const polynomial* p);

void prim_inplace(polynomial* p);

#endif