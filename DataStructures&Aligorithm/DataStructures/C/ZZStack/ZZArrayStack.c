//
//  ZZArrayStack.c
//  ZZDataStructures
//
//  Created by zhaoName on 2019/12/4.
//  Copyright © 2019 XMB. All rights reserved.
//  C 顺序栈

#include "ZZArrayStack.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

unsigned int _capacity;

ZZArrayStack* zz_init_arrayStack(unsigned int capacity)
{
    ZZArrayStack *stack = malloc(sizeof(ZZArrayStack));
    if (stack) {
        stack->top = -1;
        stack->data = malloc(sizeof(void*) * capacity);
        assert(stack->data);
        _capacity = capacity;
    }
    return stack;
}

#pragma mark -- private method

void zz_resize_arrayStack(ZZArrayStack *stack)
{
    if (stack->top + 2 >= _capacity) {
        _capacity *= 2;
    }
    else {
        return;
    }
    
    stack->data = realloc(stack->data, _capacity * sizeof(void *));
    assert(stack->data);
}

#pragma mark --

/**
 * push stack and modify top pointer
 */
void zz_push_arrayStack(ZZArrayStack *stack, void *newData)
{
    assert(stack);
    zz_resize_arrayStack(stack);
    stack->data[++stack->top] = newData;
}

/**
 * return top element and modify top pointer
 */
void* zz_pop_arrayStack(ZZArrayStack *stack)
{
    assert(stack);
    if (stack->top < 0) {
        return NULL;
    }
    return stack->data[stack->top--];
}

/**
 * return top element
 */
void* zz_getTop_arrayStack(ZZArrayStack *stack)
{
    assert(stack);
    if (stack->top < 0) {
        return NULL;
    }
    return stack->data[stack->top];
}

#pragma mark -- clear

unsigned int zz_length_arrayStack(ZZArrayStack *stack)
{
    assert(stack);
    return stack->top + 1;
}

void zz_clear_arrayStack(ZZArrayStack *stack)
{
    assert(stack);
    free(stack->data);
    stack->data = NULL;
    stack->top = -1;
}

void zz_release_arrayStack(ZZArrayStack **stack)
{
    assert(*stack);
    free((*stack)->data);
    (*stack)->data = NULL;
    (*stack)->top = -1;
    free(*stack);
}

#pragma mark -- print

void zz_print_arrayStack(ZZArrayStack *stack)
{
    assert(stack);
    printf("[");
    for (int i=0; i<=stack->top; i++) {
        printf("%d, ", *(int *)stack->data[i]);
    }
    printf("]\n");
}
