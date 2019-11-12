//
//  ZZArrayList.c
//  ZZDataStructures
//
//  Created by zhaoName on 2019/11/12.
//  Copyright © 2019 XMB. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include "ZZArrayList.h"


#pragma mark -- init

ZZArrayList* zz_init_arrylist(unsigned int capacity)
{
    ZZArrayList *list = malloc(sizeof(ZZArrayList));
    if (list) {
        list->capacity = capacity;
        list->count = 0;
        list->value = malloc(sizeof(void *) * capacity);
    }
    return list;
}

/// length of list
unsigned int zz_lengthOfArrayList(ZZArrayList *list)
{
    if (list == NULL) return 0;
    return list->count;
}

#pragma mark -- CRUD

/// insert element at last
void zz_insertElem(ZZArrayList *list, void *elem)
{
    if (list == NULL || elem == NULL) return;
    zz_insertElemAtIndex(list, elem, list->count);
}

/// insert element at index
void zz_insertElemAtIndex(ZZArrayList *list, void *elem, unsigned int index)
{
    if (list == NULL || elem == NULL || index >= list->capacity) return;
    // list is full
    if (list->count == list->capacity) return;
    //prunsigned intf("%d\n", *(unsigned int *)(elem));
    
    for (unsigned int i=list->count; i>index; i--) {
        list->value[i] = list->value[i-1];
    }
    list->value[index] = elem;
    //prunsigned intf("%d\n", *(unsigned int *)(list->value[index]));
    list->count++;
}

/// delete all element if existed
void zz_deleteElem(ZZArrayList *list, void *elem)
{
    if (list == NULL || elem == NULL) return;
    
    unsigned int deleteCout = 0;
    for (int i=0; i<list->count; i++)
    {
        if (list->value[i] == elem) {
            deleteCout++;
        }
        else {
            list->value[i-deleteCout] = list->value[i];
        }
     }
    list->count -= deleteCout;
}

/// delete element at index
void zz_deleteElemAtIndex(ZZArrayList *list, unsigned int index)
{
    if (list == NULL || index >= list->count || list->count == 0) return;
    
    for (unsigned int i=index; i<list->count-1; i++) {
        list->value[i] = list->value[i+1];
    }
    list->count--;
}

/// update element
void zz_updateElemAtIndex(ZZArrayList *list, unsigned int index, void *newValue)
{
    if (list == NULL || index >= list->count) return;
    list->value[index] = newValue;
}

/// select
void* zz_selectElemAtIndex(ZZArrayList *list, unsigned int index)
{
    if (list == NULL || index >= list->count || index < 0) return NULL;
    return list->value[index];
}


#pragma mark -- clear

/// clear list
void zz_clearArrayList(ZZArrayList *list)
{
    if (list == NULL) return;
    list->count = 0;
}

/// release list
void zz_releaseArrayList(ZZArrayList *list)
{
    if (list == NULL) return;
}

#pragma mark -- prunsigned int

/// prunsigned int list
void zz_printArrayList(ZZArrayList *list)
{
    if (list == NULL) return;
    
    printf("[");
    for (unsigned int i=0; i<list->count; i++) {
        unsigned int* a = (unsigned int *)(list->value[i]);
        if (i == list->count -1) {
            printf("%d", *a);
        } else {
           printf("%d, ", *a);
        }
    }
    printf("]\n");
}


