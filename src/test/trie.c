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
    'a', 0, '@', -1, 'A', 'z', 25, 0, 'b', 1, 'Z', 25, '1', -1, '3', -1, '#', -1
  };

  struct QueryValue *qIt = QueryValues, *qHead = qIt,
    *qEnd = qIt + sizeof(QueryValues)/sizeof(QueryValues[0]);

  while (qIt < qEnd) {
    int32 resIndex = resolveIndex(qIt->query);
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
  Trie *dict  = trieFromFile(ifp);
  dict = tput(dict, "sum\0", n10);
  Object *c10 = charArrObject("Symbioso\0", Stackd);

  // Exhibiting adding of spaces
  dict = tput(dict, "sumofall\0", c10);

  Object *ret = tget(dict, "sumofall\0");
  assert(ret != NULL);
  printObject(ret);

  printTrie(dict);
  putchar('\n');

  dict = destroyTrie(dict);
  destroyObject(ret);

  fclose(ifp);

  return 0;
}
