//
//  ZZArrayList.h
//  ZZDataStructures
//
//  Created by zhaoName on 2019/11/12.
//  Copyright © 2019 XMB. All rights reserved.
//  C 实现的静态数组

#ifndef ZZArrayList_h
#define ZZArrayList_h


#ifndef ZZArrayListCompileDefine
typedef void ZZArrayList;
#endif

/// init list
ZZArrayList* zz_init_arrylist(unsigned int capacity);
/// length of list
unsigned int zz_lengthOfArrayList(ZZArrayList *list);


/// insert element at last
void zz_insertElem(ZZArrayList *list, void *elem);
/// insert element at index
void zz_insertElemAtIndex(ZZArrayList *list, void *elem, unsigned int index);
/// delete all element if existed
void zz_deleteElem(ZZArrayList *list, void *elem);
/// delete element at index
void zz_deleteElemAtIndex(ZZArrayList *list, unsigned int index);
/// update element
void zz_updateElemAtIndex(ZZArrayList *list, unsigned int index, void *newValue);
/// select
void* zz_getElemAtIndex(ZZArrayList *list, unsigned int index);


/// judge arr contained elem or not, return 1 or 0
unsigned int zz_containsElem(ZZArrayList *list, void *ele);


/// clear list
void zz_clearArrayList(ZZArrayList *list);
/// release list
void zz_releaseArrayList(ZZArrayList **list);


/// print list
void zz_printArrayList(ZZArrayList *list);

#endif /* ZZArrayList_h */
