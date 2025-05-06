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

polynomial add(const polynomial* const p1, const polynomial* const p2);

// /* Adds p2 to p1 and returns p1. */
// int add_inplace(polynomial* p1, polynomial* const p2);

polynomial negate(const polynomial* const p);

void negate_in_place(polynomial* const p);

void display(const polynomial*const p);

void free_polynomial(polynomial* p);

long cont(const polynomial* const p);

polynomial prim(const polynomial* const p);

void prim_inplace(polynomial* const p);

polynomial pquo(const polynomial* const p, const polynomial* const q);

polynomial prem(const polynomial* const p, const polynomial* const q);

polynomial quo(const polynomial* const p, const polynomial* const q);

polynomial rem(const polynomial* const p, const polynomial* const q);

polynomial prod(const polynomial* const p, const polynomial* const q);

long leval(polynomial* const p, long x);
int ieval(polynomial* const p, int x);
float feval(polynomial* const p, float x);

long lc(const polynomial* const p);

int deg(const polynomial* const p);

#endif