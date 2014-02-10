// Author: Emmanuel Odeke <odeke@ualberta.ca>
#ifndef _PRIME_LIB_H
#define _PRIME_LIB_H
  #include "typedefs.h"
  #include "objFuncs.h"
  #include "hashMap.h"

  HashMap *primes(const uint64 primeCount);

  Bool __isPrime(const Object *numObject, HashMap *__primeSav);

  int32 bSearch(const uint64 query, const uint64 *arr, const uint32 size);

  uint64 intersectionSum(const int64 *l, const uint32 lSize, const uint64 *r, const uint32 rSize);

  // Returns True if a prime is truncatable and so are all it's
  // [su/pre]fixes as determined by the truncator function
  Bool isFullyTruncatable(uint64 n, HashMap *primeH, uint64 (*truncator)(uint64, const uint32));

  // Given a truncator function, return a list of prime numbers that qualify
  uint64 *truncatables(HashMap *primeH, uint64 (*f)(uint64, const uint32), uint32 *savSize);

  // Truncator functions here
  inline uint64 truncateLeft(uint64 n, const uint32 base);
  inline uint64 truncateRight(uint64 n, const uint32 base);
#endif
