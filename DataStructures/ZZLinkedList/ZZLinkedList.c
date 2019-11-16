//
//  ZZLinkedList.c
//  ZZDataStructures
//
//  Created by zhao on 2019/11/15.
//  Copyright © 2019 XMB. All rights reserved.
//  C 单链表 无头结点

#include "ZZLinkedList.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


ZZLinkedList* zz_init_linkedList(void)
{
    ZZLinkedList *list = malloc(sizeof(ZZLinkedList));
    assert(list);
    list->next = NULL;
    return list;
}

unsigned int zz_length_linkedList(void)
{
    return 1;
}


unsigned int zz_isEmpty(ZZLinkedList *list);
void zz_clear_linkedList(ZZLinkedList *list);
void zz_relsese_linkedList(ZZLinkedList *list);


void zz_addNode(ZZLinkedList *list, Node *node)
{
    
}

void zz_insertNodeAtIndex(ZZLinkedList *list, Node *node, unsigned int index);


void zz_deleteNode(ZZLinkedList *list, Node *node);
void zz_deleteNodeAtIndex(ZZLinkedList *list, unsigned int index);

void zz_updateNodeAtIndex(ZZLinkedList *list, unsigned int index);

void zz_getNodeAtIndex(ZZLinkedList *list, unsigned int index);

