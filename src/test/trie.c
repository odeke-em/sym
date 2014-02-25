#include <stdio.h>
#include <assert.h>
#include "../trie.h"
#include "../objFuncs.h"

int main() {
#ifdef RESOLVE_INDEX_TEST
  struct QueryValue {
    char query;
    int expected;
  } QueryValues[] = {
    'A', 0, '@', -1, 'a', 26, 'Z', 25, 'b', 27, 'Z', 25, '1', -1, '3', -1, '#', -1, 'c', 28
  };

  struct QueryValue *qIt = QueryValues, *qHead = qIt,
    *qEnd = qIt + sizeof(QueryValues)/sizeof(QueryValues[0]);

  while (qIt < qEnd) {
    int32 resIndex = ctoi(qIt->query);
    printf("resIndex: %d Expected: %d\n", resIndex, qIt->expected);
    assert(resIndex == qIt->expected);
    printf("qIt:: %ld %c => %d Passed\n", qIt - qHead, qIt->query, resIndex);
    ++qIt;
  } 
#endif

#ifdef SHOW_RESOLVED_INDICES
  char c;
  printf("\033[92m");
  for (c='A'; c <= 'Z'; c++) {
    printf("%c %d\n", c, resolveIndex(c));
  }

  printf("\033[95m");
  for (c='a'; c <= 'z'; c++) {
    printf("%c %d\n", c, resolveIndex(c));
  }
   
  printf("\033[94m");
  for (c='0'; c <= '9'; c++) {
    printf("%c %d\n", c, resolveIndex(c));
  }
  printf("\033[00m\n");
#endif

  Object *n10 = intObject(10);
  assert(n10 != NULL);

  FILE *ifp = fopen(__FILE__, "r");
  printf("ifp: %p\n", ifp);
  Object *sentinel = charArrObject("$\0", Stackd);
  Trie *dict  = trieFromFile(ifp, sentinel);
  dict = tput(dict, "sum\0", n10);
  Object *c10 = charArrObject("Symbioso\0", Stackd);

  const char *testKey = "sumofallfears\0";
  // Exhibiting adding of keys
  dict = tput(dict, testKey, c10);

  // Exhibiting retrieval
  Object *ret = tget(dict, testKey);
  assert(ret != NULL);
  printObject(ret);

  // Exhibiting popping 
  Object *popd = tpop(dict, testKey);
  assert(popd != NULL); 

  // The popd and previously queried values must point to the same memory
  assert(popd == ret);

  // Making sure that the testKey no longer has a value
  Object *afterPop = tget(dict, testKey);
  assert(afterPop == NULL);

  printTrie(dict);
  putchar('\n');

  Object *got = tget(dict, "ziff\0");
  printObject(got);
  putchar('\n');

  printf("dict: %p\n", dict);

  Chain *ct = __testChainPrependInFunc();
  printChain(ct);
  destroyChain(ct);

  dict = destroyTrie(dict);
  destroyObject(ret);

  fclose(ifp);

  return 0;
}
