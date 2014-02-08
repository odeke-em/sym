// Author: Emmanuel Odeke <odeke@ualberta.ca>
#ifndef _TYPEDEFS_H
#define _TYPEDEFS_H
  #define raiseError(msg){\
    fprintf(stderr, "%s::%s \033[91m%s on line %d\n\033[00m",\
      __FILE__, __func__, #msg, __LINE__\
    );\
    exit(-1);\
  }

  typedef int int32;
  typedef long int int64;
  typedef unsigned int uint32;
  typedef unsigned long int uint64;

  typedef enum {
    False=0, True=1
  } Bool;

  typedef enum {
    IntTag, LIntTag, CharArrayTag, ObjTag,
    Undefined, KeyValueTag, DoubleTag, PadTag
  } TypeTag;

  typedef enum {
    Heapd, Stackd
  } MemTag;

  typedef struct {
    void *data;
    Bool isFreed;
    MemTag memTag;
    uint32 refCount;
    TypeTag typeTag;
  } Object;

  typedef struct {
    Object *key, *value;
  } KeyValue;

  typedef Bool (*Quantifier)(const Object *o); 
#endif