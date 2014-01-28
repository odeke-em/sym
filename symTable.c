// Author: Emmanuel Odeke <odeke@ualberta.ca>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  printf("addrKeyByKey : %d\n", addrKey);
  return addrKey < 0 ? NULL : *(s->symbolList + addrKey);
}

STable *createSTable(unsigned int size) {
  STable *stringIndexT = (STable *)malloc(sizeof(STable));
  if (stringIndexT == NULL) {
    raiseError("No memory left to create symbol table");
  }

  if ((stringIndexT->symbolList = (Symbol **)malloc(sizeof(Symbol *) * size)) == NULL) {
    raiseError("Cannot allocate memory for the symbol table list");
  }

  stringIndexT->size = size;
  stringIndexT->nextAvailIndex = 0;

  Symbol **sIt = stringIndexT->symbolList, 
         **sEnd = sIt + stringIndexT->size;

  while (sIt < sEnd) {
    *sIt++ = NULL;
  }

  return stringIndexT;
}

// Protocol => Each entry in the symbol table is a hashlist
int newSymbolEntry(STable *s) {
  if (s != NULL) {
    if (s->symbolList == NULL) {
      raiseError("Symbol Table List is null");
    } else {
      unsigned int key = s->nextAvailIndex;
      while (*(s->symbolList + key) != NULL && key++ < s->size) {
        // TODO Figure out a policy for vacating already registered symbols;
        // raiseWarning("Was not expecting symbol at position: %d", key++);
      }

      if (key >= s->size) {
        raiseError("No space left to insert new symbol");
      } else {
        *(s->symbolList + key) = createSymbol(
          (void *)createHashList, (void *)destroyHashList
        );
        (*(s->symbolList + key))->data = createHashList(SYM_HASHLIST_SIZE);
        s->nextAvailIndex = key;
        return key;
      }
    }
  } else {
    raiseWarning("Symbol Table is null", "");
    return -1;
  }
}

int addSymbol(STable *s, const unsigned int symIndex, void *data, const unsigned int hash) {
  if (s != NULL) {
    Symbol *symAtI = getSymbolByIndex(s, symIndex);
    if (symAtI == NULL) {
      raiseError("Symbol entry not yet defined");
    } else {
      insertElem(symAtI->data, data, hash);
      return 0;
    }
  } else {
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
  char *strings[] = {
    "flux", "increase", "chainz"
  };

  STable *sT = createSTable(100);
  int intIndex = newSymbolEntry(sT);
  int stringIndex = newSymbolEntry(sT);
  int doubleIndex = newSymbolEntry(sT);

  Symbol *f1 = getSymbolByIndex(sT, 1);
  HashList *hl = (HashList *)(f1 == NULL ? NULL : f1->data);
  int i;
  for (i=0; i < sizeof(strings)/sizeof(strings[0]); ++i) {
    printf("insertion : %d\n", 
      addSymbol(sT, 0, strdup(strings[i]), i)
    );
  }

  printf("hl: %p hSz: %d\n", hl, hl->size);
  printf("intIndex %d\n", intIndex);
  printf("ns %d\n", stringIndex);
  printf("ns %d\n", doubleIndex);
  sT = freeSTable(sT);
  return 0;
}
