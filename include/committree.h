#ifndef COMMITTREE_INCLUDE_H_
#define COMMITTREE_INCLUDE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "gitm.h"
#include "filetree.h"
#include "macros.h"
#include "structures.h"

/**
 * @brief 向CTree中插入对应commit id节点
 * @attention commit id基于时间戳生成
 */
CTree *insert_CTree(CTree *head, char *parent, char *id, char *message, char *timestamp, char *mode, FTree *filetree);

/**
 * @brief 在CTree中按commit id查找对应commit节点
 * @param cnt:搜索的起始节点
 */
CTree *search_CTree(CTree *cnt, char *id);

/**
 * @brief 生成一个CTree节点
 */
CTree *new_CNode(char *id, char *message, char *timestamp, char *mode, CTree *parent_node, FTree *filetree);

/**
 * @brief 从指定路径中读取历史commit并生成树
 * @param PATH:一般使用".gitm/log.txt"
 * @attention 若是第一个commit, 令其parent为0
 */
CTree *read_log(char *PATH);

/**
 * @brief 将最新一个commit节点写入log.txt
 * @param node:该commit节点
 */
void write_log(CTree *node, char *PATH);

/**
 * @brief 将最新的->HEAD写入PATH
 * @param id:该->HEAD对应的commit id
 * @param PATH:一般使用".gitm/head.txt
 */
void write_head(char *id, char *PATH);

/** 
 * @brief 从PATH中读取->HEAD
 * @param node:CTree的根节点
 * @param PATH:一般使用".gitm/head.txt
 * @return 返回指向对应CTree中的节点
 */
CTree *read_head(CTree *node, char *PATH);

#endif
