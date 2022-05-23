//
//  ZZDoubleHeadLinkedList.h
//  ZZDataStructures
//
//  Created by zhaoName on 2019/11/28.
//  Copyright © 2019 XMB. All rights reserved.
//  C 带头结点双向链表

#ifndef ZZDoubleHeadLinkedList_h
#define ZZDoubleHeadLinkedList_h

typedef struct ZZNode {
    void *data;
    struct ZZNode *next;
    struct ZZNode *prev;
}ZZNode;

typedef struct DoubleLinkedList {
    unsigned int count;
    ZZNode *head;
    // tail pointer, point to the last node
    ZZNode *tail;
}ZZDoubleHeadLinkedList;


ZZDoubleHeadLinkedList* zz_init_doubleLinkedList(void);


void zz_addNode(ZZDoubleHeadLinkedList *list, void *newData);
void zz_insertNodeAtIndex(ZZDoubleHeadLinkedList *list, void *newData, unsigned int index);

void zz_deleteNodeAtIndex(ZZDoubleHeadLinkedList *list, unsigned int index);
void zz_deleteNodeWithData(ZZDoubleHeadLinkedList *list, void *data);

void* zz_getNodeAtIndex(ZZDoubleHeadLinkedList *list, unsigned int index);
void zz_updateNodeAtInde(ZZDoubleHeadLinkedList *list, void *newData, unsigned int index);

/// find the index of node whose value is equal to given data
/// data == null or don't find return -1
int zz_indexOfData_doubleLinkedList(ZZDoubleHeadLinkedList *list, void *data);
/// count of list
unsigned int zz_lenght_doubleLinkedList(ZZDoubleHeadLinkedList *list);
void zz_clear_doubleLinkedList(ZZDoubleHeadLinkedList *list);
void zz_release_doubleLinkedList(ZZDoubleHeadLinkedList **list);


void zz_print_doubleLinkedList(ZZDoubleHeadLinkedList *list);
void zz_reverse_print_doubleLinkedList(ZZDoubleHeadLinkedList *list);

#endif /* ZZDoubleHeadLinkedList_h */
