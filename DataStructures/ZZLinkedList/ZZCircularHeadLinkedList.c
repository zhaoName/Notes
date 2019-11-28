//
//  ZZCircularHeadLinkedList.c
//  ZZDataStructures
//
//  Created by zhaoName on 2019/11/27.
//  Copyright © 2019 XMB. All rights reserved.
//  C 带头结点的单循环链表

#include "ZZCircularHeadLinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

ZZCircularHeadLinkedList* zz_init_circularLinkedList(void)
{
    ZZCircularHeadLinkedList *list = malloc(sizeof(ZZCircularHeadLinkedList));
    if (list) {
        list->count = 0;
        list->head = malloc(sizeof(ZZNode));
        assert(list->head);
        list->head->next = list->head;
        
        list->tail = malloc(sizeof(ZZNode));
        assert(list->tail);
        list->tail = list->head;
    }
    return list;
}

#pragma mark -- CRDU

void zz_addNode(ZZCircularHeadLinkedList *list, void *newData)
{
    assert(list);
    zz_insertNodeAtIndex(list, newData, list->count);
}

void zz_insertNodeAtIndex(ZZCircularHeadLinkedList *list, void *newData, unsigned int index)
{
    assert(index <= list->count);
    
    // create new node
    ZZNode *newNode = malloc(sizeof(ZZNode));
    newNode->data = newData;
    newNode->next = NULL;
    // lookup previous node that given index
    ZZNode *nd = list->head;
    for (int i=0; i<index; i++) {
        nd = nd->next;
    }
    // insert
    newNode->next = nd->next;
    nd->next = newNode;
    // reset tail
    if (index == list->count) {
        list->tail = newNode;
    }
    list->count++;
}

void zz_deleteNodeWithData(ZZCircularHeadLinkedList *list, void *data)
{
    assert(list);
    
    ZZNode *nd = list->head;
    for (int i=0; i<list->count; i++)
    {
        if (nd->next->data == data) {
            ZZNode *delNode = nd->next;
            nd->next = nd->next->next;
            free(delNode);
            // reset tail
            if (i == list->count-1) {
                list->tail = nd;
            }
            list->count--;
        }
        else {
            nd = nd->next;
        }
    }
}

void zz_deleteNodeAtIndex(ZZCircularHeadLinkedList *list, unsigned int index)
{
    assert(index < list->count);
    
    ZZNode *nd = list->head;
    for (int i=0; i<index; i++) {
        nd = nd->next;
    }
    
    ZZNode *delNode = nd->next;
    nd->next = nd->next->next;
    free(delNode);
    // reset tail
    if (index == list->count - 1) {
        list->tail = nd;
    }
    list->count--;
}

void zz_updateNodeAtIndex(ZZCircularHeadLinkedList *list, unsigned int index, void *newData)
{
    assert(index < list->count);
    
    ZZNode *nd = list->head->next;
    for (int i=0; i<index; i++) {
        nd = nd->next;
    }
    nd->data = newData;
}

void* zz_getNodeAtIndex(ZZCircularHeadLinkedList *list, unsigned int index)
{
    assert(index < list->count);
    
    ZZNode *nd = list->head->next;
    for (int i=0; i<index; i++) {
        nd = nd->next;
    }
    return nd->data;
}

#pragma mark -- clear

unsigned int zz_length_circularLinkedList(ZZCircularHeadLinkedList *list)
{
    assert(list);
    return list->count;
}

void zz_clear_circularLinkedList(ZZCircularHeadLinkedList *list)
{
    assert(list);
    ZZNode *nd = list->head->next;
    while (nd != list->head) {
        list->head->next = nd->next;
        free(nd);
        nd = list->head->next;
    }
    list->tail = list->head;
    list->count = 0;
}

void zz_release_circularLinkedList(ZZCircularHeadLinkedList **list)
{
    assert(*list);
    ZZNode *nd = (*list)->head->next;
    while (nd != (*list)->head) {
        (*list)->head->next = nd->next;
        free(nd);
        nd = (*list)->head->next;
    }
    (*list)->tail = NULL;
    free((*list)->head);
    (*list)->head = NULL;
    (*list)->count = 0;
    free((*list));
    (*list) = NULL;
}


#pragma mark -- print

void zz_print_circularLinkedList(ZZCircularHeadLinkedList *list)
{
    assert(list);
    
    ZZNode *nd = list->head->next;
    while (nd != list->head) {
        int *ele = (int *)nd->data;
        printf("%d->", *ele);
        nd = nd->next;
    }
    printf("head\n");
}
