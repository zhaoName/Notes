//
//  ZZLinkedQueue.c
//  ZZDataStructures
//
//  Created by zhaoName on 2019/12/6.
//  Copyright © 2019 XMB. All rights reserved.
//  C 带头节点链队列

#include "ZZLinkedQueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


ZZLinkedQueue* zz_init_linkedQueue(void)
{
    ZZLinkedQueue *queue = malloc(sizeof(ZZLinkedQueue));
    if (queue) {
        queue->count = 0;
        queue->head = malloc(sizeof(ZZQueueNode));
        assert(queue->head);
        queue->head->next = NULL;
        
        queue->rear = malloc(sizeof(ZZQueueNode));
        assert(queue->rear);
        queue->rear = queue->head;
    }
    
    return queue;
}

/**
 * enqueue
 */
void zz_addElem_linkedQueue(ZZLinkedQueue *queue, void *newData)
{
    assert(queue);
    ZZQueueNode *newNode = malloc(sizeof(ZZQueueNode));
    if (newNode == NULL) return;
    
    // create new node
    newNode->data = newData;
    newNode->next = NULL;
    
    queue->rear->next = newNode;
    queue->rear = newNode;
    
    queue->count++;
}

/**
 * dequeue
 */
void* zz_deleteElem_linkedQueue(ZZLinkedQueue *queue)
{
    assert(queue);
    // queue is empty
    if (queue->rear == NULL) {
        return NULL;
    }
    // delete first node
    ZZQueueNode *firstNode = queue->head->next;
    void *result = firstNode->data;
    queue->head->next = firstNode->next;
    // determine if the queue is empty after deletion
    if (firstNode == queue->rear) {
        queue->rear = NULL;
    }
    free(firstNode);
    return  result;
}

/**
 * get queue head element
 */
void* zz_getHeadElem_linkedQueue(ZZLinkedQueue *queue)
{
    assert(queue);
    
    if (queue->head->next) {
        return queue->head->next->data;
    } else {
        return NULL;
    }
}
