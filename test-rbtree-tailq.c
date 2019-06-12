/*
 * Copyright (C) 2019 Sebastian Huber <sebhub@gmail.com>
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
#include <sys/queue.h>
#include "test.h"

typedef struct test_tailq_node {
  TAILQ_ENTRY(test_tailq_node) node;
  test_data data;
} test_tailq_node;

TAILQ_HEAD(test_tailq_head, test_tailq_node);

typedef struct test_tailq_head test_tailq_head;

static void test_tailq_init_tree(void *t)
{
  test_tailq_head *head = t;

  TAILQ_INIT(head);
}

static void test_tailq_init_node(void *node, int key)
{
  test_tailq_node *n = node;

  n->data.key = key;
}

static test_data *test_tailq_get_data(void *node)
{
  test_tailq_node *n = node;

  return &n->data;
}

static void test_tailq_insert(void *t, void *i)
{
  test_tailq_head *head = t;
  test_tailq_node *insert = i;
  test_tailq_node *node;

  TAILQ_FOREACH(node, head, node) {
    if (insert->data.key < node->data.key) {
      TAILQ_INSERT_BEFORE(node, insert, node);
      return;
    }
  }

  TAILQ_INSERT_TAIL(head, insert, node);
}

static void test_tailq_extract(void *t, void *n)
{
  test_tailq_head *head = t;
  test_tailq_node *node = n;

  TAILQ_REMOVE(head, node, node);
}

void test_rbtree_tailq(void)
{
  test_tailq_head head;

  test(
    "TAILQ",
    &head,
    sizeof(test_tailq_node),
    test_tailq_init_tree,
    test_tailq_init_node,
    test_tailq_get_data,
    test_tailq_insert,
    test_tailq_extract
  );
}
