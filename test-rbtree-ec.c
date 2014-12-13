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
#include "rbtree-ec.h"

static void test_ec_rbtree_init_tree(void *tree)
{
  jsw_rbtree_t *t = tree;

  t->root = NULL;
}

static void test_ec_rbtree_init_node(void *node, int key)
{
  jsw_rbnode_t *n = node;

  n->data.key = key;
}

static test_data *test_ec_rbtree_get_data(void *node)
{
  jsw_rbnode_t *n = node;

  return &n->data;
}

static void test_ec_rbtree_insert(void *tree, void *node)
{
  jsw_rbnode_t *n = node;

  jsw_rbinsert(tree, n);
}

static void test_ec_rbtree_extract(void *tree, void *node)
{
  jsw_rbnode_t *n = node;

  jsw_rberase(tree, n);
}

void test_rbtree_ec(void)
{
  jsw_rbtree_t tree;

  test(
    "Eternally Confuzzled",
    &tree,
    sizeof(jsw_rbnode_t),
    test_ec_rbtree_init_tree,
    test_ec_rbtree_init_node,
    test_ec_rbtree_get_data,
    test_ec_rbtree_insert,
    test_ec_rbtree_extract
  );
}
