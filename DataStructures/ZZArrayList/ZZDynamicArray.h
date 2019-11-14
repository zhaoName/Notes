//
//  ZZDynamicArray.h
//  ZZDataStructures
//
//  Created by zhaoName on 2019/11/12.
//  Copyright © 2019 XMB. All rights reserved.
//

#ifndef ZZDynamicArray_h
#define ZZDynamicArray_h

#ifndef ZZDynamicArrayCompileDefine
typedef void ZZDynamicArray;
#endif

/// init list
ZZDynamicArray* zz_init_dynamicArray(unsigned int capacity);
/// length of list
unsigned int zz_length_dynamicArray(ZZDynamicArray *arr);


/// insert element at last
void zz_insertElem(ZZDynamicArray *arr, void *elem);
/// insert element at index
void zz_insertElemAtIndex(ZZDynamicArray *arr, void *elem, unsigned int index);
/// delete all given element if existed
void zz_deleteElem(ZZDynamicArray *arr, void *elem);
/// delete element at index
void zz_deleteElemAtIndex(ZZDynamicArray *arr, unsigned int index);
/// update element
void zz_updateElemAtIndex(ZZDynamicArray *arr, unsigned int index, void *newValue);
/// get element
void* zz_getElemAtIndex(ZZDynamicArray *arr, unsigned int index);


/// clear list
void zz_clear_dynamicArray(ZZDynamicArray *arr);
/// release list
void zz_release_dynamicArray(ZZDynamicArray *arr);


/// return a sublist 
ZZDynamicArray* zz_subArray_dynamicArray(ZZDynamicArray *arr, unsigned int index, unsigned int length);
/// return a copy of list
ZZDynamicArray* zz_copy_dynamicArray(ZZDynamicArray *arr);
/// append list onto des
ZZDynamicArray* zz_append_dynamicArray(ZZDynamicArray *des, ZZDynamicArray *src);
/// insert list at given index
ZZDynamicArray* zz_insert_dynamicArray(ZZDynamicArray *des, ZZDynamicArray *src, unsigned int index);

/// print list
void zz_print_dynamicArray(ZZDynamicArray *arr);

#endif /* ZZDynamicArray_h */
