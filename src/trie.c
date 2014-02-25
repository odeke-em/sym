// Author: Emmanuel Odeke <odeke@ualberta.ca>
// Implementation of a trie dictionary

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trie.h"
#include "objFuncs.h"
#define ALPHA_START 'a'
#define ALPHA_END   'z'
#define ALPHA_DIFF  ((ALPHA_END - ALPHA_START) + 1)
#define ALPHA_SIZE (ALPHA_DIFF * 2) // Remembering that 'Z' - 'A' is 25 yet there are 26 characters
                                    //  since 'A' -> base is at index 0, 'Z' the last is at 25

inline Trie *allocTrie() { 
  return (Trie *)malloc(sizeof(Trie)); 
}

char itoc(const int i) {
  char outC = EOF;
  if (i >=0 && i < ALPHA_SIZE) {
    if (i < ALPHA_DIFF) {
      outC = i + 'A';
    } else {
      outC = (i - ALPHA_DIFF) + 'a';
    }
  }
  return outC;
}

int ctoi(const char c) {
  int outV = -1;
  if (isalpha(c)) {
    if (isupper(c)) {
      outV = c - 'A';
    } else {
      outV = ALPHA_DIFF + (c - 'a');
    }
  }
  return outV;
}

Trie *createTrie() {
  Trie *freshTrie = allocTrie();
  if (freshTrie == NULL) {
    raiseError("Run-out of memory");
  }

  freshTrie->EOS = False;
  freshTrie->value = NULL;
  freshTrie->nodes = NULL;

  return freshTrie;
}

Trie *trieFromFile(FILE *ifp, Object *sentinel) {
  Trie *fTrie = NULL;
  if (ifp != NULL) {
    fTrie = createTrie();
    int MAX_SZ = 90;
    while (! feof(ifp)) {
      int bufSz = 10, index = 0;
      char c, *wIn = (char *)malloc(sizeof(char) * bufSz);
      while ((c = getc(ifp)) && isalpha(c) && index < MAX_SZ) {
        if (index >= bufSz) {
          bufSz += 10; // Arbitrary increase of 10
          wIn = (char *)realloc(wIn, sizeof(char) * bufSz);
        }
    
        *(wIn + index++) = c;
      }

      if (index) {
        *(wIn + index++) = '\0';
        wIn = (char *)realloc(wIn, sizeof(char) * index);
        fTrie = tput(fTrie, wIn, sentinel);
      }
      free(wIn);
    }
  }

  return fTrie;
}

Trie *destroyTrie(Trie *tr) {
#ifdef DEBUG
  printf("In %s\n", __func__);
#endif
  if (tr != NULL) {
    if (tr->nodes != NULL) {
      Trie **it = tr->nodes;
      Trie **end = it + ALPHA_SIZE;
      while (it < end) {
        if (*it != NULL) {
            *it = destroyTrie(*it);
        }
        ++it;
      }

      free(tr->nodes);
      tr->nodes = NULL;
    }

    tr->value = destroyObject(tr->value);

    free(tr);
    tr = NULL;
  }

  return tr;
}

void printTrie(Trie *t) {
  printf("{ ");
  exploreTrie(t, "\0");
  putchar('}');
}

void exploreTrie(Trie *t, const char *pAxiom) {
  exploreAndMapTrie(t, pAxiom, NULL);
}

void exploreAndMapTrie(Trie *t, const char *pAxiom,
  void (*func)(const char *, Object *)
) {
  if (t != NULL) {
    if (t->nodes != NULL) {
      ssize_t pAxiomLen = strlen(pAxiom);
      // space for 1 extra char and a '\0'
      char *ownAxiom = (char *)malloc(pAxiomLen + 2); 
      if (ownAxiom == NULL)
        raiseError("Run out of memory");

      memcpy(ownAxiom, pAxiom, pAxiomLen);
      *(ownAxiom + pAxiomLen + 1) = '\0';

      Trie **it = t->nodes, 
           **end = it + ALPHA_SIZE, **start = it; 
      while (it < end) {
        if (*it != NULL) {
          *(ownAxiom + pAxiomLen) = itoc(it - start);

          if ((*it)->EOS == True) {
            printf("%s:", ownAxiom);
            printObject((*it)->value);
            putchar(' ');
            if (func != NULL) {
              func(ownAxiom, (*it)->value);
            }
          }

          exploreAndMapTrie(*it, ownAxiom, func);
        }

        ++it;

      }

      free(ownAxiom);
    }
  }
}

Trie *tput(Trie *tr, const char *key, Object *value) {
#ifdef DEBUG
  printf("%s seq: %s\n", __func__, key);
#endif
  if (key != NULL) {
    if (tr == NULL) {
      tr = createTrie();
    }

    if (*key != '\0') {
      int targetIndex = ctoi(*key);
      if (targetIndex >= 0 && targetIndex < ALPHA_SIZE) {
        if (tr->nodes == NULL) {
          tr->nodes = (Trie **)malloc(sizeof(Trie *) * ALPHA_SIZE);
          Trie **it = tr->nodes, **end = it + ALPHA_SIZE;
          while (it < end) {
            *it++ = NULL;
          }
        }

        *(tr->nodes + targetIndex) =\
            tput(*(tr->nodes + targetIndex), key + 1, value);
      }
    } else { // End of this sequence, time to add the value
      tr->EOS = True;
      tr->value = value;
    }
  }

  return tr;
}

Object *__tQueryOrPop(Trie *tr, const char *seq, Bool isQuery) {
  if (seq == NULL || tr == NULL) {
    return NULL;
  } else if (*seq == '\0') {
    Object *retObject = tr->value;
    if (isQuery == False) { // Therefore requesting for a pop
       tr->value = NULL;
       tr->EOS = False; // No longer visible as a key
    }
    return retObject;
  } else {
    int resIndex = ctoi(*seq);
    if (resIndex < 0 || resIndex >= ALPHA_SIZE) {
      return NULL;
    } else if (*(tr->nodes + resIndex) == NULL) { 
      return NULL;
    } else {
      return __tQueryOrPop(*(tr->nodes + resIndex), seq + 1, isQuery);
    }
  }
}

Object *tget(Trie *tr, const char *key) {
  return __tQueryOrPop(tr, key, True);
}

Object *tpop(Trie *tr, const char *key) {
  return __tQueryOrPop(tr, key, False);
}
