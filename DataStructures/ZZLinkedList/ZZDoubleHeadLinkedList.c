//
//  ZZDoubleHeadLinkedList.c
//  ZZDataStructures
//
//  Created by zhaoName on 2019/11/28.
//  Copyright © 2019 XMB. All rights reserved.
//  C 带头结点双向链表

#include "ZZDoubleHeadLinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

ZZDoubleHeadLinkedList* zz_init_doubleLinkedList(void)
{
    ZZDoubleHeadLinkedList *list = malloc(sizeof(ZZDoubleHeadLinkedList));
    if (list) {
        list->count = 0;
        list->head = malloc(sizeof(ZZNode));
        assert(list->head);
        list->head->next = NULL;
        
        // the head node's tail point to the last node
        list->tail = malloc(sizeof(ZZNode));
        assert(list->tail);
        list->tail = NULL;
    }
    return list;
}

#pragma mark -- CRDU

void zz_addNode(ZZDoubleHeadLinkedList *list, void *newData)
{
    assert(list);
    zz_insertNodeAtIndex(list, newData, list->count);
}

void zz_insertNodeAtIndex(ZZDoubleHeadLinkedList *list, void *newData, unsigned int index)
{
    assert(index <= list->count);
    // create new node
    ZZNode *newNode = malloc(sizeof(ZZNode));
    newNode->data = newData;
    newNode->next = NULL;
    newNode->prev = NULL;
    // lookup previous node that given index
    ZZNode *nd = list->head;
    for (int i=0; i<index; i++) {
        nd = nd->next;
    }
    // insert node
    // first node and last node
    if (index == list->count) {
        nd->next = newNode;
        newNode->prev = nd;
    }
    else {
        newNode->prev = nd;
        newNode->next = nd->next;
        nd->next->prev = newNode;
        nd->next = newNode;
    }
    // reset tail
    if (index == list->count) {
        list->tail = newNode;
    }
    list->count++;
}

void zz_deleteNodeAtIndex(ZZDoubleHeadLinkedList *list, unsigned int index)
{
    
}

void zz_deleteNodeWithData(ZZDoubleHeadLinkedList *list, void *data)
{
    
}

void* zz_getNodeAtIndex(ZZDoubleHeadLinkedList *list, unsigned int index);
void zz_updateNodeAtInde(ZZDoubleHeadLinkedList *list, void *newData, unsigned int index);


#pragma mark -- clear

unsigned int zz_lenght_doubleLinkedList(ZZDoubleHeadLinkedList *list);
void zz_clear_doubleLinkedList(ZZDoubleHeadLinkedList *list);
void zz_release_doubleLinkedList(ZZDoubleHeadLinkedList *list);


#pragma mark -- print

void zz_print_doubleLinkedList(ZZDoubleHeadLinkedList *list)
{
    assert(list);
    
    ZZNode *nd = list->head->next;
    while (nd) {
        int *ele = (int *)nd->data;
        printf("%d->", *ele);
        nd = nd->next;
    }
    printf("null\n");
    
}
