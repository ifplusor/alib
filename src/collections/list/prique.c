/**
 * prique.c
 *
 * @author James Yin <ywhjames@hotmail.com>
 */
#include "alib/collections/list/prique.h"

prique_t prique_construct(prique_compare_f compare_func) {
  prique_t prique = amalloc(sizeof(prique_s));
  if (prique == NULL) {
    return NULL;
  }

  prique->compare_func = compare_func;
  prique->len = 0;
  prique->capacity = 16;
  prique->data = amalloc(sizeof(void*) * (prique->capacity + 1));
  if (prique->data == NULL) {
    afree(prique);
    return NULL;
  }
  prique->data[0] = NULL;

  return prique;
}

void prique_destruct(prique_t self) {
  if (self != NULL) {
    afree(self->data);
    afree(self);
  }
}

static size_t prique_alloc_node(prique_t self) {
  if (self->len > self->capacity) {
    size_t new_size = self->capacity << 1;
    void** new_space = arealloc(self->data, sizeof(void*) * (new_size + 1));
    if (new_space == NULL) {
      // ...
    }
    self->data = new_space;
    self->capacity = new_size;
  }
  return ++self->len;
}

static void prique_shift_up(prique_t self, size_t node) {
  // 将插入的元素放到合适位置，保持最小堆
  size_t parent = node >> 1;
  while (node > 1 && self->compare_func(self->data[node], self->data[parent]) < 0) {
    alib_swap(void*, self->data[node], self->data[parent]);
    node = parent;
    parent = node >> 1;
  }
}

static void prique_shift_down(prique_t self, size_t node) {
  // 将堆的索引位置元素向下移动到合适位置，保持最小堆
  size_t bound = self->len;
  size_t child = node << 1;
  while (child <= bound) {
    // get lesser child
    if (child + 1 <= bound) {
      if (self->compare_func(self->data[child], self->data[child + 1]) > 0) {
        child = child + 1;
      }
    }
    if (self->compare_func(self->data[node], self->data[child]) <= 0) {
      break;
    }
    alib_swap(void*, self->data[node], self->data[child]);
    node = child;
    child = node << 1;
  }
}

void* prique_peek(prique_t self) {
  if (self->len > 0) {
    return self->data[1];
  }
  return NULL;
}

void prique_push(prique_t self, void* value) {
  // 插入元素入堆
  if (value != NULL) {
    size_t node = prique_alloc_node(self);
    self->data[node] = value;
    prique_shift_up(self, node);
  }
}

void* prique_pop(prique_t self) {
  // 出堆
  if (self->len <= 0) {
    return NULL;
  } else if (self->len > 1) {
    void* value = self->data[1];
    self->data[1] = self->data[self->len--];
    prique_shift_down(self, 1);
    return value;
  } else {
    return self->data[self->len--];
  }
}
