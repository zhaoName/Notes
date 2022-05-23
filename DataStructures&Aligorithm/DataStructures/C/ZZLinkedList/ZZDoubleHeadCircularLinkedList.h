//
//  ZZDoubleHeadCircularLinkedList.h
//  ZZDataStructures
//
//  Created by zhaoName on 2019/12/2.
//  Copyright © 2019 XMB. All rights reserved.
//  C 带头节点双向循环链表

#ifndef ZZDoubleHeadCircularLinkedList_h
#define ZZDoubleHeadCircularLinkedList_h

typedef struct ZZNode {
    void *data;
    struct ZZNode *next;
    struct ZZNode *prev;
}ZZNode;

typedef struct ZZDHCircularLinkedList {
    unsigned int count;
    ZZNode *head;
}ZZDHCircularLinkedList;



ZZDHCircularLinkedList* zz_init_dhCircularLinkedList(void);


void zz_addNode(ZZDHCircularLinkedList *list, void *newData);
void zz_insertNodeAtIndex(ZZDHCircularLinkedList *list, void *newData, unsigned int index);

void zz_deleteNodeAtIndex(ZZDHCircularLinkedList *list, unsigned int index);
void zz_deleteNodeWithData(ZZDHCircularLinkedList *list, void *data);

void zz_updateNodeAtIndex(ZZDHCircularLinkedList *list, void *newData, unsigned int index);
void* zz_getNodeAtIndex(ZZDHCircularLinkedList *list, unsigned int index);
/// find the index of node whose value is equal to given data
/// data == null or don't find return -1
int zz_indexOfData(ZZDHCircularLinkedList *list, void *data);


unsigned int zz_length_dhCircularLinkedList(ZZDHCircularLinkedList *list);
void zz_clear_dhCircularLinkedList(ZZDHCircularLinkedList *list);
void zz_release__dhCircularLinkedList(ZZDHCircularLinkedList **list);


void zz_print_dhCircularLinkedList(ZZDHCircularLinkedList *list);
void zz_reverse_print_dhCircularLinkedList(ZZDHCircularLinkedList *list);


#endif /* ZZDoubleHeadCircularLinkedList_h */
