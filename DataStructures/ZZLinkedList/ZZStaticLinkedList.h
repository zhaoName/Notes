//
//  ZZStaticLinkedList.h
//  ZZDataStructures
//
//  Created by zhaoName on 2019/11/21.
//  Copyright © 2019 XMB. All rights reserved.
//  C 静态链表

#ifndef ZZStaticLinkedList_h
#define ZZStaticLinkedList_h

typedef struct ZZStaticLinkedList{
    // cursor: the subscript of the next node in the array.
    // cur = 0 indicates the next node is null.
    // the cursor of the first element in the array stores the subscript of the next node
    // the cursor of the last element in the array stores the subscript of the first node
    // if the cursor of the last element in the array is 0, indicating that the static linked list is null
    unsigned int cur;
    void *data;
}ZZStaticLinkedList;


ZZStaticLinkedList* zz_init_staticLinkedList(unsigned int capacity);


/// add node at tail
void zz_addNode(ZZStaticLinkedList *list, void *newData);
/**
 * insert node at index in static linked list
 *
 * @param index node position int the linked list, [0, _count-1] in list, [1, _capacity-2] in array
 */
void zz_insertNodeAtIndex(ZZStaticLinkedList *list, void *newData, unsigned int index);

/**
* delete node at index in static linked list
*
* @param index node position in the linked list, [0, _count-1] in list, [1, _capacity-2] in array
*/
void zz_deleteNodeAtIndex(ZZStaticLinkedList *list, unsigned int index);

void zz_updateNodeAtIndex(ZZStaticLinkedList *list, void *newData, unsigned int index);
void* zz_getNodeDataAtIndex(ZZStaticLinkedList *list, unsigned int index);



unsigned int zz_length_staticLinkedList(ZZStaticLinkedList *list);
void zz_clear_staticLinkedList(ZZStaticLinkedList *list);
void zz_release_staticLinkedList(ZZStaticLinkedList **list);


void zz_print_staticLinkedList(ZZStaticLinkedList *list);

#endif /* ZZStaticLinkedList_h */
