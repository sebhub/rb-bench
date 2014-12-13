/*
 *  Copyright (c) 2010 Gedare Bloom.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include "compat.h"
#include "rbtree-rtems-compact-impl.h"

static bool _RBTree_Is_null_or_black( const RBTree_Node *node )
{
  return node == NULL || _RBTree_Color( node ) == RBT_BLACK;
}

/** @brief  Validate and fix-up tree properties after deleting a node
 *
 *  This routine is called on a black node, @a the_node, after its deletion.
 *  This function maintains the properties of the red-black tree.
 */
static void rtems_rbtree_extract_validate( RBTree_Node *the_node )
{
  RBTree_Color  color;
  RBTree_Node  *parent = _RBTree_Parent_and_color( the_node, &color );
  RBTree_Color  parentcolor;
  RBTree_Node  *grandparent = _RBTree_Parent_and_color( parent, &parentcolor );

  if ( grandparent == NULL )
    return;

  /* continue to correct tree as long as the_node is black and not the root */
  while ( grandparent != NULL &&  color == RBT_BLACK ) {
    RBTree_Node  *sibling = _RBTree_Sibling( the_node, parent );

    /* if sibling is red, switch parent (black) and sibling colors,
     * then rotate parent left, making the sibling be the_node's grandparent.
     * Now the_node has a black sibling and red parent. After rotation,
     * update sibling pointer.
     */
    if ( _RBTree_Color( sibling ) == RBT_RED ) {
      RBTree_Direction dir = _RBTree_Direction( the_node, parent );
      RBTree_Direction opp_dir = _RBTree_Opposite_direction( dir );

      parentcolor = RBT_RED;
      _RBTree_Set_parent_and_color( parent, grandparent, parentcolor );
      _RBTree_Set_parent_and_color( sibling, parent, RBT_BLACK );

      _RBTree_Rotate( parent, dir );
      grandparent = sibling;
      sibling = parent->child[ opp_dir ];
    }

    /* sibling is black, see if both of its children are also black. */
    if ( _RBTree_Is_null_or_black( sibling->child[ RBT_RIGHT ] ) &&
         _RBTree_Is_null_or_black( sibling->child[ RBT_LEFT ] ) ) {
      _RBTree_Set_parent_and_color( sibling, parent, RBT_RED );

      if ( parentcolor == RBT_RED ) {
        _RBTree_Set_parent_and_color( parent, grandparent, RBT_BLACK );
        break;
      }

      the_node = parent;   /* done if parent is red */
      parent = grandparent;
      grandparent = _RBTree_Parent_and_color( parent, &parentcolor );
    } else {
      /* at least one of sibling's children is red. we now proceed in two
       * cases, either the_node is to the left or the right of the parent.
       * In both cases, first check if one of sibling's children is black,
       * and if so rotate in the proper direction and update sibling pointer.
       * Then switch the sibling and parent colors, and rotate through parent.
       */
      RBTree_Direction dir = _RBTree_Direction( the_node, parent );
      RBTree_Direction opp_dir = _RBTree_Opposite_direction( dir );

      if ( _RBTree_Is_null_or_black( sibling->child[ opp_dir ] ) ) {
        _RBTree_Set_parent_and_color( sibling, parent, RBT_RED );
        _RBTree_Set_parent_and_color(
          sibling->child[ dir ],
          sibling,
          RBT_BLACK
         );
        _RBTree_Rotate( sibling, opp_dir );
        sibling = parent->child[ opp_dir ];
      }

      _RBTree_Set_parent_and_color( sibling, parent, parentcolor );
      _RBTree_Set_parent_and_color( parent, grandparent, RBT_BLACK );
      _RBTree_Set_parent_and_color(
         sibling->child[ opp_dir ],
         sibling,
         RBT_BLACK
       );

      _RBTree_Rotate( parent, dir );
      grandparent = sibling;
      break; /* done */
    }
  } /* while */

  if ( grandparent == NULL )
    _RBTree_Set_parent_and_color( the_node, parent, RBT_BLACK );
}

