#ifndef _EUCLID_H_INCLUDED_
#define _EUCLID_H_INCLUDED_

#include <stddef.h>

long gcd(long a, long b);

long list_gcd(size_t num, long nums[]);

typedef struct extended_euclid_ret extended_euclid_ret;

struct extended_euclid_ret {
    long a; 
    long b;
    long gcd;
};

extended_euclid_ret extended_euclid(long a, long b);

#endif