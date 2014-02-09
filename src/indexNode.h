// Author: Emmanuel Odeke <odeke@ualberta.ca>
#ifndef _INDEX_NODE_H
#define _INDEX_NODE_H
  typedef struct IndexNode_ {
    int index;
    struct IndexNode_ *next;
  } IndexNode;
#endif
