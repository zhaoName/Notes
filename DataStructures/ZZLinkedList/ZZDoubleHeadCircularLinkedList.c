//
//  ZZDoubleHeadCircularLinkedList.c
//  ZZDataStructures
//
//  Created by zhaoName on 2019/12/2.
//  Copyright © 2019 XMB. All rights reserved.
//  C 带头节点双向循环链表

#include "ZZDoubleHeadCircularLinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


ZZDHCircularLinkedList* zz_init_dhCircularLinkedList(void)
{
    ZZDHCircularLinkedList *list = malloc(sizeof(ZZDHCircularLinkedList));
    if (list) {
        list->count = 0;
        list->head = malloc(sizeof(ZZNode));
        assert(list->head);
        list->head->next = list->head;
        list->head->prev = list->head;
    }
    return list;
}

#pragma mark -- CRUD

void zz_addNode(ZZDHCircularLinkedList *list, void *newData)
{
    zz_insertNodeAtIndex(list, newData, list->count);
}

void zz_insertNodeAtIndex(ZZDHCircularLinkedList *list, void *newData, unsigned int index)
{
    assert(index <= list->count);
    // create new node
    ZZNode *newNode = malloc(sizeof(ZZNode));
    newNode->data = newData;
    newNode->next = NULL;
    newNode->prev = NULL;
    // lookup previcous node that given index
    ZZNode *nd = list->head;
    for (int i=0; i<index; i++) {
        nd = nd->next;
    }
    // insert
    nd->next->prev = newNode;
    newNode->prev = nd;
    newNode->next = nd->next;
    nd->next = newNode;
    
    list->count++;
}

void zz_deleteNodeAtIndex(ZZDHCircularLinkedList *list, unsigned int index)
{
    assert(index < list->count);
    
    ZZNode *nd = list->head;
    for (int i=0; i<index; i++) {
        nd = nd->next;
    }
    
    ZZNode *delNode = nd->next;
    nd->next->next->prev = nd;
    nd->next = nd->next->next;
    free(delNode);
    delNode = NULL;
    
    list->count--;
}

void zz_deleteNodeWithData(ZZDHCircularLinkedList *list, void *data)
{
    assert(list);
    
    ZZNode *nd = list->head;
    for (int i=0; i<=list->count; i++)
    {
        if (nd->next->data == data) {
            ZZNode *delNode = nd->next;
            nd->next->next->prev = nd;
            nd->next = nd->next->next;
            free(delNode);
            delNode = NULL;
            list->count--;
        }
        else {
            nd = nd->next;
        }
    }
}

void zz_updateNodeAtIndex(ZZDHCircularLinkedList *list, void *newData, unsigned int index)
{
    assert(index < list->count);
    
    if (index == list->count - 1) {
        list->head->prev->data = newData;
        return;
    }
    
    ZZNode *nd = list->head->next;
    for (int i=0; i<index; i++) {
        nd = nd->next;
    }
    nd->data = newData;
}

void* zz_getNodeAtIndex(ZZDHCircularLinkedList *list, unsigned int index)
{
    assert(index < list->count);
    
    if (index == list->count - 1) {
        return list->head->prev->data;
    }
    
    ZZNode *nd = list->head->next;
    for (int i=0; i<index; i++) {
        nd = nd->next;
    }
    return nd->data;
}

/// find the index of node whose value is equal to given data
/// data == null or don't find return -1
int zz_indexOfData(ZZDHCircularLinkedList *list, void *data)
{
    assert(list);
    if (data == NULL) return -1;
    
    ZZNode *nd = list->head->next;
    for (int i=0; i<list->count; i++)
    {
        if (nd->data == data) {
            return i;
        } else {
            nd = nd->next;
        }
    }
    return -1;
}


#pragma mark -- clear

unsigned int zz_length_dhCircularLinkedList(ZZDHCircularLinkedList *list)
{
    assert(list);
    return list->count;
}

void zz_clear_dhCircularLinkedList(ZZDHCircularLinkedList *list)
{
    assert(list);
    
    ZZNode *nd = list->head->next;
    while (nd != list->head) {
        // point to next node
        nd->next->prev = list->head;
        list->head->next = nd->next;
        // free current node
        free(nd);
        // reset nd
        nd = list->head->next;
    }
    list->count = 0;
}

void zz_release__dhCircularLinkedList(ZZDHCircularLinkedList *list)
{
    assert(list);
    
    ZZNode *nd = list->head->next;
    while (nd != list->head) {
        // point to next node
        nd->next->prev = list->head;
        list->head->next = nd->next;
        // free current node
        free(nd);
        // reset nd
        nd = list->head->next;
    }
    free(list->head);
    list->head = NULL;
    list->count = 0;
    free(list);
}

#pragma mark -- print

void zz_print_dhCircularLinkedList(ZZDHCircularLinkedList *list)
{
    assert(list);
    ZZNode *nd = list->head->next;
    while (nd != list->head) {
        printf("%d->", *(int *)(nd->data));
        nd = nd->next;
    }
    printf("head\n");
}

void zz_reverse_print_dhCircularLinkedList(ZZDHCircularLinkedList *list)
{
    assert(list);
    ZZNode *nd = list->head->prev;
    while (nd != list->head) {
        printf("%d->", *(int *)(nd->data));
        nd = nd->prev;
    }
    printf("tail\n");
}
