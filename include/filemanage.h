#ifndef FILEMANAGE_INCLUDE_H_
#define FILEMANAGE_INCLUDE_H_

#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

#include "structures.h"
#include "macros.h"
#include "gitm.h"
#include "pair.h"
#include "structures.h"


/**
 * @brief 将file上传
 * @attention 会检测是否已有该文件
 * @return 成功0, 失败-1
 */
int commit_file(Pair* file, const char *cwd);

/**
 * @brief 将file拉取
 * @attention 假定已有该文件
 * @return 成功0, 失败-1
 */
int checkout_file(Pair *file, const char *cwd);

/**
 * @brief 按照对应的FTree将所有文件上传
 */
void commit_all(FTree *node);

/**
 * @brief 按照对应的FTree将所有文件拉取
 */
void checkout_all(FTree *node);

/**
 * @brief 清除指定路径的的所有文件(除了. .. .gitm gitm)
 */
void clean_directory(const char *path);

/**
 * @brief 清除PATH指向的文件或文件夹
 */
int delete_file_or_dir(const char *path);


#endif