//
//  ZZArrayList.c
//  ZZDataStructures
//
//  Created by zhaoName on 2019/11/12.
//  Copyright © 2019 XMB. All rights reserved.
//

#define ZZArrayListCompileDefine
typedef struct {
    unsigned int capacity; // allocate memory size
    unsigned int count; // count of element currently in list
    void **value;
}ZZArrayList;

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
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
    assert(list);
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
    if (list == NULL || elem == NULL || index >= list->count) return;
    //printf("%d\n", *(unsigned int *)(elem));
    
    for (unsigned int i=list->count; i>index; i--) {
        list->value[i] = list->value[i-1];
    }
    list->value[index] = elem;
    //printf("%d\n", *(unsigned int *)(list->value[index]));
    list->count++;
}

/// delete all element if existed
void zz_deleteElem(ZZArrayList *list, void *elem)
{
    if (list == NULL || elem == NULL) return;
    
    unsigned int deleteCount = 0;
    for (int i=0; i<list->count; i++)
    {
        if (list->value[i] == elem) {
            deleteCount++;
        }
        else {
            list->value[i-deleteCount] = list->value[i];
        }
     }
    list->count -= deleteCount;
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

/// get elem
void* zz_getElemAtIndex(ZZArrayList *list, unsigned int index)
{
    assert(index < list->count);
    return list->value[index];
}

#pragma mark --

unsigned int zz_containsElem(ZZArrayList *list, void *ele)
{
    assert(list);
    for (int i=0; i<list->count; i++) {
        if (list->value[i] == ele) {
            return 1;
        }
    }
    return 0;
}

#pragma mark -- clear

/// clear list
void zz_clearArrayList(ZZArrayList *list)
{
    assert(list);
    list->count = 0;
}

/// release list
void zz_releaseArrayList(ZZArrayList **list)
{
    assert(*list);
    free((*list)->value);
    free(*list);
    *list = NULL;
}

#pragma mark -- print

/// print list
 zz_printArrayList(ZZArrayList *list)
{
    assert(list);
    
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


