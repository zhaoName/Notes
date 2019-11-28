//
//  ZZCircularHeadLinkedList.h
//  ZZDataStructures
//
//  Created by zhaoName on 2019/11/27.
//  Copyright © 2019 XMB. All rights reserved.
//  C 带头结点的单循环链表

#ifndef ZZCircularHeadLinkedList_h
#define ZZCircularHeadLinkedList_h

typedef struct ZZNode {
    void *data;
    struct ZZNode *next;
}ZZNode;

typedef struct {
    unsigned int count;
    ZZNode *head;
    ZZNode *tail;
}ZZCircularHeadLinkedList;


ZZCircularHeadLinkedList* zz_init_circularLinkedList(void);

void zz_addNode(ZZCircularHeadLinkedList *list, void *newData);
void zz_insertNodeAtIndex(ZZCircularHeadLinkedList *list, void *newData, unsigned int index);

void zz_deleteNodeWithData(ZZCircularHeadLinkedList *list, void *data);
void zz_deleteNodeAtIndex(ZZCircularHeadLinkedList *list, unsigned int index);

void zz_updateNodeAtIndex(ZZCircularHeadLinkedList *list, unsigned int index, void *newData);
void* zz_getNodeAtIndex(ZZCircularHeadLinkedList *list, unsigned int index);


unsigned int zz_length_circularLinkedList(ZZCircularHeadLinkedList *list);
void zz_clear_circularLinkedList(ZZCircularHeadLinkedList *list);
void zz_release_circularLinkedList(ZZCircularHeadLinkedList **list);


void zz_print_circularLinkedList(ZZCircularHeadLinkedList *list);

#endif /* ZZCircularHeadLinkedList_h */
