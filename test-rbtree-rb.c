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

#define NDEBUG
#define RB_COMPACT

#include "compat.h"
#include "rbtree-rb.h"
#include "test.h"

typedef struct test_rb_node test_rb_node;

struct test_rb_node {
  test_data data;
  rb_node(test_rb_node) node;
};

typedef rbt(test_rb_node) test_rb_tree;

static int node_cmp(
  const test_rb_node *n1,
  const test_rb_node *n2
)
{
  return n2->data.key - n1->data.key;
}

rb_gen(static inline, test_rb_, test_rb_tree, test_rb_node, node, node_cmp);

static void test_rb_rb_init_tree(void *tree)
{
  test_rb_tree *t = tree;

  test_rb_new(t);
}

static void test_rb_rb_init_node(void *node, int key)
{
  struct test_rb_node *n = node;

  n->data.key = key;
}

static test_data *test_rb_rb_get_data(void *node)
{
  struct test_rb_node *n = node;

  return &n->data;
}

static void test_rb_rb_insert(void *tree, void *node)
{
  test_rb_insert(tree, node);
}

static void test_rb_rb_extract(void *tree, void *node)
{
  test_rb_remove(tree, node);
}

void test_rbtree_rb(void)
{
  test_rb_tree tree;

  test(
    "RB Compact",
    &tree,
    sizeof(test_rb_node),
    test_rb_rb_init_tree,
    test_rb_rb_init_node,
    test_rb_rb_get_data,
    test_rb_rb_insert,
    test_rb_rb_extract
  );
}
