// Author: Emmanuel Odeke <odeke@ualberta.ca>
#include <stdio.h>
#include <stdlib.h>

#include "symTable.h"
#include "hashlist/errors.h"

inline unsigned int isEmpty(const STable *s) {
  return (s == NULL || s->symbolList == NULL || s->size == 0 ? 1 : 0);
}

inline int keyToSymAddrSpace(const STable *s, const int key) {
  return isEmpty(s) || key < 0 ? -1 : key % s->size;
}

inline Symbol *getSymbolByIndex(STable *s, const int key) {
  const int addrKey = keyToSymAddrSpace(s, key);
  return addrKey < 0 ? NULL : *(s->symbolList + addrKey);
}

STable *createSTable(unsigned int size) {
  STable *nST = (STable *)malloc(sizeof(STable));
  if (nST == NULL) {
    raiseError("No memory left to create symbol table");
  }

  if ((nST->symbolList = (Symbol **)malloc(sizeof(Symbol *) * size)) == NULL) {
    raiseError("Cannot allocate memory for the symbol table list");
  }

  nST->size = size;
  nST->nextAvailIndex = 0;

  Symbol **sIt = nST->symbolList, 
         **sEnd = sIt + nST->size;

  while (sIt < sEnd) {
    *sIt++ = NULL;
  }

  return nST;
}

// Protocol => Each entry in the symbol table is a hashlist
int newSymbolEntry(STable *s) {
  if (s != NULL) {
    if (s->symbolList == NULL) {
      raiseError("Symbol Table List is null");
    } else {
      unsigned int key = s->nextAvailIndex;
      while (*(s->symbolList + key) && key++ < s->size) {
        // TODO Figure out a policy for vacating already registered symbols;
        raiseWarning("Was not expecting symbol at position: %d", s->nextAvailIndex);
      }

      if (key >= s->size) {
        raiseError("No space left to insert new symbol");
      } else {
        *(s->symbolList + key) = createSymbol(
          (void *)createHashList, (void *)destroyHashList
        );
        (*(s->symbolList + key))->data = createHashList(SYM_HASHLIST_SIZE);
        ++s->nextAvailIndex;
        return key;
      }
    }
  } else {
    raiseWarning("Symbol Table is null", "");
    return -1;
  }
}

Symbol *createSymbol(allocator allocFunc, deAllocator deAllocFunc) {
  Symbol *freshSym = (Symbol *)malloc(sizeof(Symbol));
  if (freshSym == NULL) {
    raiseError("No memory for a new Symbol");
  }

  freshSym->data = NULL;
  freshSym->isHeapd = 1;
  freshSym->memGen = allocFunc;
  freshSym->memDel = deAllocFunc;

  return freshSym;
}

Symbol *freeSymbol(Symbol *s) {
  if (s != NULL) {
    if (s->memDel != NULL) {
      s->data = s->memDel(s->data);
    } else {
      raiseWarning("Memory deallocator is NULL", "");
    }
    free(s);
  }

  return s;
}

STable *freeSTable(STable *s) {
  if (s != NULL) {
    if (s->symbolList != NULL) {
      Symbol **sIt = s->symbolList, 
            **sEnd = sIt + s->size;
      while (sIt < sEnd) {
        if (*sIt != NULL) {
          *sIt = freeSymbol(*sIt);
        }
        ++sIt;
      }

      free(s->symbolList);
    }

    free(s);
  }

  return s;
}

int main(int argc, char *argv[]) {
  STable *sT = createSTable(100);
  int nQ = newSymbolEntry(sT);
  int nS = newSymbolEntry(sT);
  int nT = newSymbolEntry(sT);

  Symbol *f1 = getSymbolByIndex(sT, 1);
  HashList *hl = (HashList *)(f1 == NULL ? NULL : f1->data);
  printf("hl: %p hSz: %d\n", hl, hl->size);
  printf("nq %d\n", nQ);
  printf("ns %d\n", nS);
  printf("ns %d\n", nT);
  sT = freeSTable(sT);
  return 0;
}
