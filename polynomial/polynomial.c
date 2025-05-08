#include "./polynomial.h"
#include "../numeric/euclid.h"
#include "../util/heap.h"
#include "../numeric/pow.h"

#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "assert.h"
#include "string.h" // for memcpy

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

polynomial zero_polynomial() {
    polynomial out;
    out.terms = calloc(1, sizeof(term));
    out.n = 0;
    return out;
}

/* Assumes that the terms are sorted. */
polynomial add(const polynomial* const p1, const polynomial* const p2) {
    size_t max_terms = p1->n + p2->n;
    term* terms = calloc(max_terms, sizeof(term));

    int i = 0, j = 0, k = 0;
    while (i < p1->n && j < p2->n) {
        if (p1->terms[i].exp == p2->terms[j].exp) {
            if (p1->terms[i].coeff + p2->terms[j].coeff) {
                terms[k].coeff = (p1->terms[i].coeff + p2->terms[j].coeff);
                terms[k].exp = p1->terms[i].exp;
                k++;
            }
            i++; j++;
        }
        else if (p1->terms[i].exp < p2->terms[j].exp) {
            terms[k].coeff = p2->terms[j].coeff;
            terms[k].exp = p2->terms[j].exp;
            j++; k++;
        }
        else {
            terms[k].coeff = p1->terms[i].coeff;
            terms[k].exp = p1->terms[i].exp;
            i++; k++;
        }
    }

    while (i < p1->n) {
        terms[k].coeff = p1->terms[i].coeff;
        terms[k].exp = p1->terms[i].exp;
        i++; k++;
    }

    while (j < p2->n) {
        terms[k].coeff = p2->terms[j].coeff;
        terms[k].exp = p2->terms[j].exp;
        j++; k++;
    }

    polynomial p = {
        .n = k,
        .terms = realloc(terms, k * sizeof(term))
    };

    // handle the exceptional case where all terms cancel
    if (!k) {
        return zero_polynomial();
    }
    return p;
}

polynomial scalar_prod(long s, const polynomial* const p) {
    polynomial g = {
        .n = p->n,
        .terms = malloc(p->n * sizeof(term))
    };
    memcpy(g.terms, p->terms, p->n * sizeof(term));
    for (size_t i = 0; i < g.n; i++) {
        g.terms[i].coeff *= s;
    }
    return g;
}

void scalar_prod_in_place(long s, polynomial* const p) {
    for (size_t i = 0; i < p->n; i++) {
        p->terms[i].coeff *= s;
    }
}

polynomial negate(const polynomial* const p) {
    term* terms = calloc(p->n, sizeof(term));
    for (size_t i = 0; i < p->n; i++) {
        terms[i].coeff = -p->terms[i].coeff;
        terms[i].exp = p->terms[i].exp;
    }
    polynomial g = {
        .n = p->n,
        .terms = terms
    };

    return g;
}

void negate_in_place(polynomial* const p) {
    for (size_t i = 0; i < p->n; i++) {
        p->terms[i].coeff = -p->terms[i].coeff;
    }
}

