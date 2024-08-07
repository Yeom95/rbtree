#include "rbtree.h"

#include <stdlib.h>

#define SENTINEL = nil;

//nil 
node_t nil = {
  RBTREE_BLACK,
  NULL,
  NULL,
  NULL,
  NULL
};

//트리를 생성
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  p->nil = &nil;
  p->root = p->nil;

  return p;
}

//트리를 삭제
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory

  while(t->root != &nil){
    rbtree_erase(t,t->root);
  }

  free(t);
  t = NULL;
}

//트리에 삽입
node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  //x=>searchNode,y=>parentNode,z=>insertNode
  node_t *insertNode = (node_t*)malloc(sizeof(node_t));

  insertNode->color = RBTREE_RED;
  insertNode->key = key;
  insertNode->left = NULL;
  insertNode->right = NULL;
  insertNode->parent = NULL;

  node_t *searchNode = t->root;
  node_t *parentNode = t->nil;

  while(searchNode != t->nil){
    parentNode = searchNode;
    if(insertNode->key < searchNode->key)
      searchNode = searchNode->left;
    else
      searchNode = searchNode->right;
  }

  insertNode->parent = parentNode;

  if(parentNode == t->nil)
    t->root = insertNode;
  else if (insertNode->key < parentNode->key)
    parentNode->left = insertNode;
  else
    parentNode->right = insertNode;
  
  insertNode->left = t->nil;
  insertNode->right = t->nil;

  rbtreeInsertFixup(t,insertNode);

  return t->root;
}

int rbtreeInsertFixup(rbtree *tree,node_t *fixupNode){
  //node_t *parentNode = fixupNode->parent;
  node_t *uncleNode;
  int parentPosition = 0;

  while(fixupNode->parent->color == RBTREE_RED){
    if(fixupNode->parent == fixupNode->parent->parent->left){
      uncleNode = fixupNode->parent->parent->right;
      parentPosition = LEFT_CHILD;

      if(uncleNode->color == RBTREE_RED)
        fixupNode = RunInsertFixup(tree,fixupNode,uncleNode,CASE_ONE,parentPosition);
      else{
        if(fixupNode == fixupNode->parent->right)
          fixupNode = RunInsertFixup(tree,fixupNode,uncleNode,CASE_TWO,parentPosition);
        
        fixupNode = RunInsertFixup(tree,fixupNode,uncleNode,CASE_THREE,parentPosition);
      }
    }
    else{
      uncleNode = fixupNode->parent->parent->left;
      parentPosition = RIGHT_CHILD;

      if(uncleNode->color == RBTREE_RED)
        fixupNode = RunInsertFixup(tree,fixupNode,uncleNode,CASE_ONE,parentPosition);
      else{
        if(fixupNode == fixupNode->parent->left)
          fixupNode = RunInsertFixup(tree,fixupNode,uncleNode,CASE_TWO,parentPosition);
        
        fixupNode = RunInsertFixup(tree,fixupNode,uncleNode,CASE_THREE,parentPosition);
      }
    }
  }

  if(fixupNode == tree->root)
    fixupNode->color = RBTREE_BLACK;

  return 0;
}

node_t *RunInsertFixup(rbtree *tree,node_t *fixupNode,node_t *uncleNode,int fixupCase,int position){
  //케이스에 맞춰서 설정
  switch (fixupCase)
  {
  case CASE_ONE:
    fixupNode = InsertFixupCaseOne(tree,fixupNode,uncleNode);
    break;

  case CASE_TWO:
    fixupNode = InsertFixupCaseTwo(tree,fixupNode,position);
    break;

  case CASE_THREE:
    fixupNode = InsertFixupCaseThree(tree,fixupNode,position);
  
  default:
    break;
  }

  return fixupNode;
}

node_t *InsertFixupCaseOne(rbtree *tree,node_t *fixupNode,node_t *uncleNode){
  fixupNode->parent->color = RBTREE_BLACK;
  uncleNode->color = RBTREE_BLACK;
  //if(fixupNode->parent->parent != tree->nil)
  fixupNode->parent->parent->color = RBTREE_RED;
  fixupNode = fixupNode->parent->parent;

  return fixupNode;
}

node_t *InsertFixupCaseTwo(rbtree *tree,node_t *fixupNode,int position){
  fixupNode = fixupNode->parent;

  switch (position)
  {
  case LEFT_CHILD:
    rotate_rbtree(tree,fixupNode,ROTATE_LEFT);
    break;

  case RIGHT_CHILD:
    rotate_rbtree(tree,fixupNode,ROTATE_RIGHT);
    break;
  
  default:
    printf("Wrong node position ");
    break;
  }

  return fixupNode;
}

