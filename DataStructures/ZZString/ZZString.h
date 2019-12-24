//
//  ZZString.h
//  ZZDataStructures
//
//  Created by zhaoName on 2019/12/10.
//  Copyright © 2019 XMB. All rights reserved.
//

#ifndef ZZString_h
#define ZZString_h


/**
 * BF 算法(Brute Force):中文叫作暴力匹配算法，也叫朴素匹配算法
 *
 * 最坏：O((n-m+1)*m) => O(n*m) 最好：O(m)
 */
int zz_bf(char *mainStr, char *subStr);
int zz_bf_1(char *mainStr, char *subStr);


int zz_BM(char *mainStr, char *patternStr);


int zz_KMP(char *mainStr, char *patternStr);

#endif /* ZZString_h */
