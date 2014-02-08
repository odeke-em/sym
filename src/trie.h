// Author: Emmanuel Odeke <odeke@ualberta.ca>
#ifndef _TRIE_H
#define _TRIE_H
  #include <stdio.h>
  #include "typedefs.h"
  
  typedef struct Trie_ {
    Object *value;
    Bool EOS; //End-Of-Sequence
    struct Trie_ **nodes;
  } Trie;

  inline Trie *allocTrie();
  Trie *createTrie();
  Trie *destroyTrie(Trie *t);
  
  // Keep following allowing and only deposit the load once at
  // the sequence's end ie once we've hit a '\0' 
  Trie *tput(Trie *tr, const char *seq, Object *load);

  Object *tget(Trie *tr, const char *seq);

  void exploreTrie(Trie *t, const char *pAxiom);

  Trie *trieFromFile(FILE *ifp);
  int ctoi(const char c);
  char itoc(const int i);
  void printTrie(Trie *t);
#endif
