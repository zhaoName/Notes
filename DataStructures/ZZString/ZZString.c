//
//  ZZString.c
//  ZZDataStructures
//
//  Created by zhaoName on 2019/12/10.
//  Copyright © 2019 XMB. All rights reserved.
//

#include "ZZString.h"
#include <stdio.h>
#include <string.h>

#pragma mark -- BF

/**
 * BF 算法(Brute Force):中文叫作暴力匹配算法，也叫朴素匹配算法
 *
 * return -1 not found, or return i
 * 最坏：O((n-m+1)*m) => O(n*m)  最好：O(m)
 */
int zz_bf(char *mainStr, char *subStr)
{
    unsigned long n = strlen(mainStr);
    unsigned long m = strlen(subStr);
    
    if (n < m ) return -1;
    
    for (int i=0; i<=n-m+1; i++)
    {
        int j;
        for (j=0; j<m; j++) {
            if (mainStr[i+j] == subStr[j]) {
                continue;
            }
            else {
                break;
            }
        }
        if (j == m) {
            return i;
        }
    }
    return -1;
}


int zz_bf_1(char *mainStr, char *subStr)
{
    unsigned long n = strlen(mainStr);
    unsigned long m = strlen(subStr);
    
    if (n < m ) return -1;
    
    int i=0, j = 0;
    while (i<n-m+1 && j<m)
    {
        if (mainStr[i+j] == subStr[j]) {
            j++;
            if (j==m) {
                return i;
            }
        }
        else {
            i++;
            j = 0;
        }
    }
    return -1;
}


#pragma mark --

