// Author: Emmanuel Odeke <odeke@ualberta.ca>
#ifndef _OBJ_FUNCS_H
#define _OBJ_FUNCS_H
  #include "typedefs.h"

  typedef uint64 (*hashFunc)(const void *o);

  inline uint64 doubleHashFunc(const void *data);
  uint64 pjwCharHash(const void *data);
  inline uint64 intHashFunc(const void *data);
  hashFunc getHashFuncByObject(const Object *o);

  inline Object *newObject(
    void *data, const TypeTag typeTag, const MemTag memTag
  );

  void printObject(const Object *o);
  void destroyObject(Object *o);

  inline Object *intObject(const uint64 u);
  inline Object *doubleObject(const double d);
  inline Object *charArrObject(char *v, const MemTag memTag);
  inline Object *kvObject(Object *key, Object *value);
  inline uint64 getHash(const Object *elem);

  inline KeyValue *kvStruct(Object *key, Object *value);
  inline void destroyKvStruct(KeyValue *kv);
  void printKvStruct(const KeyValue *kv);
#endif
