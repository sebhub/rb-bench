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
#include "rtems-chainimpl.h"
#include "test.h"

typedef struct test_chain_node {
  Chain_Node node;
  test_data data;
} test_chain_node;

static bool order(const Chain_Node *left_base, const Chain_Node *right_base)
{
  test_chain_node *left = (test_chain_node *) left_base;
  test_chain_node *right = (test_chain_node *) right_base;

  return left->data.key < right->data.key;
}

static void test_chain_init_tree(void *chain)
{
  _Chain_Initialize_empty( chain );
}

static void test_chain_init_node(void *node, int key)
{
  struct test_chain_node *n = node;

  n->data.key = key;
}

static test_data *test_chain_get_data(void *node)
{
  struct test_chain_node *n = node;

  return &n->data;
}

static void test_chain_insert(void *t, void *n)
{
  Chain_Control *chain = t;
  test_chain_node *node = n;

  _Chain_Insert_ordered_unprotected(chain, &node->node, order);
}

static void test_chain_extract(void *t, void *n)
{
  test_chain_node *node = n;

  (void) t;

  _Chain_Extract_unprotected(&node->node);
}

void test_rbtree_chain(void)
{
  Chain_Control chain;

  test(
    "Chain",
    &chain,
    sizeof(test_chain_node),
    test_chain_init_tree,
    test_chain_init_node,
    test_chain_get_data,
    test_chain_insert,
    test_chain_extract
  );
}
