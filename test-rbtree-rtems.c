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
#include "rbtree-rtems.h"
#include "test.h"

typedef struct {
  test_data data;
  RBTree_Node node;
} test_rtems_rbtree_node;

static RBTree_Compare_result node_cmp(
  const RBTree_Node *n1,
  const RBTree_Node *n2
)
{
  int key1 = RTEMS_CONTAINER_OF(n1, test_rtems_rbtree_node, node)->data.key;
  int key2 = RTEMS_CONTAINER_OF(n2, test_rtems_rbtree_node, node)->data.key;

  return key1 - key2;
}

static void test_rtems_rbtree_init_tree(void *tree)
{
  _RBTree_Initialize_empty(tree);
}

static void test_rtems_rbtree_init_node(void *node, int key)
{
  test_rtems_rbtree_node *n = node;

  n->data.key = key;
}

static test_data *test_rtems_rbtree_get_data(void *node)
{
  test_rtems_rbtree_node *n = node;

  return &n->data;
}

static void test_rtems_rbtree_insert(void *tree, void *node)
{
  test_rtems_rbtree_node *n = node;

  rtems_rbtree_insert(tree, &n->node, node_cmp, false);
}

static void test_rtems_rbtree_extract(void *tree, void *node)
{
  test_rtems_rbtree_node *n = node;

  rtems_rbtree_extract(tree, &n->node);
}

void test_rbtree_rtems(void)
{
  RBTree_Control tree;

  test(
    "RTEMS",
    &tree,
    sizeof(test_rtems_rbtree_node),
    test_rtems_rbtree_init_tree,
    test_rtems_rbtree_init_node,
    test_rtems_rbtree_get_data,
    test_rtems_rbtree_insert,
    test_rtems_rbtree_extract
  );
}
