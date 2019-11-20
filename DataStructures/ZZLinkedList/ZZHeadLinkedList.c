//
//  ZZHeadLinkedList.c
//  ZZDataStructures
//
//  Created by zhaoName on 2019/11/19.
//  Copyright © 2019 XMB. All rights reserved.
//  C 单链表 带头节点

#define ZZHeadLinkedListCompileDefine
typedef struct ZZNode{
    void *data;
    struct ZZNode *next;
}Node;

typedef struct ZZHeadLinkedList{
    unsigned int count;
    Node *head;
}ZZHeadLinkedList;

#include "ZZHeadLinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#pragma mark -- init
ZZHeadLinkedList* zz_init_headLinkedList(void)
{
    ZZHeadLinkedList *list = malloc(sizeof(ZZHeadLinkedList));
    assert(list);
    list->count = 0;
    list->head = malloc(sizeof(Node));
    list->head->next = NULL;
    return list;
}

#pragma mark -- CRDU

void zz_addNode(ZZHeadLinkedList *list, void *data)
{
    assert(list);
    zz_insertNodeAtIndex(list, data, list->count);
}

void zz_insertNodeAtIndex(ZZHeadLinkedList *list, void *data, unsigned int index)
{
    assert(index <= list->count);
    // create new node
    Node *newNode = malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    
    // lookup previous node that given ndoe
    Node *nd = list->head;
    unsigned int i = 0;
    while (nd && i < index) {
        nd = nd->next;
        i++;
    }
    // insert node
    newNode->next = nd->next;
    nd->next = newNode;
    list->count++;
}

void zz_deleteNode(ZZHeadLinkedList *list, void *data)
{
    assert(list);
    Node *nd = list->head;
    while (nd->next)
    {
        if (nd->next->data == data) {
            Node *delNode = nd->next;
            nd->next = nd->next->next;
            free(delNode);
            list->count--;
        }
        else {
            nd = nd->next;
        }
    }
}

void zz_deleteNodeAtIndex(ZZHeadLinkedList *list, unsigned int index)
{
    assert(index < list->count);
    
    Node *nd = list->head;
    for (int i=0; i<index; i++) {
        nd = nd->next;
    }
    Node *delNode = nd->next;
    nd->next = nd->next->next;
    free(delNode);
    list->count--;
}

void zz_updateNodeAtIndex(ZZHeadLinkedList *list, unsigned int index, void *newData)
{
    assert(index < list->count);
    Node *nd = list->head->next;
    for (int i=0; i<index; i++) {
        nd = nd->next;
    }
    nd->data = newData;
}

void* zz_getNodeAtIndex(ZZHeadLinkedList *list, unsigned int index)
{
    assert(index < list->count);
    Node *nd = list->head->next;
    for (int i=0; i<index; i++) {
        nd = nd->next;
    }
    return nd->data;
}

#pragma mark -- extend

unsigned int zz_containsNode(ZZHeadLinkedList *list, void *data)
{
    assert(list);
    Node *nd = list->head->next;
    while (nd) {
        if (nd->data == data) {
            return 1;
        }
        nd = nd->next;
    }
    return 0;
}

unsigned int zz_length_headLinkedList(ZZHeadLinkedList *list)
{
    assert(list);
    return list->count;
}

unsigned int zz_isEmpty(ZZHeadLinkedList *list)
{
    assert(list);
    if (list->count == 0) return 1;
    else return 0;
}

void zz_clear_headLinkedList(ZZHeadLinkedList *list)
{
    assert(list);
    Node *nd = list->head->next;
    while (nd) {
        Node *delNode = nd;
        nd = nd->next;
        free(delNode);
    }
    list->head->next = NULL;
    list->count = 0;
}

void zz_release_headLinkedList(ZZHeadLinkedList **list)
{
    assert(*list);
    Node *nd = (*list)->head;
    while (nd) {
        Node *delNode = nd;
        nd = nd->next;
        free(delNode);
    }
    (*list)->count = 0;
    *list = NULL;
}

#pragma mark -- print

void zz_print_headLinkedList(ZZHeadLinkedList *list)
{
    assert(list);
    
    Node *nd = list->head->next;
    while (nd) {
        int *ele = (int *)nd->data;
        printf("%d->", *ele);
        nd = nd->next;
    }
    printf("null\n");
}
