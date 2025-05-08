#include "./pow.h"
#include <stdio.h>

long long_pow(long a, long b) {
    if (b < 0) perror("Cannot raise long to negative exponent");
    if (!b) return 1;

    long c = 0;
    while (b) {
        if (b & 1) {
            c += a;
        }
        a = a * a;
        b = b >> 1;
    }
    return c;
}