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


void zz_addNode(ZZLinkedList *list, void* data);
void zz_insertNodeAtIndex(ZZLinkedList *list, void* data, unsigned int index);


void zz_deleteNodeWithData(ZZLinkedList *list, void *data);
void zz_deleteNodeAtIndex(ZZLinkedList *list, unsigned int index);
void zz_updateNodeAtIndex(ZZLinkedList *list, unsigned int index, void* newData);
void* zz_getNodeAtIndex(ZZLinkedList *list, unsigned int index);


unsigned int zz_length_linkedList(void);
unsigned int zz_isEmpty(ZZLinkedList *list);
void zz_clear_linkedList(ZZLinkedList *list);
void zz_relsese_linkedList(ZZLinkedList *list);

void zz_print_linkedList(ZZLinkedList *list);

#endif /* ZZLinkedList_h */
