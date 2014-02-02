#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "hashMap.h"
#include "objFuncs.h"

int main() {
#define DEMO_INSERTION
#ifdef DEMO_INSERTION
  FILE *ifp = fopen(__FILE__, "r");
  HashMap *h = createHashMapWithSize(100003);
  while (! feof(ifp)) {
    int bufLen=10, i=0;
    char *buf = (char *)malloc(sizeof(char) * bufLen), c;
    while ((c = getc(ifp)) != EOF) {
      if (i >= bufLen) {
        bufLen += 10;
        buf = (char *)realloc(buf, sizeof(char) * bufLen);
      }
      if (! isalnum(c)) {
        buf[i] = '\0';
        break;
      } else {
        buf[i++] = c;
      }
    }
    if (i) {
      Object *value = charArrObject(buf, Heapd);
      put(h, intObject(i), value);
    } else {
      free(buf);
    }
  }
  fclose(ifp);
  printHashMap(h);

  // Testing get
  KeyValue *querykv = kvStruct(
    charArrObject("Query\0", Stackd), intObject(21)
  );

  Chain *foundValue = get(h, querykv->value);
  putchar('\n');
  printKvStruct(querykv);
  printf("\tFound Value: \033[92m");
  printChain(foundValue);
  printf("\033[00m\n");

  destroyKvStruct(querykv);
   
  h = destroyHashMap(h);
#else

  Object *key = charArrObject("Name\0", Stackd),
       *value = charArrObject("Emmanuel\0", Stackd);
  Object *kv = kvObject(key, value);
  printObject(kv);
  putchar('\n');
  destroyObject(kv);
  HashMap *hm = createHashMapWithSize(10003);
  put(hm,
    charArrObject("HashMap\0", Stackd),
    charArrObject("Drupal\0", Stackd)
  );

  put(hm,
    charArrObject(strdup("Binder\0"), Heapd),
    intObject(1000)
  );

  put(hm,
    intObject(-100), doubleObject(-250)
  );

  printHashMap(hm);
  hm = destroyHashMap(hm);
#endif

  return 0;
}
