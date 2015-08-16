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

#ifndef BHEAP_H
#define BHEAP_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct bheap_node {
	/*
	 * The layout is carefully chosen so that bheap_head::root and
	 * bheap_node::child[0] are on the same offset.  This simplifies the
	 * insert operation (dir == 0 initially).
	 */
	struct bheap_node *child[2];
	int index;
};

struct bheap_head {
	struct bheap_node *root;
	int size;
};

typedef int (*bheap_less)(const struct bheap_node *left,
    const struct bheap_node *right);

static inline void
bheap_init(struct bheap_head *head)
{
	head->root = NULL;
	head->size = 0;
}

static inline int
bheap_shift(int index)
{
	return ((int)sizeof(int) * 8 - 2 - __builtin_clz(index));
}

static inline struct bheap_node *
bheap_get(struct bheap_head *head, int index)
{
	int shift = bheap_shift(index) + 1;
	struct bheap_node *current = (struct bheap_node *)head;
	int dir = 0;

	while (shift > 0) {
		--shift;
		current = current->child[dir];
		dir = (index >> shift) & 1;
	}

	return (current->child[dir]);
}

static inline struct bheap_node *
bheap_bubble_down(struct bheap_node *node, bheap_less less,
    int index, struct bheap_node **parent_p, int *dir_p)
{
	int shift = bheap_shift(index);
	struct bheap_node *current = *parent_p;
	int dir = *dir_p;

	do {
		struct bheap_node *last = current;

		current = current->child[dir];

		if ((*less)(node, current)) {
			struct bheap_node *tmp = current;

			*node = *current;
			last->child[dir] = node;

			current = node;
			node = tmp;
		}

		dir = (index >> shift) & 1;
		--shift;
	} while (shift >= 0);

	*parent_p = current;
	*dir_p = dir;

	return (node);
}

static inline void
bheap_move_down(struct bheap_node *node,
    bheap_less less, struct bheap_node *parent, int dir, int index,
    int new_size)
{
	for (;;) {
		struct bheap_node *next;
		int left = index << 1;

		if (left < new_size) {
			int next_dir = (*less)(node->child[1], node->child[0]);
			int next_index = left | next_dir;

			next = node->child[next_dir];

			if ((*less)(next, node)) {
				int opp_dir = (next_dir + 1) & 1;
				struct bheap_node *tmp;

				parent->child[dir] = next;
				tmp = node->child[opp_dir];
				node->child[0] = next->child[0];
				node->child[1] = next->child[1];
				node->index = next_index;
				next->child[next_dir] = node;
				next->child[opp_dir] = tmp;
				next->index = index;

				parent = next;
				dir = next_dir;
				index = next_index;
				continue;
			}
		} else if (left == new_size) {
			next = node->child[0];

			if ((*less)(next, node)) {
				parent->child[dir] = next;
				next->child[0] = node;
				next->index = index;
				node->index = left;
			}
		}

		break;
	}
}

static inline void
bheap_insert(struct bheap_head *head, struct bheap_node *node, bheap_less less)
{
	int old_size = head->size;
	int new_size = old_size + 1;
	struct bheap_node *parent = (struct bheap_node *)head;
	int dir = 0;

	head->size = new_size;

	if (old_size != 0) {
		node = bheap_bubble_down(node, less, new_size, &parent, &dir);
	}

	parent->child[dir] = node;
	node->index = new_size;
}

static inline void
bheap_extract(struct bheap_head *head, struct bheap_node *node,
    bheap_less less)
{
	int old_size = head->size;
	int new_size = old_size - 1;
	int index = node->index;

	head->size = new_size;

	if (index != old_size) {
		struct bheap_node *last = bheap_get(head, old_size);
		struct bheap_node *repl;
		struct bheap_node *parent = (struct bheap_node *)head;
		int dir = 0;

		if (index == 1) {
			repl = last;
		} else {
			repl = bheap_bubble_down(last, less, index, &parent,
			    &dir);
		}

		parent->child[dir] = repl;
		*repl = *node;

		if (repl == last) {
			bheap_move_down(last, less, parent, dir, index,
			    new_size);
		}
	} else if (index == 1) {
		head->root = NULL;
	}
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* BHEAP_H */
