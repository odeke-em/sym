// Author: Emmanuel Odeke <odeke@ualberta.ca>
#ifndef _HASH_MAP_H
#define _HASH_MAP_H
  #include "typedefs.h" 
  typedef struct Chain_ {
    Object *value;
    struct Chain_ *next;
  } Chain;

  typedef struct {
    uint32 size;
    Chain **list;
    Bool canCollide;
  } HashMap;

  HashMap *createHashMapWithSize(const uint32 len);

  inline Chain *newChain();
  inline Chain *allocChain();
  Chain *prepend(Chain *n, Object *data);

  Chain *get(const HashMap *h, const Object *key);
  inline Chain *pop(const HashMap *h, const Object *key);
  uint64 put(const HashMap *h, Object *key, Object *data);
  HashMap *destroyHashMap(HashMap *h);

  inline Bool isEmpty(const HashMap *h);
  inline uint32 getHSize(const HashMap *h);
  void printChain(Chain *n);
  void printHashMap(HashMap *hm);
#endif