node_t *InsertFixupCaseThree(rbtree *tree,node_t *fixupNode,int position){
  fixupNode->parent->color = RBTREE_BLACK;
  //if(fixupNode->parent->parent != tree->nil)
  fixupNode->parent->parent->color = RBTREE_RED;
  
  switch (position)
  {
  case LEFT_CHILD:
    rotate_rbtree(tree,fixupNode->parent->parent,ROTATE_RIGHT);
    break;

  case RIGHT_CHILD:
    rotate_rbtree(tree,fixupNode->parent->parent,ROTATE_LEFT);
    break;
  default:
    printf("Wrong node position");
    break;
  }

  return fixupNode;
}

//트리에서 검색
node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *searchNode = t->root;

  while(searchNode != t->nil && key != searchNode->key){
    if(key < searchNode->key)
      searchNode = searchNode->left;
    else
      searchNode = searchNode->right;
  }

  if(searchNode == t->nil)
    return NULL;

  return searchNode;
}

//트리 최소값
node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *minNode = t->root;
  while(minNode->left != t->nil)
    minNode = minNode->left;

  return minNode;
}

//트리 최대값
node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *maxNode = t->root;
  while(maxNode->right != t->nil)
    maxNode = maxNode->right;

  return maxNode;
}

node_t *GetSuccessor(rbtree *tree,node_t *rightChild){
  node_t *successor = rightChild;
  while(successor->left != tree->nil)
    successor = successor->left;

  return successor;
}

//트리에서 노드 삭제
int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  //z=>p,y=>replaceNode,x=>fixupNode
  node_t *replaceNode = p;
  node_t *fixupNode;
  int replaceNodeColor = replaceNode->color;

  if(p->left == t->nil){
    fixupNode = p->right;
    rbtreeTransplant(t,p,p->right);
  } 
  else if (p->right == t->nil){
    fixupNode = p->left;
    rbtreeTransplant(t,p,p->left);
  }
  else{
    replaceNode = GetSuccessor(t,p->right);
    replaceNodeColor = replaceNode->color;
    fixupNode = replaceNode->right;

    if(replaceNode != p->right){
      rbtreeTransplant(t,replaceNode,replaceNode->right);
      replaceNode->right = p->right;
      replaceNode->right->parent = replaceNode;
    }
    else
      fixupNode->parent = replaceNode;

    rbtreeTransplant(t,p,replaceNode);
    replaceNode->left = p->left;
    replaceNode->left->parent = replaceNode;
    replaceNode->color = p->color;
  }

  free(p);
  p = NULL;

  if(replaceNodeColor == RBTREE_BLACK)
    rbtreeEraseFixup(t,fixupNode);
  
  return 0;
}

int rbtreeTransplant(rbtree *tree,node_t *replaceNode,node_t *transplantNode){
  //u=>replaceNode,v=>transplantNode
  if(replaceNode->parent == tree->nil)
    tree->root = transplantNode;
  else if (replaceNode == replaceNode->parent->left)
    replaceNode->parent->left = transplantNode;
  else
    replaceNode->parent->right = transplantNode;

  transplantNode->parent = replaceNode->parent;

  return 0;
}

