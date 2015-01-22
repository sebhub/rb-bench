/*
 * Copyright (c) 2014 Sebastian Huber <sebhub@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "compat.h"
#include "rbtree-bsd.h"
#include "test.h"

typedef struct BSD_RBTree_Node {
  RB_ENTRY(BSD_RBTree_Node) Node;
} BSD_RBTree_Node;

typedef RB_HEAD(BSD_RBTree_Control, BSD_RBTree_Node) BSD_RBTree_Control;

RB_PROTOTYPE_REMOVE_COLOR(BSD_RBTree_Control, BSD_RBTree_Node, static);
RB_PROTOTYPE_REMOVE(BSD_RBTree_Control, BSD_RBTree_Node, static);
RB_PROTOTYPE_INSERT_COLOR(BSD_RBTree_Control, BSD_RBTree_Node, static);

static void _BSD_RBTree_Initialize_empty( BSD_RBTree_Control *tree )
{
  RB_INIT( tree );
}

RB_GENERATE_REMOVE_COLOR(BSD_RBTree_Control, BSD_RBTree_Node, Node, static)

RB_GENERATE_REMOVE(BSD_RBTree_Control, BSD_RBTree_Node, Node, static)

static void _BSD_RBTree_Extract( BSD_RBTree_Control *tree, BSD_RBTree_Node *node )
{
  RB_REMOVE( BSD_RBTree_Control, tree, node );
}

RB_GENERATE_INSERT_COLOR(BSD_RBTree_Control, BSD_RBTree_Node, Node, static)

static void _BSD_RBTree_Insert_color( BSD_RBTree_Control *tree, BSD_RBTree_Node *node )
{
  BSD_RBTree_Control_RB_INSERT_COLOR( tree, node );
}

static void _BSD_RBTree_Link_node( BSD_RBTree_Node *node, BSD_RBTree_Node *parent, BSD_RBTree_Node **link )
{
  RB_PARENT(node, Node) = parent;
  RB_COLOR(node, Node) = RB_RED;
  RB_LEFT(node, Node) = NULL;
  RB_RIGHT(node, Node) = NULL;
  *link = node;
}

typedef struct test_rb_bsd_node {
  test_data data;
  BSD_RBTree_Node node;
} test_rb_bsd_node;

static void test_rb_bsd_init_tree(void *tree)
{
  _BSD_RBTree_Initialize_empty( tree );
}

static void test_rb_bsd_init_node(void *node, int key)
{
  struct test_rb_bsd_node *n = node;

  n->data.key = key;
}

static test_data *test_rb_bsd_get_data(void *node)
{
  struct test_rb_bsd_node *n = node;

  return &n->data;
}

static void test_rb_bsd_insert(void *t, void *n)
{
  BSD_RBTree_Control *tree = t;
  test_rb_bsd_node *node = n;
  BSD_RBTree_Node **next = &RB_ROOT(tree);
  BSD_RBTree_Node *parent = NULL;

  while (*next != NULL) {
    const test_rb_bsd_node *c = RTEMS_CONTAINER_OF(*next, test_rb_bsd_node, node);

    parent = *next;

    if (node->data.key < c->data.key) {
      next = &RB_LEFT(*next, Node);
    } else {
      next = &RB_RIGHT(*next, Node);
    }
  }

  _BSD_RBTree_Link_node(&node->node, parent, next);
  _BSD_RBTree_Insert_color(tree, &node->node);
}

static void test_rb_bsd_extract(void *t, void *n)
{
  BSD_RBTree_Control *tree = t;
  test_rb_bsd_node *node = n;

  _BSD_RBTree_Extract( tree, &node->node );
}

void test_rbtree_bsd_for_rtems(void)
{
  BSD_RBTree_Control tree;

  test(
    "BSD for RTEMS",
    &tree,
    sizeof(test_rb_bsd_node),
    test_rb_bsd_init_tree,
    test_rb_bsd_init_node,
    test_rb_bsd_get_data,
    test_rb_bsd_insert,
    test_rb_bsd_extract
  );
}