void display(const polynomial* const p) {
    for (size_t i = 0; i < p->n; i++) {
        printf("%ld", p->terms[i].coeff);
        if (p->terms[i].exp > 0) {
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
long cont(const polynomial* const p) {
    if (p->n == 1) {
        return p->terms[0].coeff;
    }
    long curr_gcd = p->terms[0].coeff;
    for (int i = 1; i < p->n; i++) {
        curr_gcd = gcd(curr_gcd, p->terms[i].coeff);
    }
    if (curr_gcd < 0) return -curr_gcd;
    return curr_gcd;
}

polynomial prim(const polynomial* const p) {
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

void prim_in_place(polynomial* const p) {
    long c = cont(p);
    for (int i = 0; i < p->n; i++) {
        // each coefficient is divisible by the content, so integer divison makes sense
        p->terms[i].coeff /= c;
    }
}

polynomial prod(const polynomial* const p, const polynomial* const q) {
    if (!p->n || !q->n) {
        polynomial g = {
            .terms = malloc(0),
            .n = 0
        };
        return g;
    }

    term* out = calloc(p->n * q->n, sizeof(term));
    for (size_t i = 0; i < p->n; i++) {
        for (size_t j = 0; j < q->n; j++) {
            out[q->n * i + j].coeff = p->terms[i].coeff * q->terms[j].coeff;
            out[q->n * i + j].exp = p->terms[i].exp + q->terms[j].exp;
        }
    }
    // sort array
    out = heap_sort(p->n * q->n, out);
    // collect like terms
    int last_exp = out[0].exp;
    long last_coeff = 0;
    size_t curr_index = 0;
    size_t new_index = 0;
    while (curr_index < p->n * q->n) {
        if (out[curr_index].exp < last_exp) {
            out[new_index].coeff = last_coeff;
            out[new_index].exp = last_exp;
            // reset the coefficient sum
            last_coeff = 0;
            new_index++; 
        }
        last_exp = out[curr_index].exp;
        last_coeff += out[curr_index].coeff;
        curr_index++;
    }
    // add the lowest degree term (wasn't added in the while loop)
    out[new_index].coeff = last_coeff;
    out[new_index].exp = last_exp;
    new_index++;
    // avoid wasting memory
    out = realloc(out, new_index * sizeof(term));
    polynomial g = {
        .n = new_index,
        .terms = out
    };
    return g;
} 

polynomial pquo(const polynomial* const p, const polynomial* const q) {
    if (deg(p) < deg(q)) {
        return zero_polynomial();
    }

    long b = lc(q);
    b = long_pow(b, deg(p) - deg(q) + 1);
    polynomial g = scalar_prod(b, p);
    return quo(&g, q);
}

polynomial prem(const polynomial* const p, const polynomial* const q) {
    polynomial g = pquo(p, q);
    negate_in_place(&g);
    polynomial gq = prod(&g, q);

    free(g.terms);

    long b = lc(q);
    b = long_pow(b, deg(p) - deg(q) + 1);
    polynomial sp = scalar_prod(b, p);
    polynomial out = add(&sp, &gq);
    free(gq.terms);
    free(sp.terms);
    return out;
}

/* Requires that deg(p) >= deg(q). Returns p/q. */
polynomial quo(const polynomial* const p, const polynomial* const q) {
    assert(deg(p) >= deg(q));

    term* terms = calloc(deg(p) - deg(q) + 1, sizeof(term));
    size_t n = deg(p) - deg(q) + 1;

    polynomial g = {
        .terms = terms,
        .n = 0
    };

    int d = deg(q);
    long c = lc(q);

    polynomial curr_rem = {
        .terms = malloc(p->n * sizeof(term)),
        .n = p->n
    };

    polynomial curr_prod = {
        .terms = malloc(q->n * sizeof(term)),
        .n = q->n
    };

    memcpy(curr_rem.terms, p->terms, p->n * sizeof(term));

    term t;
    size_t i = 0;
    while (i < n) {
        t.exp = deg(&curr_rem);
        t.coeff = lc(&curr_rem);

        // if remainder has degree less than q or if 
        // the leading coefficient of q doesn't divide the leading coefficient of the remainder.
        if (t.exp < d || (t.coeff % c) ) {
            break;
        }

        g.terms[i].exp = t.exp - d;
        g.terms[i].coeff = t.coeff / c;
        g.n += 1;

        for (size_t j = 0; j < q->n; j++) {
            curr_prod.terms[j].exp = q->terms[j].exp + g.terms[i].exp;
            curr_prod.terms[j].coeff = q->terms[j].coeff * g.terms[i].coeff;
        }

        negate_in_place(&curr_prod);
        curr_rem = add(&curr_rem, &curr_prod);
        i++;
    }
    // avoid memory leak
    g.terms = realloc(g.terms, i * sizeof(term));
    free(curr_prod.terms);
    curr_prod.terms = 0;
    free(curr_rem.terms);
    curr_rem.terms = 0;
    return g;
}

polynomial rem(const polynomial* const p, const polynomial* const q) {
    polynomial g = quo(p, q);
    negate_in_place(&g);
    polynomial gq = prod(&g, q);
    free(g.terms);
    polynomial out = add(p, &gq);
    free(gq.terms);
    return out;
}

polynomial prim_gcd(const polynomial* const p, const polynomial* const q) {
    if (deg(p) < deg(q)) {
        return prim_gcd(q, p);
    }

    long a = cont(p);
    long b = cont(q);
    long c = gcd(a, b);

    polynomial p1 = prim(p);
    polynomial q1 = prim(q);
    polynomial r;

    // while q1 is not zero
    while(lc(&q1)) {
        r = prem(&p1, &q1);

        free(p1.terms);
        p1 = q1;
        prim_in_place(&r);
        q1 = r;
    }
    free(r.terms);
    scalar_prod_in_place(c, &p1);
    return p1;
}

/* Assumes that the terms of the polynomials are sorted by exponent!  */
long lc(const polynomial* const p) {
    return p->terms[0].coeff;
}
/* Assumes that the terms of the polynomials are sorted by exponent!  */
int deg(const polynomial* const p) {
    return p->terms[0].exp;
}