int rbtreeEraseFixup(rbtree *tree,node_t *fixupNode){
   //T => *t,x => *p

  //node_t *parentNode = fixupNode->parent;
  /*node_t *siblingNode = NULL;
  int nodePosition = 0;


  while(*fixupNode != tree->root && (*fixupNode)->color == RBTREE_BLACK){

    //부모노드의 왼쪽에 위치할 경우
    if(*fixupNode == (*fixupNode)->parent->left){
      nodePosition = LEFT_CHILD;
      siblingNode = (*fixupNode)->parent->right;

      //case 1:uncleNode가 RED
      if(siblingNode->color == RBTREE_RED){
        RunEraseFixup(tree,fixupNode,&siblingNode,CASE_ONE,nodePosition);
      }
      //case 2:uncleNode가 BLACK,cousinNode가 전부 BLACK
      if(siblingNode->left->color == RBTREE_BLACK && siblingNode->right->color == RBTREE_BLACK){
        RunEraseFixup(tree,fixupNode,&siblingNode,CASE_TWO,nodePosition);
      }
      //case 2가 아니라면 case 3,case 4 체크 시작
      else{
        //삼촌노드와 같은 방향의 조카노드 색이 검정일경우 case 3(삭제노드는 부모노드의 왼쪽에 위치)
        if(siblingNode->right->color == RBTREE_BLACK){
          RunEraseFixup(tree,fixupNode,&siblingNode,CASE_THREE,nodePosition);
        }
        //case 4
        RunEraseFixup(tree,fixupNode,&siblingNode,CASE_FOUR,nodePosition);
      }
    }
    else{
      nodePosition = RIGHT_CHILD;
      siblingNode = (*fixupNode)->parent->left;

      //case 1:uncleNode가 RED
      if(siblingNode->color == RBTREE_RED){
        RunEraseFixup(tree,fixupNode,&siblingNode,CASE_ONE,nodePosition);
      }
      //case 2:uncleNode가 BLACK,cousinNode가 전부 BLACK
      if(siblingNode->left->color == RBTREE_BLACK && siblingNode->right->color == RBTREE_BLACK){
        RunEraseFixup(tree,fixupNode,&siblingNode,CASE_TWO,nodePosition);
      }
      //case 2가 아니라면 case 3,case 4 체크 시작
      else{
        //삼촌노드와 같은 방향의 조카노드 색이 검정일경우 case 3(삭제노드는 부모노드의 왼쪽에 위치)
        if(siblingNode->left->color == RBTREE_BLACK){
          RunEraseFixup(tree,fixupNode,&siblingNode,CASE_THREE,nodePosition);
        }
        //case 4
        RunEraseFixup(tree,fixupNode,&siblingNode,CASE_FOUR,nodePosition);
      }
    }
  };*/

  node_t *siblingNode = NULL;
  int nodePosition = 0;

  while(fixupNode != tree->root && fixupNode->color == RBTREE_BLACK){
    //부모노드의 왼쪽에 위치할 경우
    if(fixupNode == fixupNode->parent->left){
      nodePosition = LEFT_CHILD;
      siblingNode = fixupNode->parent->right;
      //case 1:siblingNode가 RED
      if(siblingNode->color == RBTREE_RED){
        siblingNode->color = RBTREE_BLACK;
        fixupNode->parent->color = RBTREE_RED;
        rotate_rbtree(tree,fixupNode->parent,ROTATE_LEFT);
        siblingNode = fixupNode->parent->right;
      }
      //case 2:siblingNode가 BLACK,cousinNode가 전부 BLACK
      if(siblingNode->left->color == RBTREE_BLACK && siblingNode->right->color == RBTREE_BLACK){
        siblingNode->color = RBTREE_RED;
        fixupNode = fixupNode->parent;
      }
      //case 2가 아니라면 case 3,case 4 체크 시작
      else{
        if(siblingNode->right->color == RBTREE_BLACK){
          siblingNode->left->color = RBTREE_BLACK;
          siblingNode->color = RBTREE_RED;
          rotate_rbtree(tree,siblingNode,ROTATE_RIGHT);
          siblingNode = fixupNode->parent->right;
        }
        //case 4
        siblingNode->color = fixupNode->parent->color;
        fixupNode->parent->color = RBTREE_BLACK;
        siblingNode->right->color = RBTREE_BLACK;
        rotate_rbtree(tree,fixupNode->parent,ROTATE_LEFT);
        fixupNode = tree->root;
      }
    }
    else{
      nodePosition = RIGHT_CHILD;
      siblingNode = fixupNode->parent->left;
      //case 1:siblingNode가 RED
      if(siblingNode->color == RBTREE_RED){
        siblingNode->color = RBTREE_BLACK;
        fixupNode->parent->color = RBTREE_RED;
        rotate_rbtree(tree,fixupNode->parent,ROTATE_RIGHT);
        siblingNode = fixupNode->parent->left;
      }
      //case 2:siblingNode가 BLACK,cousinNode가 전부 BLACK
      if(siblingNode->right->color == RBTREE_BLACK && siblingNode->left->color == RBTREE_BLACK){
        siblingNode->color = RBTREE_RED;
        fixupNode = fixupNode->parent;
      }
      //case 2가 아니라면 case 3,case 4 체크 시작
      else{
        //형제노드와 같은 방향의 조카노드(형제노드의 left,right) 색이 검정일경우 case 3(삭제노드는 부모노드의 왼쪽에 위치)
        if(siblingNode->left->color == RBTREE_BLACK){
          siblingNode->right->color = RBTREE_BLACK;
          siblingNode->color = RBTREE_RED;
          rotate_rbtree(tree,siblingNode,ROTATE_LEFT);
          siblingNode = fixupNode->parent->left;
        }
        //case 4
        siblingNode->color = fixupNode->parent->color;
        fixupNode->parent->color = RBTREE_BLACK;
        siblingNode->left->color = RBTREE_BLACK;
        rotate_rbtree(tree,fixupNode->parent,ROTATE_RIGHT);
        fixupNode = tree->root;
      }
    }
  };


  //(*fixupNode)->color = RBTREE_BLACK;

  fixupNode->color = RBTREE_BLACK;
  return 0;
}

void RunEraseFixup(rbtree *tree,node_t **fixupNode,node_t **siblingNode,int fixupCase,int position){
  //케이스에 맞춰서 설정
  switch (fixupCase)
  {
  case CASE_ONE:
    EraseFixupCaseOne(tree,siblingNode,fixupNode,position);
    break;

  case CASE_TWO:
    EraseFixupCaseTwo(fixupNode,siblingNode);
    break;

  case CASE_THREE:
    EraseFixupCaseThree(fixupNode,siblingNode,fixupNode,position);
    break;

  case CASE_FOUR:
    EraseFixupCaseFour(tree,fixupNode,siblingNode,position);
    break;
  
  default:
    break;
  }

  return;
}

