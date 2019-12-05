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



ZZArrayQueue* zz_init_arrayQueue(unsigned int capacity);


void zz_addElem_arrayQueue(ZZArrayQueue *queue, void *newData);
void* zz_deleteElem_arrayQueue(ZZArrayQueue *queue);
void* zz_getHeadElem_arrayQueue(ZZArrayQueue *queue);


unsigned int zz_isEmpty_arrayQueue(ZZArrayQueue *queue);
unsigned int zz_length_arrayQueue(ZZArrayQueue *queue);
void zz_clear_arrayQueue(ZZArrayQueue *queue);
void zz_release_arrayQueue(ZZArrayQueue **queue);


void zz_print_arrayQueue(ZZArrayQueue *queue);

#endif /* ZZArrayQueue_h */

