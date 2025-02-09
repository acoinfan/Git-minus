#include "filetree.h"

int is_directory(char *PATH){
    struct stat statbuf;
    
    // 使用 stat 函数获取路径的文件状态信息
    assert(stat(PATH, &statbuf) != -1);

    // 使用 S_ISDIR 宏判断文件状态是否是目录
    return S_ISDIR(statbuf.st_mode);
}

void add_file(FTree *target, char *filename){
    int num = target->fileNum;
    char *PATH = target->cwd;

    if (num == 0){
        target->fileList = malloc(sizeof(Pair *));
    }
    else{
        target->fileList = realloc(target->fileList, (num + 1) * (sizeof(Pair *)));
    }

    assert(target->fileList != NULL);
    target->fileList[num] = malloc(sizeof(Pair));

    assert(target->fileList[num] != NULL);
    pair_init(target->fileList[num], filename, PATH);
    

    target->fileNum += 1;
    return;
}

void add_directory(FTree *target, char *dirname){
    int num = target->dirNum;
    if (num == 0){
        target->dirList = malloc(sizeof(FTree *));
    }
    else{
        target->dirList = realloc(target->dirList, (num + 1) * sizeof(FTree *));
    }

    assert(target->dirList != NULL);
    target->dirList[num] = malloc(sizeof(FTree));

    assert(target->dirList[num] != NULL);
    
    char *new_cwd = malloc(PATH_LEN * sizeof(char));
    link_path(new_cwd, target->cwd, dirname);

    target->dirList[num]->parent = target;
    target->dirList[num]->cwd = new_cwd;
    target->dirList[num]->fileNum = 0;
    target->dirList[num]->dirNum = 0;
    target->dirList[num]->fileList = NULL;
    target->dirList[num]->dirList = NULL;

    target->dirNum += 1;
}

void link_path(char *new_cwd, const char *cwd, const char *filename){
    snprintf(new_cwd, PATH_LEN, "%s/%s", cwd, filename);
}

void fill_filetree(const char *PATH, FTree *node){

    assert(node != NULL);
    FArr *filearr = malloc(sizeof(FArr));
    path_to_filearr(filearr, PATH, 0);
    
    node->cwd = malloc(PATH_LEN * sizeof(char));
    assert(node->cwd != NULL);

    strcpy(node->cwd, PATH);
    node->dirNum = 0;
    node->fileNum = 0;

    int len = filearr->num;
    char *name, fullname[PATH_LEN]; // name:文件名, fullname:带路径文件名
    for (int i = 0; i < len; i++){
        name = filearr->list[i];
        link_path(fullname, PATH, name);
        if (is_directory(fullname)){
            add_directory(node, name);
            fill_filetree(fullname, node->dirList[node->dirNum - 1]);
        }
        else{
            add_file(node, name);
        }
    }
    return;
}

void print_filetree(FTree *node, int level) {
    // 打印当前目录的路径
    for (int i = 0; i < level; i++) {
        printf("  ");  // 每一层缩进两个空格
    }
    printf("[%d] %s/\n", level, node->cwd);  // 打印当前目录的路径

    // 打印当前目录下的文件
    for (int i = 0; i < node->fileNum; i++) {
        for (int j = 0; j < level + 1; j++) {
            printf("  ");  // 每个文件前面增加一个缩进
        }
        printf("- %s\n", node->fileList[i]->fileName);  // 打印文件名
    }

    // 递归打印子目录
    for (int i = 0; i < node->dirNum; i++) {
        print_filetree(node->dirList[i], level + 1);  // 递归打印子目录，增加缩进
    }
}

void write_struct(const char id[HASH_LEN], FTree *filetree){
    char PATH[PATH_LEN] = ".gitm/struct/";
    strcat(PATH, id);
    strcat(PATH, ".txt");

    FILE *file = fopen(PATH, "w");
    assert(file != NULL);
    fclose(file);

    write_filetree(PATH, filetree);
}

