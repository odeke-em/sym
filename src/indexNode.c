// Author: Emmanuel Odeke <odeke@ualberta.ca>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "indexNode.h"
#define ALPHA_SIZE 26

void printIndexNode(IndexNode **r) {
  int i=0;
  for (i=0; i < ALPHA_SIZE; ++i) {
    printf("%c => [", i + 'a');
    IndexNode *t = *(r + i);
    while (t != NULL) {
      printf("%d", t->index);
      if (t->next != NULL) printf(", ");
      t = t->next;
    }
    printf("]\n");
  }
}

int getRank(const char *subject, const char *base) {
  int rank = -1;
  if (subject != NULL && base != NULL) {
    IndexNode *r[ALPHA_SIZE];
    int subjectLen = strlen(subject)/1;
    int i;
    for (i=0; i < ALPHA_SIZE; ++i) {
      *(r + i) = NULL;
    }

    // For later ascending order traversal, add indices from high to low
    i = subjectLen;  
    while (--i >= 0) {
      if (isalpha(*(subject + i))) {
        int index = tolower(*(subject + i)) - 'a';
        IndexNode *t = (IndexNode *)malloc(sizeof(IndexNode));
        t->index = i;
        t->next  = *(r + index);
        *(r + index) = t;
      }
    }

    int reuses=0, moves=0, inplace=0, deletions=0, additions=0;

    printIndexNode(r);
    int baseLen = strlen(base)/1;
    for (i =0; i < baseLen; ++i) {
      if (isalpha(*(base + i))) {
        int index = tolower(*(base + i)) - 'a';
        // printf("i: %d index: %d rIndex: %p %c\n", i, index, *(r + index), *(base + i));
        if (*(r + index) == NULL) {
          ++deletions;
        } else {
          ++reuses;
          if ((*(r + index))->index == i) {
            ++inplace;
          } else {
            ++moves;
          }
          // Time to pop it off
          IndexNode *tmp = (*(r + index))->next;
          free(*(r + index));
          *(r + index) = tmp;
        }
      }
    }


    // Cleaning up
    IndexNode **tIt = r, **tEnd = tIt + ALPHA_SIZE, *tmp;
    while (tIt < tEnd) {
      while (*tIt != NULL) {
        tmp = (*tIt)->next;
        free(*tIt);
        *tIt = tmp;
        ++additions;
      }
      ++tIt;
    }

    printf("Add: %d Move: %d Delete: %d Keep: %d\n", 
      additions, moves, deletions, inplace
    );
    rank = (inplace * 3) + (moves * 2) + ((deletions + additions) * -1);
  }

  return rank;
}

int main() {
  printf("%d\n", getRank("https://www.youtube.com/watch?v=CfihYWRWRTQ\0", "https://github.com\0"));
  return 0;
}
