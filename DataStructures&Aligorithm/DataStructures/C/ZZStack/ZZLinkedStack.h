//
//  ZZLinkedStack.h
//  ZZDataStructures
//
//  Created by zhaoName on 2019/12/4.
//  Copyright © 2019 XMB. All rights reserved.
//  C 链栈

#ifndef ZZLinkedStack_h
#define ZZLinkedStack_h

typedef struct ZZLinkedStack{
    void *data;
    struct ZZLinkedStack *next;
}ZZLinkedStack;


ZZLinkedStack* zz_init_linkedStack(void);

/**
 * push stack. The first node is the top of the stack
 */
void zz_push_linkedStack(ZZLinkedStack **stack, void *newData);
/**
 * return top element and modify top pointer
 */
void* zz_pop_linkedStack(ZZLinkedStack **stack);
/**
 * return top element
 */
void* zz_getTop_linkedStack(ZZLinkedStack *stack);


unsigned int zz_length_linkedStack(ZZLinkedStack *stack);

void zz_release_linkedStack(ZZLinkedStack **stack);

void zz_print_linkedStack(ZZLinkedStack *stack);

#endif /* ZZLinkedStack_h */
