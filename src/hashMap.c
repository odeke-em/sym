// Author: Emmanuel Odeke <odeke@ualberta.ca>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "hashMap.h"
#include "objFuncs.h"


inline Chain *allocChain(void) {
  return (Chain *)malloc(sizeof(Chain));
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

HashMap *destroyHashMap(HashMap *h) {
  if (h != NULL) {
    if (h->list != NULL) {
      Chain **cIt = h->list, **cEnd = cIt + h->size;
      while (cIt < cEnd) {
        if (*cIt != NULL) {
        #ifdef DEBUG
          printf("Freeing chain: %ld\n", h->size - (cEnd - cIt));
          printChain(*cIt);
        #endif
          *cIt = destroyChain(*cIt);
        }
        ++cIt;
      }

      free(h->list);
      h->list = NULL;
    }

    free(h);
    h = NULL;
  }

  return h;
}

inline Bool isEmpty(const HashMap *h) {
  return (h == NULL || h->list == NULL || ! h->size) ? True : False;
};

inline uint32 getHSize(const HashMap *h) {
  return isEmpty(h) ? 0 : h->size;
}

Chain *get(const HashMap *hm, const Object *key) {
  if (! isEmpty(hm)) {
    uint64 hash = getHash(key);
    return *(hm->list + (hash % hm->size));
  } else {
    return NULL;
  }
}

HashMap *createHashMapWithSize(const uint32 len) {
  HashMap *h = (HashMap *)malloc(sizeof(HashMap));
  if (h == NULL) {
    raiseError("No memory for new hashMap");
  } else {
    h->size = len;
    h->list = (Chain **)malloc(sizeof(Chain *) * len);
    Chain **cIt = h->list, **cEnd = cIt + len;
    while (cIt < cEnd) {
      *cIt++ = NULL;
    }
    return h;
  }
}

uint64 put(const HashMap *h, Object *key, Object *value) {
  if (h != NULL) {
    if (h->list != NULL) {
      uint64 hash = getHash(key);
      if (h->size) {
        hash %= h->size;
        // Allowing collisions for starters
        Object *kv = kvObject(key, value);
        *(h->list + hash) = prepend(*(h->list + hash), kv);
        return hash;
      } else {
        raiseError("Can't insert into an empty hashMap!");
      }
    } else {
      raiseError("HashList is NULL");
    }
  } else {
    return -1;
  }
}

inline Chain *pop(const HashMap *h, const Object *key) {
  if (isEmpty(h)) {
    return NULL;
  } else {
    uint64 keyHash = getHash(key);
    Chain *popd = *(h->list + keyHash);
    *(h->list + keyHash) = NULL;
    return popd;
  }
}

void printChain(Chain *n) {
  printf("{");
  if (n != NULL) {
    Chain *trav;
    for (trav = n; trav != NULL; trav = trav->next) {
      printObject(trav->value);
      if (trav->next)
        printf(", ");
    }
  }
  printf("}\n");
}

void printHashMap(HashMap *hm) {
  putchar('{');
  Bool hadPrev = False;
  if (hm != NULL) {
    if (hm->list != NULL) {
      Chain **it = hm->list, **end = it + hm->size;
      while (it < end) {
        if (*it != NULL) {
          if (hadPrev == True) printf(", ");
          printChain(*it);
          hadPrev = True;
        }
        ++it;
      }
    }
  }
  putchar('}');
}

