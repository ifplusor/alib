//
// Created by james on 9/28/17.
//
// reference:
//    http://adtinfo.org/libavl.html/AVL-Trees.html
//

#include "avl.h"

avl_node_t avl_default_replace(avl_node_t old, avl_node_t new) {
  return new;
}

avl_t avl_construct(avl_compare_func compare) {
  avl_t avl = NULL;

  do {
    if (compare == NULL) break;

    avl = amalloc(sizeof(avl_s));
    if (avl == NULL) break;

    avl->len = 0;
    avl->root = NULL;

    avl->compare = compare;
    avl->replace = avl_default_replace;

    return avl;
  } while (0);

  avl_destruct(avl);

  return avl;
}

bool avl_destruct(avl_t self) {
  if (self == NULL) return false;
  afree(self);
  return true;
}

void avl_reset(avl_t self) {
  self->root = NULL;
  self->len = 0;
}

/*
 *         A                 B
 *       +   +             +   +
 *     B       c  =>     d       A
 *   +   +                     +   +
 * d       e                 e       c
 */
avl_node_t avl_rotate_ll(avl_t self, avl_node_t a) {
  avl_node_t p = a->avl_parent;
  avl_node_t b = a->avl_left;
  avl_node_t e = b->avl_right;

  b->avl_parent = p;
  b->avl_right = a;

  a->avl_parent = b;
  a->avl_left = e;

  if (e != NULL)
    e->avl_parent = a;

  a->bf = 0;
  b->bf = 0;

  return b;
}

avl_node_t avl_rotate_lz(avl_t self, avl_node_t a) {
  avl_node_t p = a->avl_parent;
  avl_node_t b = a->avl_left;
  avl_node_t e = b->avl_right;

  b->avl_parent = p;
  b->avl_right = a;

  a->avl_parent = b;
  a->avl_left = e;

  if (e != NULL)
    e->avl_parent = a;

  a->bf = -1;
  b->bf = +1;

  return b;
}

/*
 *         A
 *       +   +                   E
 *     B       c              +     +
 *   +   +         =>      B          A
 * d       E             +   +      +   +
 *       +   +         d       f   g      c
 *     f       g
 */
avl_node_t avl_rotate_lr(avl_t self, avl_node_t a) {
  avl_node_t p = a->avl_parent;
  avl_node_t b = a->avl_left;
  avl_node_t e = b->avl_right;
  avl_node_t f = e->avl_left;
  avl_node_t g = e->avl_right;

  e->avl_left = b;
  e->avl_right = a;
  e->avl_parent = p;

  b->avl_parent = e;
  b->avl_right = f;

  a->avl_parent = e;
  a->avl_left = g;

  if (f != NULL)
    f->avl_parent = b;

  if (g != NULL)
    g->avl_parent = a;

  if (e->bf == 0) {
    a->bf = 0;
    b->bf = 0;
  } else if (e->bf > 0) {
    a->bf = 0;
    b->bf = -1;
  } else {
    a->bf = +1;
    b->bf = 0;
  }
  e->bf = 0;

  return e;
}

avl_node_t avl_rotate_rr(avl_t self, avl_node_t a) {
  avl_node_t p = a->avl_parent;
  avl_node_t b = a->avl_right;
  avl_node_t e = b->avl_left;

  b->avl_parent = p;
  b->avl_left = a;

  a->avl_parent = b;
  a->avl_right = e;

  if (e != NULL)
    e->avl_parent = a;

  a->bf = 0;
  b->bf = 0;

  return b;
}

avl_node_t avl_rotate_rz(avl_t self, avl_node_t a) {
  avl_node_t p = a->avl_parent;
  avl_node_t b = a->avl_right;
  avl_node_t e = b->avl_left;

  b->avl_parent = p;
  b->avl_left = a;

  a->avl_parent = b;
  a->avl_right = e;

  if (e != NULL)
    e->avl_parent = a;

  a->bf = +1;
  b->bf = -1;

  return b;
}

avl_node_t avl_rotate_rl(avl_t self, avl_node_t a) {
  avl_node_t p = a->avl_parent;
  avl_node_t b = a->avl_right;
  avl_node_t e = b->avl_left;
  avl_node_t f = e->avl_right;
  avl_node_t g = e->avl_left;

  e->avl_right = b;
  e->avl_left = a;
  e->avl_parent = p;

  b->avl_parent = e;
  b->avl_left = f;

  a->avl_parent = e;
  a->avl_right = g;

  if (f != NULL)
    f->avl_parent = b;

  if (g != NULL)
    g->avl_parent = a;

  if (e->bf == 0) {
    a->bf = 0;
    b->bf = 0;
  } else if (e->bf < 0) {
    a->bf = 0;
    b->bf = +1;
  } else {
    a->bf = -1;
    b->bf = 0;
  }
  e->bf = 0;

  return e;
}

avl_node_t avl_search_node(avl_t self, void *key,
                           avl_node_t *out_parent, uint64_t *out_path) {
  avl_node_t p = self->root, pp = NULL;
  uint64_t path = 1LL; // left is 0, right is 1.
  while (p != NULL) {
    int ret = self->compare(p, key);
    if (ret == 0) break;
    pp = p;
    path <<= 1;
    if (ret > 0) {
      p = p->avl_left;
    } else {
      p = p->avl_right;
      path |= 1LL;
    }
  }

  if (out_parent != NULL) *out_parent = pp;
  if (out_path != NULL) *out_path = path;

  return p;
}

