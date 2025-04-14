#include "../../numeric/euclid.h"
#include "assert.h"
#include "stdio.h"

int main(int argc, char* argv[argc]) {
    long a = 13 * 7; long b = 13 * 15;
    long c = gcd(a, b);
    printf("the gcd of %ld and %ld is %ld\n", a, b, c); 

    a = 37; b = 185;
    printf("the gcd of %ld and %ld is %ld\n", a, b, gcd(a, b)); 

    long arr[4] = {13 * 15, 13 * 3 * 7, 3 * 9 * 5, 3 * 61};
    printf("The gcd of %ld, %ld, %ld, %ld is %ld\n", arr[0], arr[1], arr[2], arr[3], list_gcd(4, arr));


    a = 13; b = 9;
    extended_euclid_ret r = extended_euclid(a, b);
    printf("The gcd of %ld and %ld is %ld, with %ld, %ld being Bezout coefficients\n", 
            a, b, r.gcd, r.a, r.b);
    
    //assert(a * r.a + b * r.b == 1);

    return 0;
}