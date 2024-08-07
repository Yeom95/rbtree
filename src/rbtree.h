#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>

typedef enum { RBTREE_RED, RBTREE_BLACK } color_t;
typedef enum { LEFT_CHILD,RIGHT_CHILD } position_t;
typedef enum { ROTATE_LEFT,ROTATE_RIGHT } directon_t;
typedef enum { CASE_ONE,CASE_TWO,CASE_THREE,CASE_FOUR } fixup_case;

typedef int key_t;

typedef struct node_t {
  color_t color;
  key_t key;
  struct node_t *parent, *left, *right;
} node_t;

typedef struct {
  node_t *root;
  node_t *nil;  // for sentinel
} rbtree;

rbtree *new_rbtree(void);
void delete_rbtree(rbtree *);

node_t *rbtree_insert(rbtree *, const key_t);
node_t *rbtree_find(const rbtree *, const key_t);
node_t *rbtree_min(const rbtree *);
node_t *rbtree_max(const rbtree *);

int rbtreeInsertFixup(rbtree *,node_t *);
node_t *RunInsertFixup(rbtree *,node_t *,node_t *,int,int);
node_t *InsertFixupCaseOne(rbtree *,node_t *,node_t *);
node_t *InsertFixupCaseTwo(rbtree *,node_t *,int);
node_t *InsertFixupCaseThree(rbtree *,node_t *,int);

int rbtree_erase(rbtree *, node_t *);
int rbtreeEraseFixup(rbtree *,node_t *);
int rbtreeTransplant(rbtree *,node_t *,node_t *);

void RunEraseFixup(rbtree *,node_t **,node_t **,int,int);
void EraseFixupCaseOne(rbtree *,node_t **,node_t **,int);
void EraseFixupCaseTwo(node_t **,node_t **);
void EraseFixupCaseThree(rbtree *,node_t **,node_t **,int);
void EraseFixupCaseFour(rbtree *,node_t **,node_t **,int);

int rbtree_to_array(const rbtree *, key_t *, const size_t);
void rotate_rbtree(rbtree *,node_t *,int);

#endif  // _RBTREE_H_
