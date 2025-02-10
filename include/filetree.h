#ifndef FILETREE_INCLUDE_H_
#define FILETREE_INCLUDE_H_

#include "gitm.h"
#include "committree.h"
#include "macros.h"
#include "pair.h"
#include "filearr.h"
#include "structures.h"

/**
 * @brief 传入一个相对路径进行判断
 * @return 目录:1, 文件:0
 */
int is_directory(char *PATH);

/**
 * @brief 向target所指的FTree节点添加文件filename
 * @return assert
 */
void add_file(FTree *target, char *filename);

/** 
 * @brief 向target所指的FileTree节点添加文件夹dirname(创造一个空的新节点)
 * @return 成功:0,失败:1
 */
void add_directory(FTree *target, char *dirname);

/**
 * @brief 将cwd和filename连接并补充/, 最后复制给new_cwd
 * @example 例如:"./dir1/dir2" (./dir1 + / + dir2)
 */
void link_path(char *new_cwd, const char *cwd, const char *filename);

/** 
 * @brief 根据parent和当前文件夹名称生成新节点
 * @param target:新节点指针 
 * @param parent:父节点指针
 * @param dirName:该文件夹名称(非完整路径)
 * @return 成功:0, 失败:1
 */
int new_TNode(FTree *target, FTree *parent, char *dirName);

/**
 * @brief 根据指定路径文件夹填满filetree对应节点的内容
 * @attention 忽略. .. .gitm gitm这四个文件/文件夹 
 * @return 成功:0, 失败:1
 */
void fill_filetree(const char *PATH, FTree *node);

/*
    根据指定路径获取文件的hash值
    成功 返回0
    失败 返回1
*/
void encode_filename(char *PATH, char *hashName);

/**
 * @brief 根据指定文件指针, 阅读一个文件夹并写入传入的FTree节点
 * @attention 该函数中包含了对filetree子节点的malloc，但是不含其本身的malloc
 * @return 基于assert
 */
void read_filetree(FILE *log, FTree *filetree);

/**
 * @brief 根据指定txt文件路径, 阅读传入的FTree节点并写入txt文件中
 * @return 基于assert
 */
void write_filetree(const char PATH[HASH_LEN], FTree *filetree);

/**
 * @brief 根据指定PATH和对应的FTree初始化.gitm/struct/id.txt, 并启动写入递归
 * @param id:commit id
 * @param filetree:commit对应的filetree
 * @attention filetree需要malloc后传入(作为根节点)
 */
void write_struct(const char id[HASH_LEN], FTree *filetree);

/**
 * @brief 根据指定PATH(.gitm/struct/id.txt)初始化对应的FTree, 并启动读取递归
 * @param id:commit id
 * @param filetree:commit对应的需要初始化的filetree
 * @return 成功0, 失败-1
 */
int read_struct(const char id[HASH_LEN], FTree *filetree);

size_t get_file_size(FILE *file);

void print_filetree(FTree *node, int level);

/**
 * @brief 根据两个根FTree比较是否相同
 * @return 相同0，不相同1
 */
int compare_filetree(FTree *node1, FTree *node2);

// // 只是一个存储所有文件的表，而不包含文件夹
// void get_fileset(FTree *tree, FList *list);








// bool compare_file(char* PATH, FileList *filelist);

// bool is_in_filelist(char *file, FileList *filelist);

// bool compare_filelist(FileList *filelist1, FileList *filelist2);

// size_t get_file_size(FILE *file);

// void save_file(char *filename);



// void get_hash_directory(char *timestamps, char *sha1);

// void hash_filelist(char *PATH, FileList *filelist, FileList *newlist);

// void print_filelist(FileList *filelist);

// void get_head(char *id);

// void write_head(char *id);

#endif