//
//  ZZArrayQueue.c
//  ZZDataStructures
//
//  Created by zhaoName on 2019/12/5.
//  Copyright © 2019 XMB. All rights reserved.
//  C 循环顺序队列

#include "ZZArrayQueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// size of queue
unsigned int _queueSize = 0;

/**
 * init circular queue with capacity(_queueSize = capacity)
 *
 * Enqueue and Dequeue time complexity is O(1)
 * front == rear indicates that queue is empty
 * (rear + 1) % _queueSize == front indicates that queue is full
 */
ZZArrayQueue* zz_init_arrayQueue(unsigned int capacity)
{
    ZZArrayQueue *queue = malloc(sizeof(ZZArrayQueue));
    if (queue) {
        queue->front = queue->rear = 0;
        queue->data = malloc(sizeof(void*) * capacity);
        _queueSize = capacity;
    }
    return queue;
}

#pragma mark -- private method
/**
 * resize queue if it is full
 */
void zz_resize_arrayQueue(ZZArrayQueue *queue)
{
    assert(queue);
    void **newData = malloc(sizeof(void*) * _queueSize * 2);
    // create new array and copy data to newData
    int i = 0;
    for (int j = queue->front; j != queue->rear; j = (j+1) % _queueSize) {
        newData[i++] = queue->data[j];
    }
    // reset queue
    queue->data = newData;
    queue->front = 0;
    queue->rear = i;
    _queueSize *= 2;
}

#pragma mark -- opreation
/**
 * insert new element in queue if it is not full, and modify queue->rear
 *
 * (rear + 1) % _queueSize == front indicates that queue is full
 */
void zz_addElem_arrayQueue(ZZArrayQueue *queue, void *newData)
{
    assert(queue);
    if ((queue->rear + 1) % _queueSize == queue->front) {
        printf("Queue is full !!\n");
        return;
        //zz_resize_arrayQueue(queue);
    }
    queue->data[queue->rear] = newData;
    queue->rear = (queue->rear + 1)  % _queueSize;
}

void* zz_deleteElem_arrayQueue(ZZArrayQueue *queue)
{
    assert(queue);
    if (queue->front == queue->rear) {
        printf("Queue is empty !!\n");
        return NULL;
    }
    void *result = queue->data[queue->front];
    queue->data[queue->front] = NULL;
    queue->front = (queue->front + 1)  % _queueSize;
    return result;
}

void* zz_getHeadElem_arrayQueue(ZZArrayQueue *queue)
{
    assert(queue);
    if (queue->front == queue->rear) {
        printf("Queue is empty !!\n");
        return NULL;
    }
    return queue->data[queue->front];
}

#pragma mark -- clear

unsigned int zz_isEmpty_arrayQueue(ZZArrayQueue *queue)
{
    assert(queue);
    return queue->front == queue->rear;
}

unsigned int zz_length_arrayQueue(ZZArrayQueue *queue)
{
    assert(queue);
    return (queue->rear - queue->front + _queueSize) % _queueSize;
}

void zz_clear_arrayQueue(ZZArrayQueue *queue)
{
    assert(queue);
    for (int i=queue->front; i != queue->rear; i = (i + 1)  % _queueSize) {
        queue->data[i] = NULL;
    }
    queue->front = queue->rear = 0;
}

void zz_release_arrayQueue(ZZArrayQueue **queue)
{
    assert(*queue);
    for (int i=(*queue)->front; i != (*queue)->rear; i = (i + 1)  % _queueSize) {
        (*queue)->data[i] = NULL;
    }
    free((*queue)->data);
    (*queue)->front = (*queue)->rear = 0;
    free(*queue);
    *queue = NULL;
}

#pragma mark -- print

void zz_print_arrayQueue(ZZArrayQueue *queue)
{
    printf("[");
    for (int i=queue->front; i != queue->rear; i = (i + 1)  % _queueSize) {
        printf("%d, ", *(int*)queue->data[i]);
    }
    printf("]\n");
}
