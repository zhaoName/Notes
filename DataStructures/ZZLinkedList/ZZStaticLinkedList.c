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

unsigned int count = 0;

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
    }
    return list;
}


void addNode(ZZStaticLinkedList *list, void *newData)
{
    assert(list);
    zz_insertNodeAtIndex(list, newData, count);
}

void zz_insertNodeAtIndex(ZZStaticLinkedList *list, void *newData, unsigned int index)
{
    assert(list);
    // get the cursor of the next node
    unsigned int nextIndex = list[0].cur;
    
    for (int i=0; i<index; i++) {
        int k = list[i].cur;
    }
    list[nextIndex].cur = 0;
    list[nextIndex].data = newData;
    if (index == 0) {
        list[nextIndex].cur = list[count+1].cur;
        list[count+1].cur = nextIndex;
    }
    // record the cursor of the next node
    list[0].cur = list[nextIndex].cur;
    count++;
}


#pragma mark -- priate method



