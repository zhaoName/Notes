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
    assert(index < list->count);
    
    ZZNode *nd = list->head;
    for (int i=0; i<index; i++) {
        nd = nd->next;
    }
    ZZNode *delNode = nd->next;
    if (index == list->count - 1) {
        nd->next = NULL;
        list->tail = nd;
    }
    else {
        nd->next->next->prev = nd;
        nd->next = nd->next->next;
    }
    delNode = NULL;
    free(delNode);
    list->count--;
}

void zz_deleteNodeWithData(ZZDoubleHeadLinkedList *list, void *data)
{
    assert(list);
    
    ZZNode *nd = list->head;
    for (int i=0; i<=list->count; i++)
    {
        if (nd->next->data == data) {
            ZZNode *delNode = nd->next;
            if (i == list->count) {
                nd->next = NULL;
                list->tail = nd;
            }
            else {
                nd->next->next->prev = nd;
                nd->next = nd->next->next;
            }
            delNode = NULL;
            free(delNode);
            list->count--;
        }
        else {
            nd = nd->next;
        }
    }
}

void* zz_getNodeAtIndex(ZZDoubleHeadLinkedList *list, unsigned int index)
{
    assert(index < list->count);
    
    if (index == list->count - 1) {
        return list->tail->data;
    }
    ZZNode *nd = list->head->next;
    for (int i=0; i<index; i++) {
        nd = nd->next;
    }
    return nd->data;
}

void zz_updateNodeAtInde(ZZDoubleHeadLinkedList *list, void *newData, unsigned int index)
{
    assert(index < list->count);
    
    if (index == list->count - 1) {
        list->tail->data = newData;
        return;
    }
    ZZNode *nd = list->head->next;
    for (int i=0; i<index; i++) {
        nd = nd->next;
    }
    nd->data = newData;
}


#pragma mark -- clear

/// find the index of node whose value is equal to given data
/// data == null or don't find return -1
int zz_indexOfData_doubleLinkedList(ZZDoubleHeadLinkedList *list, void *data)
{
    assert(list);
    if (data == NULL) return -1;
    
    ZZNode *nd = list->head->next;
    for (int i=0; i<list->count; i++) {
        if (nd->data == data) {
            return i;
        } else {
            nd = nd->next;
        }
    }
    return -1;
}

unsigned int zz_lenght_doubleLinkedList(ZZDoubleHeadLinkedList *list)
{
    assert(list);
    return list->count;
}

void zz_clear_doubleLinkedList(ZZDoubleHeadLinkedList *list)
{
    assert(list);
    ZZNode *nd = list->head;
    for (int i=0; i<list->count; i++) {
        ZZNode *delNode = nd->next;
        if (i == list->count -1) {
            nd->next = NULL;
            list->tail = nd;
        }
        else {
            nd->next->next->prev = nd;
            nd->next = nd->next->next;
        }
        delNode = NULL;
        free(delNode);
        list->count--;
    }
    list->head->next = NULL;
    list->tail = NULL;
    list->count = 0;
}

void zz_release_doubleLinkedList(ZZDoubleHeadLinkedList **list)
{
    assert(*list);
    assert(*list);
    ZZNode *nd = (*list)->head;
    for (int i=0; i<=(*list)->count; i++) {
        ZZNode *delNode = nd->next;
        if (i == (*list)->count -1) {
            nd->next = NULL;
            (*list)->tail = nd;
        }
        else {
            nd->next->next->prev = nd;
            nd->next = nd->next->next;
        }
        delNode = NULL;
        free(delNode);
        (*list)->count--;
    }
    free((*list)->head);
    (*list)->head = NULL;
    //free((*list)->tail);
    (*list)->tail = NULL;
    (*list)->count = 0;
    free(*list);
}


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

void zz_reverse_print_doubleLinkedList(ZZDoubleHeadLinkedList *list)
{
    assert(list);
    ZZNode *nd = list->tail;
    while (nd != list->head) {
        int *ele = (int *)nd->data;
        printf("%d->", *ele);
        nd = nd->prev;
    }
    printf("head\n");
}
