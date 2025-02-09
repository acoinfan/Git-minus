#ifndef PAIR_INCLUDE_H_
#define PAIR_INCLUDE_H_

#include "gitm.h"
#include "filetree.h"
#include "committree.h"
#include "macros.h"
#include "structures.h"


/** 
 * @brief 基于指定文件及其路径生成pair
 * @param fileName:文件名
 * @param dirPATH:文件所在路径(不含文件名)
 * @attention Pair需要malloc后传入
 * @return 基于assert
 */
void pair_init(Pair *pair, const char *fileName, const char *dirPATH);

/**
 * @brief 基于指定的fileName和hashName生成pair
 * @attention Pair需要malloc后传入
 * @return 基于assert
 */
void pair_write(Pair *pair, char *fileName, char *hashName);

// 获得pair的文件名
char *get_filename(Pair *pair);

// 获得pair的hash值
char *get_hashname(Pair *pair);

/**
 * @brief 比较hash值确认是否是同一个文件
 * @attention 文件名不影响hash值
 * @return 相同:0, 不相同:非零值
 */
int is_same_hashname(Pair *pair1, Pair *pair2);

/*
    比较文件名是否相同
    相同 返回0
    不相同 返回非零值
*/
int is_same_filename(Pair *pair1, Pair *pair2);

/*
    比较两个pair是否完全相同
    相同 返回0
    不相同 返回非零值
*/
int is_same_pair(Pair *pair1, Pair *pair2);

#endif