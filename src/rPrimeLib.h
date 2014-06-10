// Author: Emmanuel Odeke <odeke@ualberta.ca>
#ifndef _R_PRIME_LIB_H
#define _R_PRIME_LIB_H
  #include "typedefs.h"
  #include "radTrie.h"

  RTrie *primes(const uint64 primeCount);
  Bool isPrime(const uint64, RTrie *);

#endif // _R_PRIME_LIB_H
