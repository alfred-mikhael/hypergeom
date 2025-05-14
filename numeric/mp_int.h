#ifndef _MP_INT_H_INCLUDED_
#define _MP_INT_H_INCLUDED_

#include <stdbool.h>

/*multiprecision integer in base 10. */
struct mp_int;
/*p-adic representation of an arbitrarily large integer. The modulus must be positive and less than 2^64. */
struct padic_int;

typedef struct mp_int mp_int;
typedef struct padic_int padic_int;

/*Uses the provided string (\0 terminated) to initialize a mp_int. The string must have length at most 2^64-1 digits. */
mp_int mpint_init(const char*);

mp_int mpint_add(const mp_int* const, const mp_int* const);
mp_int mpint_sub(const mp_int* const, const mp_int* const);
mp_int mpint_prod(const mp_int* const, const mp_int* const);
mp_int mpint_div(const mp_int* const, const mp_int* const);
mp_int mpint_pow(const mp_int* const, const mp_int* const);

void mpint_display(const mp_int* const);

bool mpint_lt(const mp_int* const, const mp_int* const);
bool mpint_eq(const mp_int* const, const mp_int* const);
bool mpint_lt_i(const mp_int* const, const int);
bool mpint_eq_i(const mp_int* const, const int);
bool mpint_nz(const mp_int* const);


bool padic_lt(const padic_int* const, const padic_int* const);
bool padic_eq(const padic_int* const, const padic_int* const);
bool padic_lt_i(const padic_int* const, const int);
bool padic_eq_i(const padic_int* const, const int);
bool padic_nz(const padic_int* const);

void padic_display(const padic_int* const);

padic_int mpint_to_padic(const mp_int* const);
mp_int padic_to_mpint(const padic_int* const);

#endif