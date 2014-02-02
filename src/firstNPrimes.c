#include <stdio.h>
#include <math.h>

#include "objFuncs.h"
#include "hashMap.h"

HashMap *__primeSav = NULL;
Bool isPrime(const Object *numObject) {
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
      put(__primeSav, intObject(numValue), intObject(numValue));
      return True;
    } else { // Already memoized
      return True;
    }
  } else {
    return False;
  }
}

int main(int argc, char *argv[]) {
  uint64 i=0, primeCount=1000;
  printf("argc:: %d\n", argc);
  if (argc > 1) {
    if (sscanf(argv[1], "%ld", &primeCount) != 1) {
      primeCount=10000;
    }
  }

  printf("\033[92mGenerating the first: %ld primes\n\033[00m", primeCount);
  __primeSav = createHashMapWithSize((primeCount * 100) + 3);

  // Punching in the known primes
  uint64 knownPrimes[] = { 2, 3, 5, 7, 11, 13, 17 };
  uint64 *trav = knownPrimes,
	 *end = trav + sizeof(knownPrimes)/sizeof(knownPrimes[0]); 
  while (trav < end) {
    put(__primeSav, intObject(*trav), intObject(*trav));
    ++trav;
  }

  while (primeCount) {
    Object *tmpObject = intObject(i);
    if (isPrime(tmpObject)) {
      --primeCount;
    }
    destroyObject(tmpObject);
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

  __primeSav =  destroyHashMap(__primeSav);
  return 0;
}