void write_filetree(const char PATH[HASH_LEN], FTree *filetree){
    FILE *file = fopen(PATH, "a");
    assert(file != NULL);

    int fileNum = filetree->fileNum, dirNum = filetree->dirNum;
    fprintf(file, "%s\n", filetree->cwd);
    fprintf(file, "%d", fileNum);
    for (int i = 0; i < fileNum; i++){
        Pair *pair = filetree->fileList[i];
        char *fileName = get_filename(pair);
        char *hashName = get_hashname(pair);
        fprintf(file, " %s %s", fileName, hashName);
    }
    fprintf(file, "\n");
    fprintf(file, "%d", dirNum);
    for (int i = 0; i < dirNum; i++){
        fprintf(file, " %s", filetree->dirList[i]->cwd);
    }
    fprintf(file, "\n");
    fclose(file);

    for (int i = 0; i < dirNum; i++){
        write_filetree(PATH, filetree->dirList[i]);
    }
    return;
}


void read_struct(const char id[HASH_LEN], FTree *filetree){
    char PATH[PATH_LEN] = ".gitm/struct/";
    strcat(PATH, id);
    strcat(PATH, ".txt");
    FILE *file = fopen(PATH, "r");
    assert(file != NULL);
    read_filetree(file, filetree);
    fclose(file);
}


void read_filetree(FILE *log, FTree *filetree){
    assert(log != NULL);
    assert(filetree != NULL);
    
    char *cwd = malloc(10000);
    assert(cwd != NULL);
    int fileNum = 0, dirNum = 0;
    fscanf(log, "%s", cwd);
    fscanf(log, "%d", &fileNum);

    filetree->cwd = malloc(PATH_LEN * sizeof(char));
    assert(filetree->cwd != NULL);

    strcpy(filetree->cwd, cwd);
    filetree->fileNum = fileNum;
    filetree->fileList = malloc(fileNum * sizeof(Pair *));

    for (int i = 0; i < fileNum; i++){
        // 读取两个Name
        char fileName[FILE_LEN] = {}, hashName[HASH_LEN] = {};
        fscanf(log, "%s %s", fileName, hashName);

        // 写入两个Name
        filetree->fileList[i] = malloc(sizeof(Pair));
        assert(filetree->fileList[i] != NULL);
        pair_write(filetree->fileList[i], fileName, hashName);
    }

    fscanf(log, "%d", &dirNum);
    filetree->dirNum = dirNum;
    filetree->dirList = malloc(dirNum * sizeof(FTree *));
    for (int i = 0; i < dirNum; i++){
        // 无效冗余用来测试
        char buffer[FILE_LEN] = {};
        fscanf(log, "%s", buffer);
    }


    for (int i = 0; i < dirNum; i++){
        filetree->dirList[i] = malloc(sizeof(FTree));
        assert(filetree->dirList[i] != NULL);
        read_filetree(log, filetree->dirList[i]);
    }
}

void encode_filename(char *PATH, char *hashName){
    assert(!is_directory(PATH));

    FILE *file = fopen(PATH, "r");
    assert(file != NULL);

    size_t size = get_file_size(file);
    char *buffer = malloc(size * sizeof(char));
    assert(buffer != NULL);

    fread(buffer, size, sizeof(char), file);
    fclose(file);

    int res = sha1sum(hashName, buffer, size);
    assert(res == 0);
}

