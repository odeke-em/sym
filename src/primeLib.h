// Author: Emmanuel Odeke <odeke@ualberta.ca>
#ifndef _PRIME_LIB_H
#define _PRIME_LIB_H
  #include "typedefs.h"
  #include "objFuncs.h"
  #include "hashMap.h"

  HashMap *primes(const uint64 primeCount);
  Bool __isPrime(const Object *numObject, HashMap *__primeSav);
#endif
