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
#include "test.h"
#include <boost/intrusive/rbtree.hpp>
#include <new>

using namespace boost::intrusive;

struct test_rb_boost_node {
  test_data data;
  set_member_hook<optimize_size<true> > node;

  friend bool operator<(const test_rb_boost_node &a, const test_rb_boost_node &b)
  {
    return a.data.key < b.data.key;
  }
};

typedef member_hook<test_rb_boost_node, set_member_hook<optimize_size<true> >, &test_rb_boost_node::node> test_rb_boost_member;

typedef rbtree<test_rb_boost_node, constant_time_size<false>, test_rb_boost_member> test_rb_boost_tree;

static void test_rb_boost_init_tree(void *tree)
{
	new (tree) test_rb_boost_tree();
}

static void test_rb_boost_init_node(void *node, int key)
{
	test_rb_boost_node *n = new (node) test_rb_boost_node();

        n->data.key = key;
}

static test_data *test_rb_boost_get_data(void *node)
{
	test_rb_boost_node *n = static_cast<test_rb_boost_node *>(node);

        return &n->data;
}

static void test_rb_boost_insert(void *tree, void *node)
{
	test_rb_boost_tree *t = static_cast<test_rb_boost_tree *>(tree);
	test_rb_boost_node *n = static_cast<test_rb_boost_node *>(node);

	t->insert_equal(*n);
}

static void test_rb_boost_extract(void *tree, void *node)
{
	test_rb_boost_tree *t = static_cast<test_rb_boost_tree *>(tree);
	test_rb_boost_node *n = static_cast<test_rb_boost_node *>(node);

	t->erase(*n);
}

void test_rbtree_boost(void)
{
  test_rb_boost_tree tree;

  test(
    "Boost",
    &tree,
    sizeof(test_rb_boost_node),
    test_rb_boost_init_tree,
    test_rb_boost_init_node,
    test_rb_boost_get_data,
    test_rb_boost_insert,
    test_rb_boost_extract
  );
}
