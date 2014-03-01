// Author: Emmanuel Odeke <odeke@ualberta.ca>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "objFuncs.h"

inline void incrementRefCount(Object *o) {
  if (o != NULL && o->isFreed == False) ++(*(&(o->refCount)));
}

inline void decrementRefCount(Object *o) {
  if (o != NULL && o->isFreed == False && o->refCount) --(*(&(o->refCount)));
}

inline void __clearRefCount(Object *o) {
  if (o != NULL && o->isFreed == False && o->refCount) *(&(o->refCount)) = 0;
}

void printObject(const Object *o) {
  if (o != NULL) {
    switch(o->typeTag) {
      case LIntTag: {
        printf("%ld", *((uint64 *)o->data));
        break;
      }
      case CharArrayTag: {
        printf("%s", (char *)o->data);
        break;
      }
      case IntTag: {
        printf("%d", *((uint32 *)o->data));
        break;
      }
      case DoubleTag: {
        printf("%2.3f", *((double *)o->data)); 
        break;
      }
      case KeyValueTag: {
        printKvStruct((KeyValue *)o->data);
        break;
      }
      case HashMapTag: {
        printHashMap((HashMap *)o->data);
        break;
      }
      default: break;
    }
  } else {
    printf("nil");
  }
}

inline uint64 getHash(const Object *elem) {
  hashFunc hasher = getHashFuncByObject(elem);
  
  return hasher == NULL ? 0 : hasher(elem->data);
}

inline uint64 intHashFunc(const void *data) {
  return data ? *(int *)data : 0;
}

inline uint64 getHashSize(const void *data) {
  return data ? getHSize((HashMap *)data) : 0;
}

uint64 pjwCharHash(const void *data) {
  uint64 h = 0;
  if (data != NULL) {
    char *dCast = (char *)data;
    int g = 0;
    while (*dCast != '\0') {
      h = (h << 4) + *dCast;
      g = h & 0xf0000000;
      if (g) {
        h ^= (g >> 24);
        h ^= g;
      }
       ++dCast;
    }
  }

  return h;
}

inline uint64 doubleHashFunc(const void *data) {
  return data ? *(int *)data : 0;
}

hashFunc getHashFuncByObject(const Object *o) {
  hashFunc hFunc = NULL;
  if (o != NULL) {
    switch(o->typeTag) {
      case IntTag: {
        hFunc = intHashFunc;
        break;
      }
      case CharArrayTag: {
        hFunc = pjwCharHash;
        break;
      }
      case LIntTag: {
        hFunc = intHashFunc;
        break;
      }
      case HashMapTag: {
        hFunc = getHashSize;
        break;
      }
      default:break;
    }
  }

  return hFunc;
}

inline KeyValue *kvStruct(Object *key, Object *value) {
  KeyValue *kv = (KeyValue *)malloc(sizeof(*kv));
  incrementRefCount(key);
  incrementRefCount(value);
  kv->key = key;
  kv->value = value;
  return kv;
}

inline KeyValue *destroyKvStruct(KeyValue *kv) {
  if (kv != NULL) {
    kv->key   = destroyObject(kv->key);
    kv->value = destroyObject(kv->value);
    free(kv);
    kv = NULL;
  }

  return kv;
}

void printKvStruct(const KeyValue *kv) {
  if (kv != NULL) {
    printObject(kv->key);
    putchar(':');
    printObject(kv->value);
  }
}

inline Object *kvObject(Object *key, Object *value) {
  KeyValue *kv = kvStruct(key, value);
  return newObject(kv, KeyValueTag, Heapd);
}

inline Object *intObject(const uint64 u) {
  uint32 *uMem = (uint32 *)malloc(sizeof(*uMem));
  *uMem = u;
  return newObject(uMem, LIntTag, Heapd);
}

inline Object *hashMapObject(HashMap *hm, const MemTag memTag) {
  return newObject(hm, HashMapTag, memTag);
}

inline Object *doubleObject(const double d) {
  double *dMem = (double *)malloc(sizeof(*dMem));
  *dMem = d;
  return newObject(dMem, DoubleTag, Heapd);
}

inline Object *charArrObject(char *v, const MemTag memTag) {
  Object *newObj = NULL;
  if (v != NULL) {
    newObj = newObject(
      v, CharArrayTag, memTag
    );
  }
  return newObj;
}

inline Object *newObject(void *data, const TypeTag typeTag, const MemTag memTag) {
  Object *o = (Object *)malloc(sizeof(Object));

  o->data = data;
  o->typeTag = typeTag;
  o->memTag = memTag;
  o->isFreed = False;
  o->refCount = 0;
  return o;
}

inline void *__freeAndClearMem(void *mem) {
  if (mem != NULL) {
    free(mem);
    mem = NULL;
  }
  return mem;
}

Object *destroyObject(Object *o) {
  if (o != NULL && o->isFreed == False) {
    if (o->memTag == Heapd && o->data != NULL) {
      switch(o->typeTag) {
        case LIntTag: case CharArrayTag:  // Let these cases fall through
        case IntTag: o->data = __freeAndClearMem(o->data); break;
        case KeyValueTag: {
          o->data = destroyKvStruct(o->data);
        }
        case HashMapTag: {
          o->data = destroyHashMap(o->data);
        }
        default:  o->data = __freeAndClearMem(o->data); break;
      }
    }

    *(&(o->isFreed)) = True;
    free(o);
  }

  return o;
}

inline Chain *allocChain(void) {
  return (Chain *)malloc(sizeof(Chain));
}

Chain *filter(Chain *it, Quantifier qFunc) {
  Chain *filtered = NULL;
  while (it != NULL) {
    if (qFunc(it->value) == True) {
      filtered = prepend(filtered, it->value);
    }
    it = it->next; 
  }

  return filtered;
}

inline Chain *newChain(void) {
  Chain *n = allocChain();
  n->value = NULL;
  n->next = NULL;
  return n;
}

Chain *prepend(Chain *n, Object *data) {
  Chain *aChain = newChain();
  aChain->value = data;
  incrementRefCount(data);
  if (n != NULL) {
    aChain->next = n;
  }
  n = aChain;
#ifdef DEBUG
  printf("Prepending:: %p\n", n);
#endif
  return n;
}

Chain *destroyChain(Chain *n) {
  if (n != NULL) {
    Chain *trav = n, *next = NULL;
    while (trav != NULL) {
      next = trav->next;
      trav->value = destroyObject(trav->value);
      free(trav);
      trav = next;
    }
    n = NULL;
  }
  
  return n;
}
