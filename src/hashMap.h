// Author: Emmanuel Odeke <odeke@ualberta.ca>
#ifndef _HASH_MAP_H
#define _HASH_MAP_H
  #include "typedefs.h" 
  HashMap *createHashMapWithSize(const uint32 len);

  Chain *get(const HashMap *h, const Object *key);
  inline Chain *pop(const HashMap *h, const Object *key);
  uint64 put(const HashMap *h, Object *key, Object *data);

  Chain *destroyChain(Chain *n);
  HashMap *destroyHashMap(HashMap *h);

  inline Bool isEmpty(const HashMap *h);
  inline uint64 getHSize(const HashMap *h);
  void printHashMap(HashMap *hm);
#endif
