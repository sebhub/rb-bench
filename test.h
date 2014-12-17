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

#ifndef TEST_H
#define TEST_H

#define __STDC_FORMAT_MACROS

#include <assert.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef uint64_t ticks;

#ifdef __rtems__

#include <rtems.h>
#include <rtems/bspIo.h>

#define ticks_read() rtems_clock_get_uptime_nanoseconds()

#else /* __rtems__ */

#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>

#define printk printf

static inline uintptr_t ticks_read(void)
{
	struct rusage r;

	getrusage(RUSAGE_SELF, &r);

	return r.ru_utime.tv_sec * 1000000000ULL + r.ru_utime.tv_usec * 1000ULL;
}

#endif /* __rtems__ */

#define ticks_difference(t1, t0) ((t1) - (t0))

#define ticks_to_nanoseconds(t) (t)

typedef struct {
  int key;
  bool is_member;
} test_data;

typedef void (*tree_init_tree)(void *tree);

typedef void (*tree_init_node)(void *node, int key);

typedef test_data *(*tree_get_data)(void *node);

typedef void (*tree_insert)(void *tree, void *node);

typedef void (*tree_extract)(void *tree, void *node);

static inline uint32_t simple_random( uint32_t v )
{
  v *= 1664525;
  v += 1013904223;

  return v;
}

static inline void *get_node(void *nodes, size_t node_size, size_t i)
{
  return (void *) ((char *) nodes + i * node_size);
}

static inline void *next_node(void *node, size_t node_size)
{
  return get_node(node, node_size, 1);
}

static inline void *create_nodes(
  size_t node_count,
  size_t node_size,
  tree_init_node init_node
)
{
  void *nodes;
  void *node;
  size_t i;

  nodes = calloc(node_count, node_size);
  assert(nodes != NULL);

  node = nodes;
  for (i = 0; i < node_count; ++i) {
    (*init_node)(node, (int) i);
    node = next_node(node, node_size);
  }

  return nodes;
}

static inline void test_random_ops(
  void *tree,
  size_t node_count,
  size_t node_size,
  tree_init_tree init_tree,
  tree_init_node init_node,
  tree_get_data get_data,
  tree_insert insert,
  tree_extract extract
)
{
  uint32_t v = 0xdeadbeef;
  size_t m = 123 * node_count;
  unsigned long insert_count = 0;
  unsigned long extract_count = 0;
  int shift = 8;
  void *nodes;
  size_t i;
  ticks t0;
  ticks t1;
  ticks d;

  assert(node_count < (1UL << (32 - shift)));

  (*init_tree)(tree);
  nodes = create_nodes(node_count, node_size, init_node);

  t0 = ticks_read();

  for (i = 0; i < m; ++i) {
    size_t j = (v >> shift) % node_count;
    void *node = get_node(nodes, node_size, j);
    test_data *data = (*get_data)(node);

    if (data->is_member) {
      data->is_member = false;
      ++extract_count;
      (*extract)(tree, node);
    } else {
      data->is_member = true;
      ++insert_count;
      (*insert)(tree, node);
    }

    v = simple_random(v);
  }

  t1 = ticks_read();
  d = ticks_difference(t1, t0);

  printk(
    "\t\t\t<Sample nodeCount=\"%lu\" "
      "insertCount=\"%lu\" "
      "extractCount=\"%lu\" "
      "duration=\"%" PRIu64 "\"/>\n",
    (unsigned long) node_count,
    insert_count,
    extract_count,
    ticks_to_nanoseconds(d)
  );

  free(nodes);
}

static inline void test_linear(
  void *tree,
  size_t node_count,
  size_t node_size,
  tree_init_tree init_tree,
  tree_init_node init_node,
  tree_insert insert,
  tree_extract extract
)
{
  size_t m = 10000;
  void *nodes;
  size_t i;
  size_t j;
  ticks t0;
  ticks t1;
  ticks d;

  (*init_tree)(tree);
  nodes = create_nodes(node_count, node_size, init_node);

  t0 = ticks_read();

  for (i = 0; i < m; ++i) {
    void *node;

    node = nodes;
    for (j = 0; j < node_count; ++j) {
      (*insert)(tree, node);
      node = next_node(node, node_size);
    }

    node = nodes;
    for (j = 0; j < node_count; ++j) {
      (*extract)(tree, node);
      node = next_node(node, node_size);
    }
  }

  t1 = ticks_read();
  d = ticks_difference(t1, t0);

  printk(
    "\t\t\t<Sample nodeCount=\"%lu\" "
      "insertCount=\"%lu\" "
      "extractCount=\"%lu\" "
      "duration=\"%" PRIu64 "\"/>\n",
    (unsigned long) node_count,
    (unsigned long) m,
    (unsigned long) m,
    ticks_to_nanoseconds(d)
  );

  free(nodes);
}

static inline size_t large_set_next(size_t c)
{
  return (123 * c + 99) / 100;
}

static inline void test(
  const char *impl,
  void *tree,
  size_t node_size,
  tree_init_tree init_tree,
  tree_init_node init_node,
  tree_get_data get_data,
  tree_insert insert,
  tree_extract extract
)
{
  size_t small_set_size = 128;
  size_t large_set_size = 1;
  size_t c;
  size_t i;

  printk(
    "\t<RBTest implementation=\"%s\" nodeSize=\"%lu\">\n",
    impl,
    (unsigned long) (node_size - sizeof(test_data))
  );

  printk("\t\t<SmallSetRandomOps>\n");

  for (i = 1; i < small_set_size; ++i) {
    test_random_ops(
      tree,
      i,
      node_size,
      init_tree,
      init_node,
      get_data,
      insert,
      extract
    );
  }

  printk("\t\t</SmallSetRandomOps>\n");

  printk("\t\t<LargeSetRandomOps>\n");

  c = i;

  while (c < large_set_size) {
    test_random_ops(
      tree,
      c,
      node_size,
      init_tree,
      init_node,
      get_data,
      insert,
      extract
    );

    c = large_set_next(c);
  }

  printk("\t\t</LargeSetRandomOps>\n");

  printk("\t\t<SmallSetLinear>\n");

  for (i = 1; i < small_set_size; ++i) {
    test_linear(
      tree,
      i,
      node_size,
      init_tree,
      init_node,
      insert,
      extract
    );
  }

  printk("\t\t</SmallSetLinear>\n");

  printk("\t\t<LargeSetLinear>\n");

  c = i;

  while (c < large_set_size) {
    test_linear(
      tree,
      c,
      node_size,
      init_tree,
      init_node,
      insert,
      extract
    );

    c = large_set_next(c);
  }

  printk("\t\t</LargeSetLinear>\n");

  printk("\t</RBTest>\n");
}

void test_rbtree_boost(void);

void test_rbtree_bsd(void);

void test_rbtree_ec(void);

void test_rbtree_jffs2(void);

void test_rbtree_linux(void);

void test_rbtree_llrb(void);

void test_rbtree_rb(void);

void test_rbtree_rb_new(void);

void test_rbtree_rb_old(void);

void test_rbtree_rtems(void);

void test_rbtree_rtems_compact(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TEST_H */
