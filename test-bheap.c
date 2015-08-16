/*
 * Copyright (c) 2015 Sebastian Huber <sebhub@gmail.com>
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
#include "bheap.h"
#include "test.h"

typedef struct test_bheap_node {
  struct bheap_node node;
  test_data data;
} test_bheap_node;

static int test_bheap_less(
  const struct bheap_node *left,
  const struct bheap_node *right
)
{
  const test_bheap_node *l = (const test_bheap_node *)left;
  const test_bheap_node *r = (const test_bheap_node *)right;

  return l->data.key < r->data.key;
}

static void test_bheap_init_tree(void *tree)
{
  bheap_init(tree);
}

static void test_bheap_init_node(void *node, int key)
{
  struct test_bheap_node *n = node;

  n->data.key = key;
}

static test_data *test_bheap_get_data(void *node)
{
  struct test_bheap_node *n = node;

  return &n->data;
}

static void test_bheap_insert(void *t, void *n)
{
  struct bheap_head *tree = t;
  test_bheap_node *node = n;

  bheap_insert(tree, &node->node, test_bheap_less);
}

static void test_bheap_extract(void *t, void *n)
{
  struct bheap_head *tree = t;
  test_bheap_node *node = n;

  bheap_extract(tree, &node->node, test_bheap_less);
}

void test_bheap(void)
{
  struct bheap_head tree;

  test(
    "bheap",
    &tree,
    sizeof(test_bheap_node),
    test_bheap_init_tree,
    test_bheap_init_node,
    test_bheap_get_data,
    test_bheap_insert,
    test_bheap_extract
  );
}
