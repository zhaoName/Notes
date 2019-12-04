//
//  ZZArrayStack.h
//  ZZDataStructures
//
//  Created by zhaoName on 2019/12/4.
//  Copyright © 2019 XMB. All rights reserved.
//  C 顺序栈

#ifndef ZZArrayStack_h
#define ZZArrayStack_h

typedef struct {
    // array that stores stack data
    void **data;
    // point to the top of stack
    int top;
}ZZArrayStack;


/**
 * init stack
 *
 * @param capacity allocated memory size
 */
ZZArrayStack* zz_init_arrayStack(unsigned int capacity);

/**
 * push stack and modify top pointer
 */
void zz_push_arrayStack(ZZArrayStack *stack, void *newData);
/**
 * return top element and modify top pointer
 */
void* zz_pop_arrayStack(ZZArrayStack *stack);
/**
 * return top element
 */
void* zz_getTop_arrayStack(ZZArrayStack *stack);


unsigned int zz_length_arrayStack(ZZArrayStack *stack);

void zz_clear_arrayStack(ZZArrayStack *stack);

void zz_release_arrayStack(ZZArrayStack **stack);

void zz_print_arrayStack(ZZArrayStack *stack);

#endif /* ZZArrayStack_h */
