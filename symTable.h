// Author: Emmanuel Odeke <odeke@ualberta.ca>
#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H
  #include "hashlist/hashList.h"

  // Constants here
  const unsigned int SYM_HASHLIST_SIZE = 10001;

  typedef void *(*allocator)(void);
  typedef void *(*deAllocator)(void *);
  typedef void *(*initializer)(void *);

  typedef struct {
    void *data;
    allocator memGen;
    deAllocator memDel;
    unsigned int isHeapd:1;
  } Symbol;

  typedef struct STable_ {
    unsigned int size;
    unsigned int nextAvailIndex;
    Symbol **symbolList;
  } STable;

  STable *createSTable(unsigned int size);
  STable *freeSTable(STable *s);

  int newSymbolEntry(STable *s);
  Symbol *freeSymbol(Symbol *s);

  Symbol *createSymbol(allocator allocFunc, deAllocator deAllocFunc);
  inline unsigned int isEmpty(const STable *s);
  inline Symbol *getSymbolByIndex(STable *s, const int key);
  inline int keyToSymAddrSpace(const STable *s, const int key);
#endif