void rtems_rbtree_compact_extract(
  RBTree_Control *the_rbtree,
  RBTree_Node    *the_node
)
{
  RBTree_Node     *leaf;
  RBTree_Color     color;
  RBTree_Node     *parent;
  RBTree_Color     victim_color;
  RBTree_Direction dir;

  /* check if min needs to be updated */
  if ( the_node == the_rbtree->first[ RBT_LEFT ] ) {
    the_rbtree->first[ RBT_LEFT ] = _RBTree_Successor( the_node );
  }

  /* Check if max needs to be updated. min=max for 1 element trees so
   * do not use else if here. */
  if ( the_node == the_rbtree->first[ RBT_RIGHT ] ) {
    the_rbtree->first[ RBT_RIGHT ] = _RBTree_Predecessor( the_node );
  }

  /* if the_node has at most one non-null child then it is safe to proceed
   * check if both children are non-null, if so then we must find a target node
   * either max in node->child[RBT_LEFT] or min in node->child[RBT_RIGHT],
   * and replace the_node with the target node. This maintains the binary
   * search tree property, but may violate the red-black properties.
   */

  if ( the_node->child[ RBT_LEFT ] && the_node->child[ RBT_RIGHT ] ) {
    /* find max in node->child[RBT_LEFT] */
    RBTree_Node  *target = the_node->child[ RBT_LEFT ];
    RBTree_Color  targetcolor;
    RBTree_Node  *targetparent;

    while ( target->child[ RBT_RIGHT ] != NULL )
      target = target->child[ RBT_RIGHT ];

    /* if the target node has a child, need to move it up the tree into
     * target's position (target is the right child of target->parent)
     * when target vacates it. if there is no child, then target->parent
     * should become NULL. This may cause the coloring to be violated.
     * For now we store the color of the node being deleted in victim_color.
     */
    leaf = target->child[ RBT_LEFT ];

    targetparent = _RBTree_Parent_and_color( target, &targetcolor );
    if ( leaf != NULL ) {
      _RBTree_Set_parent( leaf, targetparent );
    } else {
      /* fix the tree here if the child is a null leaf. */
      rtems_rbtree_extract_validate( target );
      targetparent = _RBTree_Parent_and_color( target, &targetcolor );
    }

    victim_color = targetcolor;
    dir = _RBTree_Direction( target, targetparent );
    targetparent->child[ dir ] = leaf;

    /* now replace the_node with target */
    parent = _RBTree_Parent_and_color( the_node, &color );
    dir = _RBTree_Direction( the_node, parent );
    parent->child[ dir ] = target;

    /* set target's new children to the original node's children */
    target->child[ RBT_RIGHT ] = the_node->child[ RBT_RIGHT ];

    if ( the_node->child[ RBT_RIGHT ] )
      _RBTree_Set_parent( the_node->child[ RBT_RIGHT ], target );

    target->child[ RBT_LEFT ] = the_node->child[ RBT_LEFT ];

    if ( the_node->child[ RBT_LEFT ] )
      _RBTree_Set_parent( the_node->child[ RBT_LEFT ], target );

    /* finally, update the parent node and recolor. target has completely
     * replaced the_node, and target's child has moved up the tree if needed.
     * the_node is no longer part of the tree, although it has valid pointers
     * still.
     */
    _RBTree_Set_parent_and_color( target, parent, color );
  } else {
    parent = _RBTree_Parent_and_color( the_node, &color );

    /* the_node has at most 1 non-null child. Move the child in to
     * the_node's location in the tree. This may cause the coloring to be
     * violated. We will fix it later.
     * For now we store the color of the node being deleted in victim_color.
     */
    leaf = the_node->child[ RBT_LEFT ] ?
           the_node->child[ RBT_LEFT ] : the_node->child[ RBT_RIGHT ];

    if ( leaf != NULL ) {
      _RBTree_Set_parent( leaf, parent );
    } else {
      /* fix the tree here if the child is a null leaf. */
      rtems_rbtree_extract_validate( the_node );
      parent = _RBTree_Parent_and_color( the_node, &color );
    }

    victim_color = color;

    /* remove the_node from the tree */
    dir = _RBTree_Direction( the_node, parent );
    parent->child[ dir ] = leaf;
  }

  /* fix coloring. leaf has moved up the tree. The color of the deleted
   * node is in victim_color. There are two cases:
   *   1. Deleted a red node, its child must be black. Nothing must be done.
   *   2. Deleted a black node, its child must be red. Paint child black.
   */
  if ( victim_color == RBT_BLACK ) { /* eliminate case 1 */
    if ( leaf != NULL ) {
      _RBTree_Set_color( leaf, RBT_BLACK ); /* case 2 */
    }
  }

  /* set root to black, if it exists */
  if ( the_rbtree->root != NULL )
    _RBTree_Set_color( the_rbtree->root, RBT_BLACK );
}
