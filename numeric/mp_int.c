#include "./mp_int.h"
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>

typedef struct darr darr;
struct darr {
    size_t n;
    size_t capacity;
    uint64_t* arr;
};

darr* init_darr(size_t s) {
    darr d;
    d.n = 0;
    d.capacity = s;
    d.arr = malloc(s * sizeof(uint64_t));
    if (!d.arr) perror("Could not allocate memory in init_darr");
    return &d;
}

void darr_insert(darr* const d, uint64_t item) {
    if (d->n < d->capacity) {
        d->n++;
        d->arr[d->n] = item;
    } else {
        d->arr = realloc(d->arr, 2 * d->capacity * sizeof(uint64_t));
        if (!d->arr) perror("Issue rellocating memory on insert to darr");
        d->n++;
        d->capacity *= 2;
        d->arr[d->n] = item;
    }
}

uint64_t darr_pop(darr* const d) {
    uint64_t out = d->arr[--d->n];
    if (d->n < 2 && d->n < d->capacity / 4) {
        d->arr = realloc(d->arr, d->capacity / 2 * sizeof(uint64_t));
    }
}

void darr_reverse(darr* const d) {
    size_t i = 0, j = d->n - 1;
    int64_t temp;
    while (i < j) {
        temp = d->arr[i];
        d->arr[i] = d->arr[j];
        d->arr[j] = temp;
        i++; j--;
    }
}

inline uint64_t pow_of_10(uint16_t n) {
    uint64_t accum = 1;
    while (n-- > 0) {
        accum *= 10;
    }
    return accum;
}

struct mp_int {
    // true for negative, false for positive
    bool sgn;
    darr* arr;
};
struct padic_int {
    bool sgn;
    uint64_t modulus;
    darr* arr;
};

mp_int mpint_init(const char* num) {
    size_t len_num = 0;
    while (num[len_num] != "\0") len_num++;
    assert(len_num > 0);

    mp_int out = {
        .arr = init_darr(2),
        .sgn = 0
    };

    size_t i = 0;
    if (num[0] == "-" || num[0] == "+") {
        out.sgn = (num[0] == "-");
        i++;
    }

    while (i < len_num) {
        int j = 0;
        uint64_t accum = 0;
        while (j < 18 && (i + j) < len_num) {
            accum += (num[i + j] * pow_of_10(18-j-1));
            j++;
        }
        darr_insert(out.arr, accum);
        i += j;
    }
    darr_reverse(out.arr);  
    return out;
}

mp_int mpint_add(const mp_int* const m1, const mp_int* const m2) {
    if (m1->sgn && !m2->sgn) {
        mp_int temp;
        temp.arr = m1->arr;
        temp.sgn = 0;
        return mpint_sub(m2, &temp);
    } else if (!m1->sgn && m2->sgn) {
        mp_int temp;
        temp.arr = m2->arr;
        temp.sgn = 0;
        return mpint_sub(m1, &temp);    
    }
    // can assume now that both m1 and m2 have the same sign.
    mp_int out = {
        .arr = init_darr(m1->arr->n > m2->arr->n ? m1->arr->n + 1 : m2->arr->n + 1),
        .sgn = 0
    };
    size_t i = 0, k = 0;

    // s for sum, r for remainder
    uint64_t s = 0;
    uint8_t r = 0;
    while (i < m1->arr->n && i < m2->arr->n) {
        s = r + m1->arr->arr[i] + m2->arr->arr[i];
        r = m1->arr->arr[i] > UINT64_MAX - m2->arr->arr[i];
        darr_insert(out.arr, s);
        i++;
    }
    while (i < m1->arr->n) {
        s = m1->arr->arr[i] + r;
        r = (s == 0);
        darr_insert(out.arr, s);
        i++;
    }
    while (i < m2->arr->n) {
        s = m2->arr->arr[i] + r;
        r = (s == 0);
        darr_insert(out.arr, s);
        i++;
    }
    if (r) {
        darr_insert(out.arr, r);
    }
    out.sgn = m1->sgn;
    return out;
}