static inline avl_node_t*
avl_node_container(avl_t self, avl_node_t node, uint64_t path) {
#if DEBUG_AVL
  return path == 1LL
         ? &self->root
         : (path & 1LL
            ? &node->avl_parent->avl_right
            : &node->avl_parent->avl_left);
#else
  return path == 1LL ? &self->root : &node->avl_parent->link[path & 1LL];
#endif
}

void avl_insert_node(avl_t self, avl_node_t node, avl_node_t parent,
                     uint64_t path) {
  avl_node_t *pc;
  avl_node_t p = node;

  // insert node as leaf
  p->avl_parent = parent;
  p->avl_left = NULL;
  p->avl_right = NULL;
  p->bf = 0;
  pc = avl_node_container(self, p, path);
  *pc = p;

  // update balance factor, and re-balance
  p = parent;
  while (p != NULL) {
    if (path & 1LL) { // right
      path >>= 1;
      p->bf++;
      if (p->bf >= 2) {
        pc = avl_node_container(self, p, path);
        if (p->avl_right->bf >= 0) {
          *pc = avl_rotate_rr(self, p);
        } else {
          *pc = avl_rotate_rl(self, p);
        }
        break;
      }
    } else { // left
      path >>= 1;
      p->bf--;
      if (p->bf <= -2) {
        pc = avl_node_container(self, p, path);
        if (p->avl_left->bf <= 0) {
          *pc = avl_rotate_ll(self, p);
        } else {
          *pc = avl_rotate_lr(self, p);
        }
        break;
      }
    }
    if (p->bf == 0) break;
    p = p->avl_parent;
  }

  self->len++;
}

void avl_delete_node(avl_t self, avl_node_t node, uint64_t path) {
  avl_node_t *pc;
  avl_node_t p = node, pp;

  // delete node
  pc = avl_node_container(self, p, path);
  if (p->avl_right == NULL) {
    // replace p by p.left
    *pc = p->avl_left;
    if (p->avl_left != NULL)
      p->avl_left->avl_parent = p->avl_parent;
    pp = p->avl_parent;
  } else {
    avl_node_t bp = p->avl_right; // bigger than p
    path <<= 1;
    path |= 1LL;
    while (bp->avl_left != NULL) {
      path <<= 1;
      bp = bp->avl_left;
    }

    if (bp->avl_parent == p) {
      pp = bp;
    } else {
      pp = bp->avl_parent;
    }

    // extract bp
    *avl_node_container(self, bp, path) = bp->avl_right;
    if (bp->avl_right != NULL)
      bp->avl_right->avl_parent = bp->avl_parent;

    // 'replace' p by bp
    *bp = *p;
    *pc = bp;
    if (bp->avl_left != NULL) bp->avl_left->avl_parent = bp;
    if (bp->avl_right != NULL) bp->avl_right->avl_parent = bp;
  }

  // update balance factor, and re-balance
  p = pp;
  while (p != NULL) {
    if (path & 1LL) { // right
      path >>= 1;
      p->bf--;
      if (p->bf == -1) break; // don't change height
      if (p->bf <= -2) { // re-balance
        pc = avl_node_container(self, p, path);
        if (p->avl_left->bf == 0) {
          *pc = avl_rotate_lz(self, p);
          break;
        } else if (p->avl_left->bf < 0) {
          *pc = avl_rotate_ll(self, p);
        } else {
          *pc = avl_rotate_lr(self, p);
        }
        p = *pc;
      }
    } else { // left
      path >>= 1;
      p->bf++;
      if (p->bf == 1) break;
      if (p->bf >= 2) {
        pc = avl_node_container(self, p, path);
        if (p->avl_right->bf == 0) {
          *pc = avl_rotate_rz(self, p);
          break;
        } else if (p->avl_right->bf > 0) {
          *pc = avl_rotate_rr(self, p);
        } else {
          *pc = avl_rotate_rl(self, p);
        }
        p = *pc;
      }
    }
    p = p->avl_parent;
  }

  self->len--;
}

void avl_replace_node(avl_t self, avl_node_t old, avl_node_t new,
                      uint64_t path) {
  *new = *old;
  *avl_node_container(self, new, path) = new;
  if (new->avl_left != NULL) new->avl_left->avl_parent = new;
  if (new->avl_right != NULL) new->avl_right->avl_parent = new;
}

/**
 * @return old node
 */
avl_node_t avl_insert(avl_t self, void *key, avl_node_t node) {
  avl_node_t p, pp;
  uint64_t path;

  p = avl_search_node(self, key, &pp, &path);
  node = self->replace(p, node);
  if (p != NULL) { // hit
    if (node == NULL) { // delete node
      avl_delete_node(self, p, path);
      return p;
    } else if (node != p) { // replace
      avl_replace_node(self, p, node, path);
      return p;
    }
  } else if (node != NULL) { // new node
    avl_insert_node(self, node, pp, path);
  }
  return NULL;
}

/**
 * @return old node
 */
avl_node_t avl_replace(avl_t self, void *key, avl_node_t node) {
  avl_node_t p;
  uint64_t path;

  p = avl_search_node(self, key, NULL, &path);
  if (p != NULL) { // hit
    if (node == NULL) { // delete node
      avl_delete_node(self, p, path);
      return p;
    } else if (node != p) {
      avl_replace_node(self, p, node, path);
      return p;
    }
  }
  return NULL;
}

/**
 * @return old node
 */
avl_node_t avl_delete(avl_t self, void *key) {
  avl_node_t p;
  uint64_t path;

  p = avl_search_node(self, key, NULL, &path);
  if (p != NULL) { // hit
    avl_delete_node(self, p, path);
  }
  return p;
}

avl_node_t avl_search(avl_t self, void *key) {
  return avl_search_node(self, key, NULL, NULL);
}
