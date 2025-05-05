#include "../../polynomial/polynomial.h"
#include "stdio.h"

int main(int argc, char* argv[argc]) {

    // testing polynomials
    int c1[] = {[0] = 1, [1] = 2};
    int e1[] = {[0] = 0, [1] = 2};

    int c2[] = {[0] = 1, [1] = 1};
    int e2[] = {[0] = 0, [1] = 3};

    polynomial p = init_polynomial(2, c1, e1);
    polynomial g = init_polynomial(2, c2, e2);

    display(&p);
    display(&g);

    polynomial h = add(&p, &g);
    display(&h);

    polynomial h2 = prod(&p, &g);
    display(&h2);

    return 0;
}