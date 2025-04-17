#include "./polynomial.h"
#include "../numeric/euclid.h"

#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "assert.h"

polynomial init_polynomial(size_t num_terms, int const coeffs[num_terms], int const degs[num_terms]) {
    polynomial p = {
        .n = num_terms,
        .terms = calloc(num_terms, sizeof(term))
    };
    if (!p.terms) perror("Could not allocate memory: ");

    for (int i = 0; i < num_terms; i++) {
        p.terms[i].coeff = coeffs[i];
        p.terms[i].exp = degs[i];
    }
    return p;
}

/* Assumes that the terms are sorted. */
polynomial add(polynomial const p1, polynomial const p2) {
    size_t max_terms = p1.n + p2.n;
    term* terms = calloc(max_terms, sizeof(term));

    int i = 0, j = 0, k = 0;
    while (i < p1.n && j < p2.n) {
        if (p1.terms[i].exp == p2.terms[j].exp) {
            if (p1.terms[i].coeff + p2.terms[j].coeff > 0) {
                terms[k].coeff = (p1.terms[i].coeff + p2.terms[j].coeff);
                terms[k].exp = p1.terms[i].exp;
                i++; j++; k++;
            }
        }
        else if (p1.terms[i].exp > p2.terms[j].exp) {
            terms[k].coeff = p2.terms[j].coeff;
            terms[k].exp = p2.terms[j].exp;
            j++; k++;
        }
        else {
            terms[k].coeff = p1.terms[i].coeff;
            terms[k].exp = p1.terms[i].exp;
            i++; k++;
        }
    }

    while (i < p1.n) {
        terms[k].coeff = p1.terms[i].coeff;
        terms[k].exp = p1.terms[i].exp;
        i++; k++;
    }

    while (j < p2.n) {
        terms[k].coeff = p2.terms[j].coeff;
        terms[k].exp = p2.terms[j].exp;
        j++; k++;
    }

    polynomial p = {
        .n = k,
        .terms = realloc(terms, k * sizeof(term))
    };
    return p;
}

polynomial negate(polynomial const p) {
    term* terms = calloc(p.n, sizeof(term));
    for (size_t i = 0; i < p.n; i++) {
        terms[i].coeff = -p.terms[i].coeff;
        terms[i].exp = p.terms[i].exp;
    }
    polynomial g = {
        .n = p.n,
        .terms = terms
    };

    return g;
}

void display(const polynomial* const p) {
    for (size_t i = 0; i < p->n; i++) {
        printf("%ld", p->terms[i].coeff);
        if (i > 0) {
            printf("x^%d", p->terms[i].exp);
        }
        if (i < p->n - 1) {
            printf(" + ");
        }
    }
    printf("\n");
}

void free_polynomial(polynomial* p) {
    if (!p) {
        return;
    }
    p->n = 0;
    free(p->terms);
    p->terms = 0;
}

/* Computes the content of a polynomial p */
long cont(const polynomial* p) {
    if (p->n == 1) {
        return p->terms[0].coeff;
    }
    long curr_gcd = p->terms[0].coeff;
    for (int i = 1; i < p->n; i++) {
        curr_gcd = gcd(curr_gcd, p->terms[i].coeff);
    }
    return curr_gcd;
}

polynomial prim(const polynomial* p) {
    long c = cont(p);
    term* terms = calloc(p->n, sizeof(term));

    for (int i = 0; i < p->n; i++) {
        terms[i].coeff = p->terms[i].coeff / c;
        terms[i].exp = p->terms[i].exp;
    }
    polynomial g = {
        .n = p->n,
        .terms = terms
    };
    return g;
}

void prim_inplace(polynomial* p) {
    long c = cont(p);
    for (int i = 0; i < p->n; i++) {
        p->terms[i].coeff /= c;
    }
}

polynomial pquo(polynomial const* p, polynomial const* q) {
    
}

polynomial prem(polynomial const* p, polynomial const* q) {

}

/* Requires that deg(p) >= deg(q). */
polynomial quo(polynomial const* p, polynomial const* q) {
    assert(deg(p) >= deg(q));
    term* terms = calloc(deg(p) / deg(q), sizeof(term));
    int n = deg(p) / deg(q) + 1;

}

polynomial rem(polynomial const* p, polynomial const* q) {

}

/* Assumes that the terms of the polynomials are sorted by exponent!  */
long lc(polynomial const* p) {
    return p->terms[p->n - 1].coeff;
}
/* Assumes that the terms of the polynomials are sorted by exponent!  */
int deg(polynomial const* p) {
    return p->terms[p->n - 1].exp;
}