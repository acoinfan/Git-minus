#ifndef STRUCTURES_INCLUDE_H_
#define STRUCTURES_INCLUDE_H_

#include "macros.h"

typedef struct Pair{
    char fileName[FILE_LEN];
    char hashName[FILE_HASH_LEN];
}Pair;

typedef struct FileTree{
    char *cwd;
    int fileNum;
    Pair **fileList;
    int dirNum;
    struct FileTree **dirList;
    struct FileTree *parent;
}FTree;

typedef struct FileArr{
    int num;
    char **list;
}FArr;

typedef struct CommitTree{
    char id[HASH_LEN];
    int num;
    char message[MESSAGE_LEN];
    char timestamp[TIMESTAMP_LEN];
    struct CommitTree **subtree;
    struct CommitTree *parent;
    char mode[MODE_LEN];
    FTree *filetree;
}CTree;

#endif