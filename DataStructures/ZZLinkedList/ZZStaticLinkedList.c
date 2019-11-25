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
unsigned int _capaticy = 0;

ZZStaticLinkedList* zz_init_staticLinkedList(unsigned int capacity)
{
    // the mix capacity is 3
    ZZStaticLinkedList *list = malloc(sizeof(ZZStaticLinkedList) * (capacity >= 3 ? capacity : 3));
    if (list) {
        for (int i=0; i<capacity; i++) {
            list[i].cur = i+1;
        }
        // init list is null
        list[capacity-1].cur = 0;
        _capaticy = capacity >= 3 ? capacity : 3;
    }
    return list;
}


void zz_addNode(ZZStaticLinkedList *list, void *newData)
{
    assert(list);
    zz_insertNodeAtIndex(list, newData, _count);
}

/**
* insert node at index in static linked list
*
* @param index position of node, [1, _capacity-1] in array, [0, _capacity-1] in list
*/
void zz_insertNodeAtIndex(ZZStaticLinkedList *list, void *newData, unsigned int index)
{
    assert(list);
    if (index > _count) return;
    
    // get the index of the next node
    unsigned int nextIndex = list[0].cur;
    // record the cursor of the next node
    list[0].cur = list[nextIndex].cur;
    // create new node
    list[nextIndex].cur = 0;
    list[nextIndex].data = newData;
    // lookup the index of previous node that given index
    int preIndex = list[_capaticy-1].cur;
    for (int i=0; i<index; i++) {
        preIndex = list[preIndex].cur;
    }
    // insert node at index
    if (index != 0 || index != _count) {
        list[nextIndex].cur = list[preIndex].cur;
        list[preIndex].cur = nextIndex;
    }
    // reset the cursor of last element in array
    if (index == 0) {
        list[_capaticy-1].cur = nextIndex;
    }
    _count++;
}


void zz_deleteNodeAtIndex(ZZStaticLinkedList *list, unsigned int index)
{
    assert(list);
    
}




void zz_print_staticLinkedList(ZZStaticLinkedList *list)
{
    unsigned int index = list[_capaticy-1].cur;
    for (int i=0; i<_count; i++) {
        printf("[%d-%d]->", *(int *)(list[index].data), index);
        index = list[index].cur;
    }
    printf("null\n");
    
    printf("nextNode:%d--firstNode:%d\n", list[0].cur, list[_capaticy-1].cur);
}

#pragma mark -- priate method