void EraseFixupCaseOne(rbtree *tree,node_t **siblingNode,node_t **fixupNode,int position){
  (*siblingNode)->color = RBTREE_BLACK;
  (*fixupNode)->parent->color = RBTREE_RED;

  switch (position)
  {
  case LEFT_CHILD:
    rotate_rbtree(tree,(*fixupNode)->parent,ROTATE_LEFT);
    *siblingNode = (*fixupNode)->parent->right;
    break;

  case RIGHT_CHILD:
    rotate_rbtree(tree,(*fixupNode)->parent,ROTATE_RIGHT);
    *siblingNode = (*fixupNode)->parent->left;
    break;
  
  default:
    printf("Wrong node position");
    break;
  }

  return;
}

void EraseFixupCaseTwo(node_t **fixupNode,node_t **siblingNode){
  (*siblingNode)->color = RBTREE_RED;
  *fixupNode = (*fixupNode)->parent;

  return;
}

void EraseFixupCaseThree(rbtree *tree,node_t **siblingNode,node_t **fixupNode,int position){
  switch (position)
  {
  case LEFT_CHILD:
    (*siblingNode)->left->color = RBTREE_BLACK;
    (*siblingNode)->color = RBTREE_RED;
    rotate_rbtree(tree,*siblingNode,ROTATE_RIGHT);
    *siblingNode = (*fixupNode)->parent->right;
    break;

  case RIGHT_CHILD:
    (*siblingNode)->right->color = RBTREE_BLACK;
    (*siblingNode)->color = RBTREE_RED;
    rotate_rbtree(tree,*siblingNode,ROTATE_LEFT);
    *siblingNode = (*fixupNode)->parent->left;
    break;
  
  default:
    printf("Wrong node position");
    break;
  }

  return;
}

void EraseFixupCaseFour(rbtree *tree,node_t **fixupNode,node_t **siblingNode,int position){
  (*siblingNode)->color = (*fixupNode)->parent->color;
  (*fixupNode)->parent->color = RBTREE_BLACK;

  switch (position)
  {
  case LEFT_CHILD:
    (*siblingNode)->right->color = RBTREE_BLACK;
    rotate_rbtree(tree,(*fixupNode)->parent,ROTATE_LEFT);
    break;

  case RIGHT_CHILD:
    (*siblingNode)->left->color = RBTREE_BLACK;
    rotate_rbtree(tree,(*fixupNode)->parent,ROTATE_RIGHT);
    break;
  
  default:
    break;
  }

  *fixupNode = tree->root;

  return;
}


int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  //중위순회로 arr에 넣는다
  inorder_traversal(t->root, t->nil, arr, 0);

  return 0;
}

int inorder_traversal(node_t *node, node_t *nil, int *arr, int index) {
    if (node == nil) {
        return index;
    }
    index = inorder_traversal(node->left, nil, arr, index);
    arr[index++] = node->key;
    index = inorder_traversal(node->right, nil, arr, index);
    return index;
}

void rotate_rbtree(rbtree *tree,node_t *rotateNode,int direction){
  //oppositeNode=>y,rotateNode=>x
    node_t *oppositeNode;

    switch (direction)
    {
    case ROTATE_LEFT:
      oppositeNode = rotateNode->right;
      rotateNode->right = oppositeNode->left;

      if(oppositeNode->left != tree->nil)
        oppositeNode->left->parent = rotateNode;
      
      oppositeNode->parent = rotateNode->parent;

      if(rotateNode->parent == tree->nil)
        tree->root = oppositeNode;
      else if (rotateNode == rotateNode->parent->left)
        rotateNode->parent->left = oppositeNode;
      else
        rotateNode->parent->right = oppositeNode;

      oppositeNode->left = rotateNode;
      rotateNode->parent = oppositeNode;
      
      break;

    case ROTATE_RIGHT:
      oppositeNode = rotateNode->left;
      rotateNode->left = oppositeNode->right;

      if(oppositeNode->right != tree->nil)
        oppositeNode->right->parent = rotateNode;
      
      oppositeNode->parent = rotateNode->parent;

      if(rotateNode->parent == tree->nil)
        tree->root = oppositeNode;
      else if (rotateNode == rotateNode->parent->right)
        rotateNode->parent->right = oppositeNode;
      else
        rotateNode->parent->left = oppositeNode;

      oppositeNode->right = rotateNode;
      rotateNode->parent = oppositeNode;

    break;
    
    default:
      printf("Wrong Direction!");
      break;
    }

    return 0;
}
