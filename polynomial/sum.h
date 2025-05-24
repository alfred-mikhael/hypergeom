#ifndef SUM_H_INCLUDED
#define SUM_H_INCLUDED

#include <stddef.h>

typedef struct term term;

typedef struct sum sum;

struct term {
    int exp;
    long coeff;
};

struct sum {
    size_t n;
    struct term* terms;
};

sum init_polynomial(size_t num_terms, const int* const exps, const int* const degs);

sum zero_polynomial();

sum add(const sum* const p1, const sum* const p2);

sum scalar_prod(long s, const sum* const p);

void scalar_prod_in_place(long s, sum* const p);

// /* Adds p2 to p1 and returns p1. */
// int add_inplace(polynomial* p1, polynomial* const p2);

sum negate(const sum* const p);

void negate_in_place(sum* const p);

void display(const sum*const p);

void free_polynomial(sum* p);

long cont(const sum* const p);

sum prim(const sum* const p);

void prim_in_place(sum* const p);

sum pquo(const sum* const p, const sum* const q);

sum prem(const sum* const p, const sum* const q);

sum quo(const sum* const p, const sum* const q);

sum rem(const sum* const p, const sum* const q);

sum prod(const sum* const p, const sum* const q);

sum prim_gcd(const sum* const p, const sum* const q);

long leval(sum* const p, long x);
int ieval(sum* const p, int x);
float feval(sum* const p, float x);

long lc(const sum* const p);

int deg(const sum* const p);

#endif