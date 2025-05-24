#include "../../polynomial/sum.h"
#include "stdio.h"

int main(int argc, char* argv[argc]) {
    int c1[] = {[1] = 1, [0] = 2};
    int e1[] = {[1] = 0, [0] = 2};

    int c2[] = {[1] = 1, [0] = 1};
    int e2[] = {[1] = 0, [0] = 3};

    sum p = init_polynomial(2, c1, e1);
    sum g = init_polynomial(2, c2, e2);

    display(&p);
    display(&g);

    sum h = add(&p, &g);
    display(&h);

    sum h2 = prod(&p, &g);
    display(&h2);

    int c3[] = {[0] = 1, [1] = -1};
    int e3[] = {[0] = 4, [1] = 0};

    sum x = init_polynomial(2, c3, e3);

    int c4[] = {[0] = 1, [1] = -1};
    int e4[] = {[0] = 1, [1] = 0};

    sum y = init_polynomial(2, c4, e4);

    sum z = quo(&x, &y);
    display(&z);

    sum z2 = prod(&p, &y);
    display(&z2);

    sum z3 = prim_gcd(&y, &z2);
    display(&z3);

    return 0;
}