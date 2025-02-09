#ifndef FILEARR_INCLUDE_H_
#define FILEARR_INCLUDE_H_

#include "filetree.h"
#include "structures.h"
#include "macros.h"


/*
    根据指定文件结构生成所有文件名列表
    mode选项
    0: 未hashed -> 未hashed (包含文件夹)
    1: 未hashed -> 已hashed (不含文件夹)
    2: 已hashed -> 已hashed (不含文件夹)
*/
void path_to_filearr(FArr *filearr, const char *PATH, int mode);

#endif