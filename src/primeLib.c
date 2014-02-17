// Author: Emmanuel Odeke <odeke@ualberta.ca>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "objFuncs.h"
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

  uint64 i=17;
  Object *tmpObject = intObject(i);

  while (primeCount) {
    *((uint64 *)tmpObject->data) = i;
    if (__isPrime(tmpObject, __primeSav)) {
      --primeCount;
    }
    i += 2; // All other primes from 2 and above are odd
  }

  tmpObject = destroyObject(tmpObject);

#ifdef PRINT_CONTENT
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
#endif

  return __primeSav;
}

inline uint64 truncateLeft(uint64 n, const uint32 base) {
  return base ? (n / base) : n;
}

inline uint64 truncateRight(uint64 n, const uint32 base) {
  uint64 pow10 = log10(n);
  uint64 mod = pow(base, pow10);
  return n % mod;
}

Bool isFullyTruncatable(uint64 n, HashMap *primeH, uint64 (*truncator)(uint64, const uint32)) {
  // Returns True if every truncated suffix of the integer n is also a prime
  if (n < 11) return False;

  Bool isFullyL = True;
  Object *tmpO = intObject(n);
  while (*(uint64 *)(tmpO->data) > 0) {
    if (! __isPrime(tmpO, primeH)){
      isFullyL = False;
      break;
    }

    *((uint64 *)tmpO->data) = truncator(*((uint64 *)tmpO->data), 10);
  }

  tmpO = destroyObject(tmpO);

  return isFullyL;
}

uint64 *truncatables(HashMap *primeH, uint64 (*f)(uint64, const uint32), uint32 *savSize) {
  Chain **pIt = primeH->list, **pEnd = pIt + primeH->size;

  uint32 i=0, bufSize = 100;
  uint64 *sav = (uint64 *)malloc(sizeof(uint64) * bufSize);

  while (pIt < pEnd) {
    if (*pIt != NULL) {
      Object *o = (*pIt)->value;
      KeyValue *kv = o->data;
      if (isFullyTruncatable(*(uint64 *)(kv->key->data), primeH, f) == True) {
        if (i >= bufSize) {
          bufSize += 100;
          sav = (uint64 *)realloc(sav, sizeof(uint64) * bufSize);
        }

        *(sav + i) = *(uint64 *)(kv->key->data);
        ++i;
      }
    }
    ++pIt;
  }

  if (i) {
    sav = (uint64 *)realloc(sav, sizeof(uint64) * i);
  }

  *savSize = i;
  return sav;
}

int32 bSearch(const uint64 query, const uint64 *arr, const uint32 size) {
  uint32 low=0, high=size-1, mid;
  while (low <= high) {
    mid = (low + high) >> 1;
    if (arr[mid] == query) return mid;
    if (arr[mid] < query) {
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }

  return -1;
}

uint64 intersectionSum(const int64 *l, const uint32 lSize, const uint64 *r, const uint32 rSize) {
  uint64 sum = 0;
  if (l != NULL && r != NULL) {
    uint32 i;
    for (i=0; i < lSize; ++i) {
      if (bSearch(l[i], r, rSize) != -1) {
        printf("Found: %ld\n", l[i]);
        sum += l[i];
      }
    }
  }

  return sum;
}

#ifdef REV_MAIN
int main(int argc, char *argv[]) {
  uint64 i=0, primeCount=1000;
  if (argc > 1) {
    if (sscanf(argv[1], "%ld", &primeCount) != 1) {
      primeCount=10000;
    }
  }

  HashMap *nprimes = primes(primeCount);

  uint32 lSize, rSize; 
  uint64 *lT = truncatables(nprimes, truncateLeft, &lSize);
  uint64 *rT = truncatables(nprimes, truncateRight, &rSize);

  printf("Sum:: %ld\n", intersectionSum(lT, lSize, rT, rSize));
  if (lT != NULL) free(lT);
  if (rT != NULL) free(rT);

  Object *primeMapObject = hashMapObject(nprimes, Heapd);
  printObject(primeMapObject);
  primeMapObject = destroyObject(primeMapObject);
  return 0;
}
#endif
