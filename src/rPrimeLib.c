// Author: Emmanuel Odeke <odeke@ualberta.ca>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "rPrimeLib.h"

inline Bool isPrime(const uint64 q, RTrie *primeMemoizer) {
    if (((q & 1) == 0 && q != 2) || q < 3)
        return False;
    else if (get(primeMemoizer, q) != NULL)
        return True;
    else {
        register unsigned int i=3;
        for (i=3; i <= sqrt(q); i += 2) {
            if (! (q % i)) return False;
        }

        ullong_t *mem = (ullong_t *)malloc(sizeof(ullong_t));
        *mem = q;
        primeMemoizer = put(primeMemoizer, q, mem, True);
        return True;
    }
}

RTrie *primes(const uint64 pCount) {
    RTrie *r = NULL;
    uint64 __l[] = {2, 3, 5, 7, 11, 13, 17};
    register ullong_t i, addedPrimes=0;

    for (i=0; i < sizeof(__l)/sizeof(__l[0]); ++i) {
        uint64 *mem = (uint64 *)malloc(sizeof(uint64));
        *mem = __l[i];
        r = put(r, *mem, mem, True);
        ++addedPrimes;
    }

    i = 17;
    while (addedPrimes < pCount) {
        if (isPrime(i, r)) {
            printf("#%lld i: %lld\n", addedPrimes, i);
            ++addedPrimes;
        }

        i += 2;
    }

    return r;
}

#ifdef _TEST_RPRIMELIB
int main() {
    RTrie *p = primes(1000000);
    p = destroyRTrie(p);
    return 0;
}
#endif // _TEST_RPRIMELIB
