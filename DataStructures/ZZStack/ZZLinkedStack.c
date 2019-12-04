//
//  ZZLinkedStack.c
//  ZZDataStructures
//
//  Created by zhaoName on 2019/12/4.
//  Copyright © 2019 XMB. All rights reserved.
//  C 链栈

#include "ZZLinkedStack.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

unsigned int _count = 0;

ZZLinkedStack* zz_init_linkedStack(void)
{
    ZZLinkedStack *stack = malloc(sizeof(ZZLinkedStack));
    stack->data = NULL;
    stack->next = NULL;
    return stack;
}

/**
 * push stack. The first node is the top of the stack
 */
void zz_push_linkedStack(ZZLinkedStack **stack, void *newData)
{
    assert(stack);
    // add first node
    if ((*stack)->data == NULL && (*stack)->next == NULL) {
        (*stack)->data = newData;
    }
    else {
        ZZLinkedStack *node = malloc(sizeof(ZZLinkedStack));
        assert(node);
        node->data = newData;
        node->next = *stack;
        // reset stack top
        *stack = node;
    }
    _count++;
}

/**
 * return top element and modify top pointer
 */
void* zz_pop_linkedStack(ZZLinkedStack **stack)
{
    assert(*stack);
    
    if (_count <= 0) return NULL;
    ZZLinkedStack *top = *stack;
    *stack = (*stack)->next;
    void *result = top->data;
    free(top);
    _count--;
    return result;
}

/**
 * return top element
 */
void* zz_getTop_linkedStack(ZZLinkedStack *stack)
{
    assert(stack);
    return stack->data;
}

#pragma mark -- clear

unsigned int zz_length_linkedStack(ZZLinkedStack *stack)
{
    assert(stack);
    return _count;
}

void zz_release_linkedStack(ZZLinkedStack **stack)
{
    while (*stack) {
        ZZLinkedStack *delNode = *stack;
        *stack = (*stack)->next;
        free(delNode);
        delNode = NULL;
    }
    _count = 0;
}

#pragma mark -- print

void zz_print_linkedStack(ZZLinkedStack *stack)
{
    printf("[");
    while (stack) {
        printf("%d, ", *(int *)stack->data);
        stack = stack->next;
    }
    printf("]\n");
}
