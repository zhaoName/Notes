//
//  ZZLinkedList.h
//  ZZDataStructures
//
//  Created by zhao on 2019/11/15.
//  Copyright © 2019 XMB. All rights reserved.
//  C 单链表 无头结点

#ifndef ZZLinkedList_h
#define ZZLinkedList_h


typedef struct ZZNode{
    void *data;
    struct ZZNode *next;
}Node;
typedef struct ZZNode ZZLinkedList;


ZZLinkedList* zz_init_linkedList(void);


/// insert node at tail
void zz_addNode(ZZLinkedList *list, void* data);

/**
 * insert node at given index
 *
 * if index == 0 you must need to return modified node
 */
ZZLinkedList* zz_insertNodeAtIndex(ZZLinkedList *list, void* data, unsigned int index);


/**
 * delete node that node value is equal to given data
 *
 * if the node to be deleted is first node, then you must return modified list
 * otherwise the modified list will only take effect inside the function.
 */
ZZLinkedList* zz_deleteNodeWithData(ZZLinkedList *list, void *data);
ZZLinkedList* zz_deleteNodeAtIndex(ZZLinkedList *list, unsigned int index);
void zz_updateNodeAtIndex(ZZLinkedList *list, unsigned int index, void* newData);
void* zz_getNodeAtIndex(ZZLinkedList *list, unsigned int index);


unsigned int zz_length_linkedList(ZZLinkedList *list);
unsigned int zz_isEmpty(ZZLinkedList *list);
void zz_clear_linkedList(ZZLinkedList *list);
void zz_relsese_linkedList(ZZLinkedList **list);


void zz_print_linkedList(ZZLinkedList *list);

#endif /* ZZLinkedList_h */
