// Author: Emmanuel Odeke <odeke@ualberta.ca>
#ifndef _OBJ_FUNCS_H
#define _OBJ_FUNCS_H
  #include "typedefs.h"

  typedef uint64 (*hashFunc)(const void *o);

  inline uint64 doubleHashFunc(const void *data);
  uint64 pjwCharHash(const void *data);
  inline uint64 intHashFunc(const void *data);
  inline uint64 getHashSize(const void* data);

  inline uint64 getHash(const Object *elem);
  hashFunc getHashFuncByObject(const Object *o);

  inline Object *newObject(
    void *data, const TypeTag typeTag, const MemTag memTag
  );

  inline void incrementRefCount(Object *o);
  inline void decrementRefCount(Object *o);
  inline void __clearRefCount(Object *o);

  void printObject(const Object *o);
  Object *destroyObject(Object *o);

  extern void printChain(Chain *n);
  extern void printHashMap(HashMap *hm);
  inline uint64 getHSize(const HashMap *elem);
  extern Chain *destroyChain(Chain *n);
  extern HashMap *destroyHashMap(HashMap *h);

  inline Object *intObject(const uint64 u);
  inline Object *doubleObject(const double d);
  inline Object *charArrObject(char *v, const MemTag memTag);
  inline Object *kvObject(Object *key, Object *value);
  inline Object *hashMapObject(HashMap *hm, const MemTag memTag);

  inline KeyValue *kvStruct(Object *key, Object *value);
  inline KeyValue *destroyKvStruct(KeyValue *kv);
  void printKvStruct(const KeyValue *kv);

  inline void *__freeAndClearMem(void *mem);

  inline Chain *newChain();
  inline Chain *allocChain();
  Chain *prepend(Chain *n, Object *data);

  Chain *filter(Chain *it, Quantifier qFunc);

  BinaryVote *newBinaryVote();
  BinaryVote *vote(Chain *it, Quantifier qFunc);
  BinaryVote *destroyBinaryVote(BinaryVote *bv);

#endif
