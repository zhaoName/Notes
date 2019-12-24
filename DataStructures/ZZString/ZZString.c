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
#include <stdlib.h>
#include <math.h>

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


#pragma mark -- BM

/**
 * 将模式串转化为散列表 这样查找坏字符在模式串中的下标 xi 的时间复杂度为O(1)
 *
 * 模式串中的每个字符对应的ASCII码值为散列表的下标，散列表中存储字符在模式串中的位置
 */
int* _zz_create_hashArray(char *patternStr, int patternStrSize)
{
    int *hashArray = malloc(sizeof(int) * 256);
    // 初始化 散列表
    memset(hashArray, -1, 256);
    
    // 计算 patternStr[i] 在散列表中的位置
    // patternStr中字符若有重复 只保留最大的，因为只需查找坏字符在模式串中第一次出现的位置(逆序)
    for (int i=0; i<patternStrSize; ++i) {
        hashArray[(int)patternStr[i]] = i;
    }
    return hashArray;
}

/**
 * 预处理模式串
 *
 * 模式串后缀子串[r, patternStrSize-1]  j+2 <= r <= patternStrSize-1 (j 为坏字符对应模式串中的下标)
 * 模式串前缀子串[0, j]    0 <= j < [(patternStrSize * 0.5) - 1]
 *
 * @param suffic 下标为模式串后缀子串的长度 值为模式串中另一个和后缀子串匹配的子串的起始下标值
 *
 * @param prefix 下标为模式串后缀子串的长度 值为 bool 类型，标记模式串的后缀子串能否匹配到模式串的前缀子串
 */
void _zz_handle_pattertStr_dalao(char *patternStr, int patternStrSize, int **suffic, int **prefix)
{
    // 初始化
    for (int i=1; i<patternStrSize; ++i) {
        (*suffic)[i] = -1;
        (*prefix)[i] = 0;
    }
    
    for (int i=0; i<patternStrSize-1; ++i)
    {
        int j = i;
        // 后缀子串长度
        int k = 0;
        while(j>=0 && patternStr[j] == patternStr[patternStrSize-1-k]) {
            ++k;
            (*suffic)[k] = j;
            --j;
        }
        if (j == -1) (*prefix)[k] = 1;
    }
}

void _zz_handle_pattertStr_self(char *patternStr, int patternStrSize, int **suffic, int **prefix)
{
    for (int i=1; i<patternStrSize; ++i) {
        (*suffic)[i] = -1;
        (*prefix)[i] = 0;
    }
    // 后缀子串长度
    int i = 1;
    while (i <= patternStrSize / 2)
    {
        // 后缀字串
        char *suf_temp = malloc(sizeof(char) * i);
        strncpy(suf_temp, patternStr + patternStrSize - i, i);
        
        for (int j=0; j<patternStrSize-i; j++) {
            // 子串
            char *pre_temp = malloc(sizeof(char) * i);
            strncpy(pre_temp, patternStr + j, i);
            
            if (suf_temp == pre_temp) {
                (*suffic)[i] = j;
                // j == 0 说明模式串的后缀子串能匹配到模式串的前缀子串
                (*prefix)[i] = (j == 0 ? 1 : 0);
            }
            free(pre_temp);
        }
        free(suf_temp);
        i++;
    }
}

/**
 * 根据 好后缀规则 计算模式串要往后滑动的距离
 *
 * 1> 假设好后缀长度为 k，若suffic[k] != -1，则滑动 j - suffic[k] + 1 (j 为坏字符对应模式串中的下标)
 * 2> 若suffic[k] == -1，取好后缀的子串b[r, m-1](j+2<= r <=m-1)的长度 k=m-r
 *    若prefix[k] == 1,则存在与好后缀子串匹配的 前缀子串，滑动距离为 r
 * 3> 若上述两条都不满足 则滑动模式串长度
 */
int _zz_good_index(int *suffic, int *prefix, int patternStrSize, int j)
{
    // patternStrSize-j-1 为好后缀的长度
    if (suffic[patternStrSize-j-1] != -1) {
        return j - suffic[patternStrSize-j-1] + 1;
    }
    int r = j + 2;
    while (r <= patternStrSize - 1) {
        if (prefix[patternStrSize - r] == 1) {
            return r;
        }
        ++r;
    }
    return patternStrSize;
}

/**
 * BM 算法 (i变大 j轮询)
 *
 * 坏字符(主串)：逆序对比模式串和主串 第一个不同的字符 记做坏字符
 * 第一个不同的字符在模式串中的下标 si
 * 坏字符在模式串中的下标(第一个) xi
 * 模式串滑动的位置 si - xi
 *
 * 好后缀(主串)：
 * 1> 在模式串中查找另外一个和 好后缀 匹配的子串
 * 2> 在 好后缀 的子串中查找最长能跟 模式串前缀子串 匹配的后缀子串
 */
