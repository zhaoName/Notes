//
//  ZZDynamicArray.c
//  ZZDataStructures
//
//  Created by zhaoName on 2019/11/12.
//  Copyright © 2019 XMB. All rights reserved.
//  C 实现的动态数组


#define ZZDynamicArrayCompileDefine
typedef struct {
    unsigned int capacity; // alloc
    unsigned int count; // count of element currently in arr
    void **value;
}ZZDynamicArray;

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "ZZDynamicArray.h"


#pragma mark -- init

ZZDynamicArray* zz_init_dynamicArray(unsigned int capacity)
{
    ZZDynamicArray *arr = malloc(sizeof(ZZDynamicArray));
    if (arr) {
        arr->capacity = capacity;
        arr->count = 0;
        arr->value = malloc(sizeof(void *) * (capacity ? capacity : 1));
    }
    return arr;
}

void zz_resize_dynamicArray(ZZDynamicArray *arr, unsigned int newSize)
{
    assert(arr);
    unsigned int newCapacity = arr->capacity;
    // grow if arr is full
    if (newSize > arr->capacity) {
        while (newCapacity < newSize) {
            newCapacity *= 2;
        }
    }
    // shrink if arr is almost empty
    else if (newSize * 2 < arr->capacity) {
        newCapacity *= 0.5;
    }
    else {
        return;
    }
    // realloc 用于修改一个已经分配内存空间的内存块的大小，可以使这块内存扩大或缩小
    // 若 newSize < size, 则原来内存空间的尾部将会被去除，剩余部分内存的内容依然会被保留
    // 若 newSize > size：扩大原来内存块大小
    // 若原来内存块尾部有足够大的内存空间，则直接在原来内存块尾部开辟新的空间、
    // 若原来内存块尾部没有足够大的内存空间，则会重新开辟一块 newSize 大小的内存空间，并将原来内存块的内容拷贝过来
    arr->value = realloc(arr->value, sizeof(void *) * newCapacity);
    arr->capacity = newCapacity;
    assert(arr->value);
}

/// length of arr
unsigned int zz_length_dynamicArray(ZZDynamicArray *arr)
{
    assert(arr);
    return arr->count;
}

#pragma mark -- CRUD

/// insert element at last
void zz_insertElem(ZZDynamicArray *arr, void *elem)
{
    if (arr == NULL || elem == NULL) return;
    zz_insertElemAtIndex(arr, elem, arr->count);
}

/// insert element at index
void zz_insertElemAtIndex(ZZDynamicArray *arr, void *elem, unsigned int index)
{
    if (arr == NULL || elem == NULL || index > arr->count) return;
    // grow if full
    zz_resize_dynamicArray(arr, arr->count+1);
    //printf("%d\n", *(unsigned int *)(elem));

    for (unsigned int i=arr->count; i>index; i--) {
        arr->value[i] = arr->value[i-1];
    }
    arr->value[index] = elem;
    //printf("%d\n", *(unsigned int *)(arr->value[index]));
    arr->count++;
}

/// delete all given element if existed
void zz_deleteElem(ZZDynamicArray *arr, void *elem)
{
    if (arr == NULL || elem == NULL) return;

    unsigned int deleteCount = 0;
    for (int i=0; i<arr->count; i++)
    {
        if (arr->value[i] == elem) {
            deleteCount++;
        }
        else {
            arr->value[i-deleteCount] = arr->value[i];
        }
     }
    arr->count -= deleteCount;
    // shrink if almost empty
    zz_resize_dynamicArray(arr, arr->count);
}

/// delete element at index
void zz_deleteElemAtIndex(ZZDynamicArray *arr, unsigned int index)
{
    if (arr == NULL || index >= arr->count || arr->count == 0) return;

    for (unsigned int i=index; i<arr->count-1; i++) {
        arr->value[i] = arr->value[i+1];
    }
    arr->count--;
    // shrink if almost empty
    zz_resize_dynamicArray(arr, arr->count);
}

/// update element
void zz_updateElemAtIndex(ZZDynamicArray *arr, unsigned int index, void *newValue)
{
    if (arr == NULL || index >= arr->count) return;
    arr->value[index] = newValue;
}
 
/// get elem
void* zz_getElemAtIndex(ZZDynamicArray *arr, unsigned int index)
{
    if (arr == NULL || index >= arr->count ) return NULL;
    return arr->value[index];
}


#pragma mark -- clear

///// clear arr
void zz_clear_dynamicArray(ZZDynamicArray *arr)
{
    assert(arr);
    arr->count = 0;
}

/// release arr
void zz_release_dynamicArray(ZZDynamicArray *arr)
{
    assert(arr);
    free(arr->value);
    free(arr);
}

#pragma mark -- 

/// return a sublist  [index, length-1]
ZZDynamicArray* zz_subArray_dynamicArray(ZZDynamicArray *arr, unsigned int index, unsigned int length)
{
    assert(arr);
    // out of range
    if (index + length > arr->count) return NULL;
    
    ZZDynamicArray *newArr = zz_init_dynamicArray(length);
    memmove(newArr->value, arr->value + index, length * sizeof(void *));
    newArr->count = length;
    return newArr;
}

/// return a copy of list
ZZDynamicArray* zz_copy_dynamicArray(ZZDynamicArray *arr)
{
    return zz_subArray_dynamicArray(arr, 0, arr->count);
}

/// append list onto des
ZZDynamicArray* zz_append_dynamicArray(ZZDynamicArray *des, ZZDynamicArray *src)
{
    return zz_insert_dynamicArray(des, src, des->count);
}

/// insert list at given index
ZZDynamicArray* zz_insert_dynamicArray(ZZDynamicArray *des, ZZDynamicArray *src, unsigned int index)
{
    // out of range
    if (index > des->count) return NULL;
    
    // expansion if capacity of des is not enough
    zz_resize_dynamicArray(des, des->count + src->count);
    // move data after index to right
    memmove(des->value + index + src->count, des->value + index, (des->count - index) * sizeof(void *));
    // copy src to des
    memmove(des->value + index, src->value, src->count * sizeof(void*));
    // modify des count
    des->count += src->count;
    return des;
}


#pragma mark -- print

/// prunsigned int arr
void zz_print_dynamicArray(ZZDynamicArray *arr)
{
    assert(arr);

    printf("[");
    for (unsigned int i=0; i<arr->count; i++) {
        unsigned int* a = (unsigned int *)(arr->value[i]);
        if (i == arr->count -1) {
            printf("%d", *a);
        } else {
           printf("%d, ", *a);
        }
    }
    printf("]\n");
}
