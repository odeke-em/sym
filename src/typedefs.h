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

#ifndef _BOOL_D
#define _BOOL_D
  typedef enum {
    False=0, True=1
  } Bool;
#endif

  typedef enum {
    IntTag, LIntTag, CharArrayTag, ObjTag,
    Undefined, KeyValueTag, DoubleTag, PadTag, HashMapTag
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

  typedef struct Chain_ {
    Object *value;
    struct Chain_ *next;
  } Chain;

  typedef struct {
    uint32 size;
    Chain **list;
    Bool canCollide;
  } HashMap;
  
  typedef Bool (*Quantifier)(const Object *o); 
  typedef void (*KeyPairOp)(const char *key, Object *o);
#endif