int zz_BM(char *mainStr, char *patternStr)
{
    int mainStrSize = (int)strlen(mainStr);
    int patternStrSize = (int)strlen(patternStr);
    // 将模式串中的字符 构建成散列表
    int *hashArray = _zz_create_hashArray(patternStr, patternStrSize);
    // 好后缀 前缀子串
    int *suffic = malloc(sizeof(int) * patternStrSize);
    int *prefix = malloc(sizeof(int) * patternStrSize);
    // 预处理模式串
    _zz_handle_pattertStr_dalao(patternStr, patternStrSize, &suffic, &prefix);
    
    
    int i = 0;
    while (i <= mainStrSize - patternStrSize)
    {
        // j 是坏字符对应模式串中的下标(即比较到下标为 j 的元素时，出现主串和模式串不匹配的情况)
        int j;
        for (j=patternStrSize-1; j>=0; --j) {
            if (mainStr[i+j] != patternStr[j]) break;
        }
        // 模式串和主串匹配成功
        if (j < 0) return i;
        
        // 坏字符在模式串中出现的下标
        int xi = hashArray[(int)mainStr[i+j]];
        // 坏字符规则 要滑动的距离
        int bad_index = j - xi;
        
        // 好后缀规则 要滑动的距离
        int good_index = 0;
        // 若存在好后缀
        if (j < patternStrSize - 1) {
            good_index = _zz_good_index(suffic, prefix, patternStrSize, j);
        }
        // 综合好后缀和坏字符之后 实际滑动的距离
        i = i + (bad_index > good_index ? bad_index : good_index);
    }
    free(hashArray);
    free(suffic);
    free(prefix);
    return -1;
}

#pragma mark -- KMP

/**
 * next 数组：下标为模式串每个前缀结尾字符下标，值为这个前缀的最长可匹配前缀字符串结尾字符下标
 *
 * 1> next[i-1] 的最长可匹配前缀为 b[0, k-1]
 *    若 b[0, k-1]的下一个字符 b[k] == b[i], 则 b[0, k] 是 b[0, i] 的最长可匹配前缀, 即 next[i] = k
 *
 * 2> 若 b[k] != b[i], 需查递归找 next[i-1]的次长可匹配前缀子串,
 *    next[i-1] 是 i-1 的最长匹配前缀子串的结尾下标,
 *    则 next[next[i-1]] 是其次长匹配前缀子串的结尾下标.
 *    递归结束条件：k=-1 或 次长匹配前缀子串的下一个字符和当前字符匹配.
 *
 * 解释 **** next[next[i-1]] 是其次长匹配前缀子串的结尾下标 ****
 *
 * 若 b[0, i-1] 的次长可匹配后缀子串 b[x, i-1] 对应的可匹配前缀 b[0, i-1-x] 的下一个字符 b[i-x] = b[i], 则 b[x, i] 就是 b[0, i] 的最长可匹配后缀子串
 * 那怎么求 b[0, i-1] 的次长可匹配后缀子串 b[x, i-1] ？？？
 *
 * 次长可匹配后缀子串肯定被包含在最长可匹配后缀子串中, 最长可匹配后缀子串 对应着 最长可匹配前缀子串,
 * 所以问题被转化为：查找 最长可匹配前缀子串 的 最长可匹配前缀子串 问题.
 *
 * 所以想要查找 b[0, i-1]的次长可匹配前缀子串，只需查找在其最长前缀子串 b[0, next[i-1]] 中查找最长可匹配前缀子串 也就是 b[0,next[next[i-1]]]
 */
void _zz_get_next(char *patternStr, int patternStrSize, int **next)
{
    (*next)[0] = -1;
    int k = -1;
    
    for (int i=1; i<patternStrSize; ++i)
    {
        // 递归查找 次长匹配前缀子串
        while (k != -1 && patternStr[k+1] != patternStr[i]) {
            k = (*next)[k];
        }
        // next[i-1] => next[i]
        if (patternStr[k+1] == patternStr[i]) {
            k++;
        }
        (*next)[i] = k;
    }
}

/**
 * KMP 算法(i不变 j变小)
 *
 * 正序匹配主串和模式串, 坏字符(下标为j)之前已匹配好的字符串称为好前缀
 * 在好前缀中查找 能与好前缀的前缀子串 匹配的最长后缀子串 长度为k
 * 则模式串往后滑动 j-k 位，也就是保持 i 不变，将 k 赋值给 j
 */
int zz_KMP(char *mainStr, char *patternStr)
{
    int mainStrSize = (int)strlen(mainStr);
    int patternStrSize = (int)strlen(patternStr);
    
    int *next = malloc(sizeof(int) * patternStrSize);
    
    // 获取 next 数组，记录模式串每个前缀的最长可匹配前缀的结尾字符下标
    _zz_get_next(patternStr, patternStrSize, &next);
    // 坏字符下标
    int j = 0;
    for (int i=0; i<mainStrSize-patternStrSize; ++i)
    {
        while (j > 0 && mainStr[i] != patternStr[j]) {
            // i 不变 j 变小 相当于往后滑动模式串
            j = next[j-1] + 1;
        }
        // 好前缀
        if (mainStr[i] == patternStr[j]) j++;
        
        // 找到匹配的模式串
        if (j == patternStrSize) return i - patternStrSize + 1;
    }
    free(next);
    return -1;
}
