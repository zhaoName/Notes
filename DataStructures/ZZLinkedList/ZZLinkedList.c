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

/// linked list count
unsigned int count = 0;

#pragma mark -- Init

ZZLinkedList* zz_init_linkedList(void)
{
    ZZLinkedList *list = malloc(sizeof(ZZLinkedList));
    assert(list);
    list->next = NULL;
    count = 0;
    return list;
}


#pragma mark -- CRDU

void zz_addNode(ZZLinkedList *list, void* data)
{
    assert(list);
    zz_insertNodeAtIndex(list, data, count);
}

void zz_insertNodeAtIndex(ZZLinkedList *list, void* data, unsigned int index)
{
    assert(list);
    assert(index <= count);
    
    // create new node
    Node *node = malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    
    if (index == 0) {
        // first node
        if (count == 0) {
            list->data = node->data;
            free(node);
        }
        else {
            // record old list
            Node *nd = list;
            // reset list
            list = node;
            // splice old list
            list->next = nd;
        }
    }
    else {
        Node *nd = list;
        for (int i=1; i<index; i++) {
            nd = nd->next;
        }
        // nd is node that given index previous
        node->next = nd->next;
        nd->next = node;
    }
    count++;
}


void zz_deleteNodeWithData(ZZLinkedList *list, void *data)
{
    assert(list);
    
    if (list->data == data) {
        // delete first node
        Node *nd = list;
        list = list->next;
        free(nd);
        count--;
    }
    else {
        Node *nd = list;
        // count limit range, no need to judge node->next is null
        for (int i=0; i<count-1; i++)  {
            if (nd->next->data == data) {
                nd->next = nd->next->next;
                free(nd->next);
                count--;
            }
            else {
                nd = nd->next;
            }
        }
    }
}

void zz_deleteNodeAtIndex(ZZLinkedList *list, unsigned int index)
{
    assert(list);
    // out of range
    if (index >= count) return;
    
    // delete first node requires special handle
    if (index == 0) {
        Node *nd = list;
        list = list->next;
        free(nd);
        count--;
    }
    else {
        Node *nd = list;
        unsigned int i = 1;
        // lookup node before index if existed
        while (nd && i<index) {
            nd = nd->next;
            i++;
        }
        // nd->next is node at index
        if (nd->next) {
            nd->next = nd->next->next;
            free(nd->next);
            count--;
        }
    }
}

void zz_updateNodeAtIndex(ZZLinkedList *list, unsigned int index, void* newData)
{
    assert(list);
    // out of range
    if (index >= count) return;
    
    Node *nd = list;
    for (unsigned int i=0; i<index; i++) {
        nd = nd->next;
    }
    nd->data = newData;
}

void* zz_getNodeAtIndex(ZZLinkedList *list, unsigned int index)
{
    assert(list);
    // out of range
    if (index > count) return NULL;
    
    Node *nd = list;
    for (unsigned int i=0; i<index; i++) {
        nd = nd->next;
    }
    return nd->data;
}


#pragma mark --

unsigned int zz_length_linkedList(void)
{
    return count;
}

unsigned int zz_isEmpty(ZZLinkedList *list)
{
    assert(list);
    return count == 0;
}

void zz_clear_linkedList(ZZLinkedList *list)
{
    assert(list);
    Node *nd = list->next;
    while (nd) {
        list->next = list->next->next;
        free(nd);
        nd = list->next;
    }
    count = 0;
    // set first node data is null. first node next was null
    list->data = NULL;
}

void zz_relsese_linkedList(ZZLinkedList *list)
{
    assert(list);
    Node *nd = list->next;
    while (nd) {
        list->next = list->next->next;
        free(nd);
        nd = list->next;
    }
    count = 0;
    // free first node
    free(list);
}


#pragma mark -- print

void zz_print_linkedList(ZZLinkedList *list)
{
    printf("[");
    Node *nd = list;
    while (nd) {
        int *ele = (int *)nd->data;
        printf("%d, ", *ele);
        nd = nd->next;
    }
    printf("]\n");
}

