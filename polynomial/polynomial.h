#ifndef POLYNOMIAL_H_INCLUDED
#define POLYNOMIAL_H_INCLUDED

#include <stddef.h>

typedef struct term term;

typedef struct polynomial polynomial;

struct term {
    int exp;
    long coeff;
};

struct polynomial {
    size_t n;
    struct term* terms;
};

polynomial init_polynomial(size_t num_terms, int const coeffs[num_terms], int const degs[num_terms]);

polynomial add(polynomial const p1, polynomial const p2);

// /* Adds p2 to p1 and returns p1. */
// int add_inplace(polynomial* p1, polynomial const* p2);

polynomial negate(polynomial const p);

void display(const polynomial* const p);

void free_polynomial(polynomial* p);

long cont(const polynomial* p);

polynomial prim(const polynomial* p);

void prim_inplace(polynomial* p);

polynomial pquo(polynomial const* p, polynomial const* q);

polynomial prem(polynomial const* p, polynomial const* q);

polynomial quo(polynomial const* p, polynomial const* q);

polynomial rem(polynomial const* p, polynomial const* q);

polynomial prod(polynomial const* p, polynomial const* q);

long leval(polynomial const* p, long x);
int ieval(polynomial const* p, int x);
float feval(polynomial const* p, float x);

long lc(polynomial const* p);

int deg(polynomial const* p);

#endif