#include "./sum.h"
#include "../numeric/euclid.h"
#include "../util/heap.h"
#include "../numeric/pow.h"

#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "assert.h"
#include "string.h" // for memcpy

sum init_polynomial(size_t num_terms, int const coeffs[num_terms], int const degs[num_terms]) {
    sum p = {
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

sum zero_polynomial() {
    sum out;
    out.terms = calloc(1, sizeof(term));
    out.n = 0;
    return out;
}

/* Assumes that the terms are sorted. */
sum add(const sum* const p1, const sum* const p2) {
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
    term* temp = realloc(terms, k * sizeof(term));
    if(!temp) {
        perror("Error reallocating in add");
        exit(EXIT_FAILURE);
    }
    sum p = {
        .n = k,
        .terms = temp
    };

    // handle the exceptional case where all terms cancel
    if (!k) {
        return zero_polynomial();
    }
    return p;
}

sum scalar_prod(long s, const sum* const p) {
    sum g = {
        .n = p->n,
        .terms = malloc(p->n * sizeof(term))
    };
    memcpy(g.terms, p->terms, p->n * sizeof(term));
    for (size_t i = 0; i < g.n; i++) {
        g.terms[i].coeff *= s;
    }
    return g;
}

void scalar_prod_in_place(long s, sum* const p) {
    for (size_t i = 0; i < p->n; i++) {
        p->terms[i].coeff *= s;
    }
}

sum negate(const sum* const p) {
    term* terms = calloc(p->n, sizeof(term));
    for (size_t i = 0; i < p->n; i++) {
        terms[i].coeff = -p->terms[i].coeff;
        terms[i].exp = p->terms[i].exp;
    }
    sum g = {
        .n = p->n,
        .terms = terms
    };

    return g;
}

void negate_in_place(sum* const p) {
    for (size_t i = 0; i < p->n; i++) {
        p->terms[i].coeff = -p->terms[i].coeff;
    }
}

void display(const sum* const p) {
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

void free_polynomial(sum* p) {
    if (!p) {
        return;
    }
    p->n = 0;
    free(p->terms);
    p->terms = 0;
}

/* Computes the content of a polynomial p */
long cont(const sum* const p) {
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

sum prim(const sum* const p) {
    long c = cont(p);
    term* terms = calloc(p->n, sizeof(term));

    for (int i = 0; i < p->n; i++) {
        terms[i].coeff = p->terms[i].coeff / c;
        terms[i].exp = p->terms[i].exp;
    }
    sum g = {
        .n = p->n,
        .terms = terms
    };
    return g;
}

void prim_in_place(sum* const p) {
    long c = cont(p);
    for (int i = 0; i < p->n; i++) {
        // each coefficient is divisible by the content, so integer divison makes sense
        p->terms[i].coeff /= c;
    }
}

sum prod(const sum* const p, const sum* const q) {
    if (!p->n || !q->n) {
        sum g = {
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
    term* temp = realloc(out, new_index * sizeof(term)); 
    if (!temp) {
        perror("Error reallocating in prod");
        exit(EXIT_FAILURE);
    }
    out = temp;

    sum g = {
        .n = new_index,
        .terms = out
    };
    return g;
} 

sum pquo(const sum* const p, const sum* const q) {
    if (deg(p) < deg(q)) {
        return zero_polynomial();
    }

    long b = lc(q);
    b = long_pow(b, deg(p) - deg(q) + 1);
    sum g = scalar_prod(b, p);
    return quo(&g, q);
}

sum prem(const sum* const p, const sum* const q) {
    sum g = pquo(p, q);
    negate_in_place(&g);
    sum gq = prod(&g, q);

    free(g.terms);

    long b = lc(q);
    b = long_pow(b, deg(p) - deg(q) + 1);
    sum sp = scalar_prod(b, p);
    sum out = add(&sp, &gq);
    free(gq.terms);
    free(sp.terms);
    return out;
}

/* Requires that deg(p) >= deg(q). Returns p/q. */
sum quo(const sum* const p, const sum* const q) {
    assert(deg(p) >= deg(q));

    term* terms = calloc(deg(p) - deg(q) + 1, sizeof(term));
    size_t n = deg(p) - deg(q) + 1;

    sum g = {
        .terms = terms,
        .n = 0
    };

    int d = deg(q);
    long c = lc(q);

    sum curr_rem = {
        .terms = malloc(p->n * sizeof(term)),
        .n = p->n
    };

    sum curr_prod = {
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
    term* temp = realloc(g.terms, i * sizeof(term)); 
    if (!temp) {
        perror("Error reallocating in quo");
        exit(EXIT_FAILURE);
    }
    g.terms = temp; 
    free(curr_prod.terms);
    curr_prod.terms = 0;
    free(curr_rem.terms);
    curr_rem.terms = 0;
    return g;
}

sum rem(const sum* const p, const sum* const q) {
    sum g = quo(p, q);
    negate_in_place(&g);
    sum gq = prod(&g, q);
    free(g.terms);
    sum out = add(p, &gq);
    free(gq.terms);
    return out;
}

sum prim_gcd(const sum* const p, const sum* const q) {
    if (deg(p) < deg(q)) {
        return prim_gcd(q, p);
    }

    long a = cont(p);
    long b = cont(q);
    long c = gcd(a, b);

    sum p1 = prim(p);
    sum q1 = prim(q);
    sum r;

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
long lc(const sum* const p) {
    return p->terms[0].coeff;
}
/* Assumes that the terms of the polynomials are sorted by exponent!  */
int deg(const sum* const p) {
    return p->terms[0].exp;
}