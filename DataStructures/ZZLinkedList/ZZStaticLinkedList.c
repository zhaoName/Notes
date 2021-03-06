//
//  ZZStaticLinkedList.c
//  ZZDataStructures
//
//  Created by zhaoName on 2019/11/21.
//  Copyright © 2019 XMB. All rights reserved.
//

#include "ZZStaticLinkedList.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

/// count of list
unsigned int _count = 0;
/// length of array
unsigned int _capacity = 0;

ZZStaticLinkedList* zz_init_staticLinkedList(unsigned int capacity)
{
    // the min capacity is 3
    ZZStaticLinkedList *list = malloc(sizeof(ZZStaticLinkedList) * (capacity >= 3 ? capacity : 3));
    if (list) {
        for (int i=0; i<capacity; i++) {
            list[i].cur = i+1;
        }
        // init list is null
        list[capacity-1].cur = 0;
        _capacity = capacity >= 3 ? capacity : 3;
    }
    return list;
}

#pragma mark -- CRDU

void zz_addNode(ZZStaticLinkedList *list, void *newData)
{
    assert(list);
    zz_insertNodeAtIndex(list, newData, _count);
}

/**
* insert node at index in static linked list
*
* @param index node position in the linked list, [0, _count-1] in list, [1, _capacity-2] in array
*/
void zz_insertNodeAtIndex(ZZStaticLinkedList *list, void *newData, unsigned int index)
{
    assert(list);
    if (index > _count || _count + 2 > _capacity) return;
    
    // get the index of the next node
    unsigned int nextIndex = list[0].cur;
    // record the cursor of the next node
    list[0].cur = list[nextIndex].cur;
    
    // create new node
    list[nextIndex].data = newData;
    
    // lookup the index of previous node that given index
    int preIndex = _capacity-1;
    for (int i=0; i<index; i++) {
        preIndex = list[preIndex].cur;
    }
    // insert node at index
    list[nextIndex].cur = list[preIndex].cur;
    list[preIndex].cur = nextIndex;
    _count++;
}

/**
* delete node at index in static linked list
*
* @param index node position in the linked list, [0, _count-1] in list, [1, _capacity-2] in array
*/
void zz_deleteNodeAtIndex(ZZStaticLinkedList *list, unsigned int index)
{
    assert(list);
    if (index >= _count) return;
    
    // lookup the index of previous node that given index
    unsigned int nextIndex = _capacity-1;
    for (int i=0; i<index; i++) {
        nextIndex = list[nextIndex].cur;
    }
    // lookup the index of the currnt node to be deleted
    unsigned int delIndex = list[nextIndex].cur;
    list[nextIndex].cur = list[delIndex].cur;
    // free del node
    list[delIndex].data = NULL;
    list[delIndex].cur = list[0].cur;
    // reset the index of next node
    list[0].cur = delIndex;
    _count--;
}

void zz_deleteNodeWithData(ZZStaticLinkedList *list, void *data)
{
    assert(list);
    
    unsigned int index = _capacity-1;
    while (index)
    {
        // lookup the index of previous node that given index
        if (list[list[index].cur].data == data) {
            unsigned int delIndex = list[index].cur;
            list[index].cur = list[delIndex].cur;
            list[delIndex].data = NULL;
            list[delIndex].cur = list[0].cur;
            list[0].cur = delIndex;
            _count--;
        }
        else {
            index = list[index].cur;
        }
    }
}

void zz_updateNodeAtIndex(ZZStaticLinkedList *list, void *newData, unsigned int index)
{
    assert(list);
    if (index > _count) return;
    
    // lookup the index of the currnt node in array
    unsigned int curIndex = _capacity-1;
    for (int i=0; i<=index; i++) {
        curIndex = list[curIndex].cur;
    }
    list[curIndex].data = newData;
}

void* zz_getNodeDataAtIndex(ZZStaticLinkedList *list, unsigned int index)
{
    assert(list);
    if (index > _count) return NULL;
    
    // lookup the index of the currnt node in array
    unsigned int curIndex = _capacity-1;
    for (int i=0; i<=index; i++) {
        curIndex = list[curIndex].cur;
    }
    return list[curIndex].data;
}

/**
 * find the index of node whose value is equal to given data
 * data == null or don't find return -1
 */
int zz_indexOfNode(ZZStaticLinkedList *list, void *data)
{
    assert(list);
    if (data == NULL) return -1;
    
    unsigned int curIndex = _capacity-1;
    while (curIndex) {
        curIndex = list[curIndex].cur;
        if (list[curIndex].data == data) {
            return curIndex;
        }
    }
    return -1;
}

#pragma mark -- clear

unsigned int zz_length_staticLinkedList(ZZStaticLinkedList *list)
{
    assert(list);
    unsigned int length = 0;
    unsigned int index = list[_capacity - 1].cur;
    while (index) {
        index = list[index].cur;
        length++;
    }
    return length;
    //return _count;
}

void zz_clear_staticLinkedList(ZZStaticLinkedList *list)
{
    assert(list);
    for (int i=0; i<_capacity; i++) {
        list[i].cur = i+1;
        list[i].data = NULL;
    }
    _count = 0;
    list[_capacity-1].cur = 0;
}

void zz_release_staticLinkedList(ZZStaticLinkedList **list)
{
    assert(*list);
    _count = 0;
    _capacity = 0;
    free(*list);
}

#pragma mark -- print


void zz_print_staticLinkedList(ZZStaticLinkedList *list)
{
    unsigned int index = list[_capacity-1].cur;
    for (int i=0; i<_count; i++) {
        printf("[%d-%d]->", *(int *)(list[index].data), list[index].cur);
        index = list[index].cur;
    }
    printf("null\n");
    
    printf("nextNode:%d--firstNode:%d\n", list[0].cur, list[_capacity-1].cur);
}