/* Returns 1 if |m1| > |m2|, 0 if |m1| = |m2|, and -1 if |m1| < |m2|*/
int8_t mpint_cmp_magnitude(const mp_int* const m1, const mp_int* const m2) {
    size_t i = m1->arr->n - 1;
    size_t j = m2->arr->n - 1;

    while (m1->arr->arr[i] == 0) {
        i--;
    } 
    while (m2->arr->arr[j] == 0) {
        j--;
    }
    if (i > j) return 1;
    if (i < j) return -1;

    while (i >= 0) {
        if (m1->arr->arr[i] == m2->arr->arr[j]) {
            i--;
        } else if (m1->arr->arr[i] > m2->arr->arr[i]) {
            return 1;
        } else {
            return -1;
        }
    }
    return 0;
}

mp_int mpint_sub(const mp_int* const m1, const mp_int* const m2) {
    // if m2 is negative, then do m1 + |m2|
    // this might send back to mpint_sub if m1 is negative, but the recursion 
    // only happens one time.
    if (m2->sgn) {
        mp_int temp;
        temp.arr = m2->arr;
        temp.sgn = 0;
        return mpint_add(m1, &temp);
    }
    // if m1 is negative (and we know m2 is positive), do -(|m1| + |m2|)
    else if (m1->sgn) {
        mp_int temp;
        temp.arr = m2->arr;
        temp.sgn = 1;
        return mpint_add(m1, &temp);
    }

    // know that m1, m2 > 0;
    // ensure that |m1| >= |m2|. If not, then return -(m2 - m1).
    if (mpint_cmp_magnitude(m1, m2) == -1) {
        mp_int t1, t2;
        t1.arr = m1->arr;
        t1.sgn = 0;

        t2.arr = m2->arr;
        t2.sgn = 0;

        mp_int out = mpint_sub(&t2, &t1);
        out.sgn = !out.sgn;
        return out;
    }

    // can now assume that both m1 and m2 are positive, |m1| >= |m2|.
    mp_int out = {
        .arr = init_darr(m1->arr->n > m2->arr->n ? m1->arr->n + 1 : m2->arr->n + 1),
        .sgn = 0
    };
    size_t i = 0, k = 0;

    // s for sum, r for remainder
    uint64_t s = 0;
    uint8_t r = 0;
    while (i < m1->arr->n && i < m2->arr->n) {
        s = m1->arr->arr[i] - m2->arr->arr[i] - r;
        r = (m1->arr->arr[i] < m2->arr->arr[i]) || (m1->arr->arr[i] == m2->arr->arr[i] && r > 0);
        darr_insert(out.arr, s);
        i++; k++;
    }
    while (i < m1->arr->n) {
        s = m1->arr->arr[i] - r;
        r = (s + r == 0 && r > 0);
        darr_insert(out.arr, s);
        i++; k++;
    }
    // remove leading zeroes
    while (out.arr->arr[k] == 0) {
        darr_pop(out.arr);
        k--;
    }
    return out;   
}

mp_int mpint_prod(const mp_int* const, const mp_int* const);
mp_int mpint_div(const mp_int* const, const mp_int* const);
mp_int mpint_pow(const mp_int* const, const mp_int* const);

bool mpint_lt(const mp_int* const, const mp_int* const);
bool mpint_eq(const mp_int* const, const mp_int* const);
bool mpint_lt_i(const mp_int* const, const int);
bool mpint_eq_i(const mp_int* const, const int);
bool mpint_nz(const mp_int* const);


bool padic_lt(const padic_int* const, const padic_int* const);
bool padic_eq(const padic_int* const, const padic_int* const);
bool mpint_lt_i(const mp_int* const, const int);
bool mpint_eq_i(const mp_int* const, const int);
bool mpint_nz(const mp_int* const);

padic_int mpint_to_padic(const mp_int* const);
mp_int padic_to_mpint(const padic_int* const);
