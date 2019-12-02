//
//  ZZHeadLinkedList.h
//  ZZDataStructures
//
//  Created by zhaoName on 2019/11/19.
//  Copyright © 2019 XMB. All rights reserved.
//  C 单链表 带头节点

#ifndef ZZHeadLinkedList_h
#define ZZHeadLinkedList_h

#ifndef ZZHeadLinkedListCompileDefine
typedef void ZZHeadLinkedList;
#endif


ZZHeadLinkedList* zz_init_headLinkedList(void);


void zz_addNode(ZZHeadLinkedList *list, void *data);
void zz_insertNodeAtIndex(ZZHeadLinkedList *list, void *data, unsigned int index);
void zz_deleteNode(ZZHeadLinkedList *list, void *data);
void zz_deleteNodeAtIndex(ZZHeadLinkedList *list, unsigned int index);
void zz_updateNodeAtIndex(ZZHeadLinkedList *list, unsigned int index, void *newData);
void* zz_getNodeAtIndex(ZZHeadLinkedList *list, unsigned int index);


unsigned int zz_containsNode(ZZHeadLinkedList *list, void *data);


unsigned int zz_length_headLinkedList(ZZHeadLinkedList *list);
unsigned int zz_isEmpty(ZZHeadLinkedList *list);
void zz_clear_headLinkedList(ZZHeadLinkedList *list);
void zz_release_headLinkedList(ZZHeadLinkedList **list);

ZZHeadLinkedList* zz_reverse_headLinkedList(ZZHeadLinkedList *list);

void zz_print_headLinkedList(ZZHeadLinkedList *list);

#endif /* ZZHeadLinkedList_h */
