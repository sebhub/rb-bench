#ifndef JSW_RBTREE_H
#define JSW_RBTREE_H

/*
  Red Black balanced tree library

    > Created (Julienne Walker): August 23, 2003
    > Modified (Julienne Walker): March 14, 2008

  This code is in the public domain. Anyone may
  use it or change it in any way that they see
  fit. The author assumes no responsibility for 
  damages incurred through use of the original
  code or any variations thereof.

  It is requested, but not required, that due
  credit is given to the original author and
  anyone who has modified the code through
  a header comment, such as this one.
*/
#ifdef __cplusplus
#include <cstddef>

using std::size_t;

extern "C" {
#else
#include <stddef.h>
#endif

#include "test.h"

/* Opaque types */
typedef struct jsw_rbtree jsw_rbtree_t;
typedef struct jsw_rbtrav jsw_rbtrav_t;

typedef struct jsw_rbnode {
  int                red;     /* Color (1=red, 0=black) */
  struct jsw_rbnode *link[2]; /* Left (0) and right (1) links */
  test_data          data;
} jsw_rbnode_t;

struct jsw_rbtree {
  jsw_rbnode_t *root; /* Top of the tree */
};

/* Red Black tree functions */
void         *jsw_rbfind ( jsw_rbtree_t *tree, jsw_rbnode_t *node );
int           jsw_rbinsert ( jsw_rbtree_t *tree, jsw_rbnode_t *node );
int           jsw_rberase ( jsw_rbtree_t *tree, jsw_rbnode_t *node );
size_t        jsw_rbsize ( jsw_rbtree_t *tree );

/* Traversal functions */
jsw_rbtrav_t *jsw_rbtnew ( void );
void          jsw_rbtdelete ( jsw_rbtrav_t *trav );
void         *jsw_rbtfirst ( jsw_rbtrav_t *trav, jsw_rbtree_t *tree );
void         *jsw_rbtlast ( jsw_rbtrav_t *trav, jsw_rbtree_t *tree );
void         *jsw_rbtnext ( jsw_rbtrav_t *trav );
void         *jsw_rbtprev ( jsw_rbtrav_t *trav );

#ifdef __cplusplus
}
#endif

#endif
