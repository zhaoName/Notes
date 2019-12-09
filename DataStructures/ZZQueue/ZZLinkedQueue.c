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

/**
 * init linked queue
 *
 * if queue is empty, rear == head or head->next = null
 */
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
    if (queue->rear == queue->head) {
        printf("linked queue is empty \n");
        return NULL;
    }
    // delete first node
    ZZQueueNode *firstNode = queue->head->next;
    void *result = firstNode->data;
    queue->head->next = firstNode->next;
    
    // determine if the queue is empty after deletion
    if (firstNode == queue->rear) {
        queue->rear = queue->head;
    }
    queue->count--;
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
        printf("linked queue is empty \n");
        return NULL;
    }
}

#pragma mark -- clear

unsigned int zz_isEmpty_linkedQueue(ZZLinkedQueue *queue)
{
    assert(queue);
    return queue->head == queue->rear;
    //return queue->count == 0;
    //return queue->head->next == NULL;
}

unsigned int zz_length_linkedQueue(ZZLinkedQueue *queue)
{
    assert(queue);
    return queue->count;
}

void zz_clear_linkedQueue(ZZLinkedQueue *queue)
{
    assert(queue);
    ZZQueueNode *nd = queue->head->next;
    while (nd) {
        queue->head->next = nd->next;
        free(nd);
        nd = queue->head->next;
    }
    queue->rear = queue->head;
    queue->count = 0;
}

void zz_release_linkedQueue(ZZLinkedQueue **queue)
{
    assert(*queue);
    while ((*queue)->head)
    {
        (*queue)->rear = (*queue)->head->next;
        free((*queue)->head);
        (*queue)->head = (*queue)->rear;
    }
    (*queue)->count = 0;
    free((*queue));
    (*queue) = NULL;
}

void zz_print_linkedQueue(ZZLinkedQueue *queue)
{
    ZZQueueNode *nd = queue->head->next;
    while (nd) {
        printf("%d->", *(int*)nd->data);
        nd = nd->next;
    }
    printf("null\n");
}
