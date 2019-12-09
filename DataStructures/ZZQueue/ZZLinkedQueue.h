//
//  ZZLinkedQueue.h
//  ZZDataStructures
//
//  Created by zhaoName on 2019/12/6.
//  Copyright © 2019 XMB. All rights reserved.
//  C 带头节点链队列

#ifndef ZZLinkedQueue_h
#define ZZLinkedQueue_h

typedef struct ZZQueueNode{
    void *data;
    struct ZZQueueNode *next;
}ZZQueueNode;

typedef struct ZZLinkedQueue{
    // length of queue
    unsigned int count;
    // head pointer => dequeue
    ZZQueueNode *head;
    // tail pointer => enqueue
    ZZQueueNode *rear;
}ZZLinkedQueue;


/**
 * init linked queue
 *
 * if queue is empty, rear == head or head->next = null
 */
ZZLinkedQueue* zz_init_linkedQueue(void);

/**
 * enqueue
 */
void zz_addElem_linkedQueue(ZZLinkedQueue *queue, void *newData);

/**
 * dequeue 
 */
void* zz_deleteElem_linkedQueue(ZZLinkedQueue *queue);
/**
 * get queue head element
 */
void* zz_getHeadElem_linkedQueue(ZZLinkedQueue *queue);



unsigned int zz_isEmpty_linkedQueue(ZZLinkedQueue *queue);
unsigned int zz_length_linkedQueue(ZZLinkedQueue *queue);
void zz_clear_linkedQueue(ZZLinkedQueue *queue);
void zz_release_linkedQueue(ZZLinkedQueue **queue);

void zz_print_linkedQueue(ZZLinkedQueue *queue);


#endif /* ZZLinkedQueue_h */
