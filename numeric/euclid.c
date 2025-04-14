#include "./euclid.h"
#include <assert.h>
#include "stdio.h"

long gcd(long a, long b) {
    if (a < b) {
        return gcd(b, a);
    }
    long temp = 0;
    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

long list_gcd(size_t num, long nums[]) {
    assert(num > 1);
    long curr_gcd = nums[0];
    for (int i = 1; i < num; i++) {
        curr_gcd = gcd(curr_gcd, nums[i]);
    }
    return curr_gcd;
}

extended_euclid_ret extended_euclid(long a, long b) {
    if (a < b) {
        return extended_euclid(b, a);
    }
    long c = 0, d = 1;
    long s = 1, t = 0;
    
    long quo = 0;
    long temp = 0;
    
    while (b != 0) {
        // normal euclid step
        quo = a / b;

        temp = b;
        b = a % b;
        a = temp;

        // extended step
        temp = s - quo * c;
        s = c;
        c = temp;

        temp = t - quo * d;
        t = d;
        d = temp;
    }

    extended_euclid_ret ret = {
        .gcd = a,
        .a = s,
        .b = t
    };
    return ret;
}