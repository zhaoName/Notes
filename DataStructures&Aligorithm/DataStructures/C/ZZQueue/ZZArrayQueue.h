//
//  ZZArrayQueue.h
//  ZZDataStructures
//
//  Created by zhaoName on 2019/12/5.
//  Copyright © 2019 XMB. All rights reserved.
//  C 循环顺序队列

#ifndef ZZArrayQueue_h
#define ZZArrayQueue_h

typedef struct {
    // array
    void **data;
    // index that point to the first element
    unsigned int front;
    // index that point to the next enqueued element
    unsigned int rear;
}ZZArrayQueue;


/**
 * init circular queue with capacity(_queueSize = capacity)
 *
 * Enqueue and Dequeue time complexity is O(1)
 * front == rear indicates that queue is empty
 * (rear + 1) % _queueSize == front indicates that queue is full
 */
ZZArrayQueue* zz_init_arrayQueue(unsigned int capacity);

/**
 * insert new element in queue if it is not full, and modify queue->rear
 *
 * (rear + 1) % _queueSize == front indicates that queue is full
 */
void zz_addElem_arrayQueue(ZZArrayQueue *queue, void *newData);

/**
 * delete element at front in queue if it isn't empty, and modify front
 *
 * front == rear indicates that queue is empty
 */
void* zz_deleteElem_arrayQueue(ZZArrayQueue *queue);

/**
 * get queue head element
 */
void* zz_getHeadElem_arrayQueue(ZZArrayQueue *queue);

/**
 * front == rear indicates that queue is empty
 */
unsigned int zz_isEmpty_arrayQueue(ZZArrayQueue *queue);

/**
 * if rear > front, length = rear - front
 * if rear < front, length = rear - front + _queueSize
 *
 * ==> length = (rear - front + _queueSize) % _queueSize
 */
unsigned int zz_length_arrayQueue(ZZArrayQueue *queue);
void zz_clear_arrayQueue(ZZArrayQueue *queue);
void zz_release_arrayQueue(ZZArrayQueue **queue);


void zz_print_arrayQueue(ZZArrayQueue *queue);

#endif /* ZZArrayQueue_h */

