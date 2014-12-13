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

typedef struct test_rb_bsd_node {
  test_data data;
  RB_ENTRY(test_rb_bsd_node) node;
} test_rb_bsd_node;

RB_HEAD(test_rb_bsd_tree, test_rb_bsd_node);

static int node_cmp(
  const test_rb_bsd_node *n1,
  const test_rb_bsd_node *n2
)
{
  return n2->data.key - n1->data.key;
}

RB_GENERATE_STATIC(test_rb_bsd_tree, test_rb_bsd_node, node, node_cmp);

static void test_rb_bsd_init_tree(void *tree)
{
  struct test_rb_bsd_tree *t = tree;
  RB_INIT(t);
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

static void test_rb_bsd_insert(void *tree, void *node)
{
  RB_INSERT(test_rb_bsd_tree, tree, node);
}

static void test_rb_bsd_extract(void *tree, void *node)
{
  RB_REMOVE(test_rb_bsd_tree, tree, node);
}

void test_rbtree_bsd(void)
{
  struct test_rb_bsd_tree tree;

  test(
    "BSD",
    &tree,
    sizeof(test_rb_bsd_node),
    test_rb_bsd_init_tree,
    test_rb_bsd_init_node,
    test_rb_bsd_get_data,
    test_rb_bsd_insert,
    test_rb_bsd_extract
  );
}
