// Author: Emmanuel Odeke <odeke@ualberta.ca>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "primeLib.h"

Bool __isPrime(const Object *numObject, HashMap *__primeSav) {
  if (numObject != NULL && numObject->data != NULL) {
    switch(numObject->typeTag) {
      case LIntTag: case IntTag: break;
      default: return False;
    }

    uint64 numValue = *(uint64 *)numObject->data;
    if (numValue < 2) {
      return False;
    } else if (! (numValue & 1)) { // Even number detected
      return (numValue == 2 ? True : False);
    }

    if (get(__primeSav, numObject) == NULL) {
      uint64 i, sqrtValue = sqrt(numValue);
      for (i=3; i <= sqrtValue; i += 2) {
        if (! (numValue % i)) return False;
      }
      // By this point we are clear to return True
      // But first memoize that value
    #ifdef DEBUG
      printf("Sqrt: %ld numValue: %ld\n", sqrtValue, numValue);
    #endif
      Object *keyValue = intObject(numValue);
      put(__primeSav, keyValue, keyValue);
      return True;
    } else { // Already memoized
      return True;
    }
  } else {
    return False;
  }
}

HashMap *primes(uint64 primeCount) {
  printf("\033[92mGenerating the first: %ld primes\n\033[00m", primeCount);
  HashMap *__primeSav = createHashMapWithSize((primeCount * 100) + 3);

  // Punching in the known primes
  uint64 knownPrimes[] = { 2, 3, 5, 7, 11, 13, 17 };
  uint64 *trav = knownPrimes,
	       *end = trav + sizeof(knownPrimes)/sizeof(knownPrimes[0]); 

  while (trav < end) {
    Object *travObject = intObject(*trav);
    put(__primeSav, travObject, travObject);
    ++trav;
  }

  uint64 i=0;
  while (primeCount) {
    Object *tmpObject = intObject(i);
    if (__isPrime(tmpObject, __primeSav)) {
      --primeCount;
    }
    tmpObject = destroyObject(tmpObject);
    ++i;
  }

  if (__primeSav != NULL) {
    Chain **pIt = __primeSav->list, **pEnd = pIt + __primeSav->size;
    printf("%-20s%-10s\n", "Index", "Value");
    uint64 primeIndex = 0;
    while (pIt < pEnd) {
      if (*pIt != NULL) {
	      printf("%-20ld", ++primeIndex);
	      printObject((*pIt)->value);
	      printf("\n");
      }
      ++pIt;
    }
  }

  return __primeSav;
}

int main(int argc, char *argv[]) {
  uint64 i=0, primeCount=1000;
  if (argc > 1) {
    if (sscanf(argv[1], "%ld", &primeCount) != 1) {
      primeCount=10000;
    }
  }

  HashMap *nprimes = primes(primeCount);
  nprimes = destroyHashMap(nprimes);
  return 0;
}