size_t get_file_size(FILE *file){
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

int compare_filetree(FTree *node1, FTree *node2){
    if (strcmp(node1->cwd, node2->cwd) != 0){
        return 1;
    }

    if (node1->fileNum != node2->fileNum){
        return 1;
    }

    for (int i = 0; i < node1->fileNum; i++){
        if (is_same_pair(node1->fileList[i], node2->fileList[i]) == 0){
            return 1;
        }
    }

    if (node1->dirNum != node2->dirNum){
        return 1;
    }

    for (int i = 0; i < node1->dirNum; i++){
        if (compare_filetree(node1->dirList[i], node2->dirList[i]) != 0){
            return 1;
        }
    }

    return 0;
}


















// // 对于已存储的文件而言，filelist就是其对应的hashmap, 默认为 "."
// void get_filelist(const char *PATH, FList *filelist){
//     DIR *dir = opendir(PATH);
//     if (dir == NULL) {
//         perror("opendir");
//         return;
//     }

//     int count = 0;
//     struct dirent *entry;
//     while ((entry = readdir(dir)) != NULL) {
//         if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 &&
//             strcmp(entry->d_name, ".gitm") != 0 && strcmp(entry->d_name, "gitm") != 0) {
//             if (count == 0){
//                 filelist->list = malloc(sizeof(char *));
//                 assert(filelist->list != NULL);
//             }
//             else{
//                 filelist->list = realloc(filelist->list, (count + 1) * sizeof(char *));
//                 assert(filelist->list != NULL);
//             }
//             filelist->list[count] = malloc(ID_LEN * sizeof(char));
//             strcpy(filelist->list[count], entry->d_name);
//             count++;
//         }
//     }
//     closedir(dir);
//     filelist->num = count;
//     return;
// }

// // PATH传入filelist对应的文件所在路径 对应于working_list 应为 ./
// void hash_filelist(char *PATH, FileList *filelist, FileList *newlist){
//     int num = filelist->num;
//     newlist->num = num;
//     char cnt[PATH_LEN] = {};
//     newlist->list = malloc(num * sizeof(char *));
//     for (int i = 0; i < num; i++){
//         strcpy(cnt, PATH);
//         strcat(cnt, filelist->list[i]);
//         newlist->list[i] = malloc(ID_LEN * sizeof(char));
//         get_hash_file(cnt, newlist->list[i]);
//     }
//     return;
// }

// // PATH均为相对路径
// bool compare_file(char* PATH, FileList *filelist){
//     char sha1[ID_LEN] = {};
//     get_hash_file(PATH, sha1);

//     for (int i = 0; i < filelist->num; i++){
//         if (strcmp(sha1, filelist->list[i]) == 0)
//             return true;
//     }
//     return false;
// }

// // 比较hash之后的表 true无修改, false有修改
// bool compare_filelist(FileList *filelist1, FileList *filelist2){
//     int num1 = filelist1->num, num2 = filelist2->num;
//     if (num1 != num2)
//         return false;
//     for (int i = 0; i < num1; i++){
//         if (!is_in_filelist(filelist1->list[i], filelist2))
//             return false;
//     }
//     return true;
// }

// // 判断file(已hash)是否在filelist(已hash)中, true在
// bool is_in_filelist(char *file, FileList *filelist){
//     int num = filelist->num;
//     for (int i = 0; i < num; i++){
//         if (strcmp(file, filelist->list[i]) == 0)
//             return true;
//     }
//     return false;
// }

// size_t get_file_size(FILE *file){
//     fseek(file, 0, SEEK_END);
//     size_t size = ftell(file);
//     fseek(file, 0, SEEK_SET);
//     return size;
// }

// // 将filename存储到.gitm/files中，不检验是否重复
// void save_file(char *filename){
//     char src_path[PATH_LEN] = "./";
//     strcat(src_path, filename);

//     char dest_path[PATH_LEN] = ".gitm/files/";
//     char id[ID_LEN] = {}; // 这里的id是文件对应的hash值
//     get_hash_file(src_path, id);
//     strcat(dest_path, id);

//     int res = copy_file(src_path, dest_path);
//     assert(res == 0);
// }

// void get_hash_file(char *PATH, char *sha1){
//     FILE *file = fopen(PATH, "r");
//     size_t size = get_file_size(file);
//     char *buffer = malloc(size * sizeof(char));
//     assert(buffer != NULL);
//     fread(buffer, size, sizeof(char), file);
//     fclose(file);
//     int res = sha1sum(sha1, buffer, size);
//     assert(res == 0);
// }

// void get_hash_directory(char *timestamps, char *sha1){
//     int res = sha1sum(sha1, (void const *)timestamps, TIMESTAMP_LEN);
//     assert(res == 0);
// }

// // 获取头
// void get_head(char* id){
//     FILE *file = fopen(".gitm/head.txt", "r");
//     fscanf(file, "%s", id);
//     return;
// }

// void write_head(char *id){
//     FILE *file = fopen(".gitm/head.txt", "w");
//     fprintf(file, "%s", id);
//     fclose(file);
//     return;
// }

// // 仅用于测试
// void print_filelist(FileList *filelist){
//     for (int i = 0; i < filelist->num; i++){
//         printf("%02d %s\n", i, filelist->list[i]);
//     }
//     return;
// }