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
        ERROR("  ");  // 每一层缩进两个空格
    }
    ERROR("[%d] %s/\n", level, node->cwd);  // 打印当前目录的路径

    // 打印当前目录下的文件
    for (int i = 0; i < node->fileNum; i++) {
        for (int j = 0; j < level + 1; j++) {
            ERROR("  ");  // 每个文件前面增加一个缩进
        }
        ERROR("- %s\n", node->fileList[i]->fileName);  // 打印文件名
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


int read_struct(const char id[HASH_LEN], FTree *filetree){
    char PATH[PATH_LEN] = ".gitm/struct/";
    strcat(PATH, id);
    strcat(PATH, ".txt");
    FILE *file = fopen(PATH, "r");
    if (file == NULL){
        return -1;
    }
    read_filetree(file, filetree);
    fclose(file);
    return 0;
}


void read_filetree(FILE *log, FTree *filetree){
    assert(log != NULL);
    assert(filetree != NULL);
    
    char *cwd = malloc(PATH_LEN + 1);
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
        char fileName[FILE_LEN] = {}, hashName[FILE_HASH_LEN] = {};
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

    res = is_stored(hashName, PATH);
    if (res == 0){
        ERROR("Invalid Commit: To many files with same hash!\n");
    }
    else{
        res = abs(res);
        char temp[FILE_HASH_LEN] = {};
        snprintf(temp, FILE_HASH_LEN, "%s-%03d", hashName, res);
        strcpy(hashName, temp);
    }
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
        if (is_same_pair(node1->fileList[i], node2->fileList[i]) != 0){
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

int is_stored(const char *hash, const char *file_PATH){
    int count = 1;
    char cwd[PATH_LEN] = ".gitm/files/";
    char stored_PATH[PATH_LEN] = ".gitm/files/";

    while (1){
        if (count > MAX_SAME_ID){
            return 0;
        }

        snprintf(stored_PATH, FILE_HASH_LEN, "%s%s-%03d", cwd, hash, count);
        if (!exists(stored_PATH)){
            printf("returning -%d since not exist file\n", count);
            return -count;
        }
        else{
            if (is_same_file(file_PATH, stored_PATH)){
                printf("returning %d since find the same file\n", count);
                return count;
            }
            count += 1;
        }
    }
}

int is_same_file(const char *PATH1, const char *PATH2) {
    printf("comparing %s -- %s\n", PATH1, PATH2);
    FILE *file1 = fopen(PATH1, "rb"); 
    FILE *file2 = fopen(PATH2, "rb");

    assert(file1 != NULL && file2 != NULL); 

    int size1 = get_file_size(file1);
    int size2 = get_file_size(file2);

    if (size1 != size2) {
        fclose(file1);
        fclose(file2);
        printf("no same\n");
        return 0; 
    }

    char buffer1[1024], buffer2[1024];
    size_t bytes_read1, bytes_read2;

    while (1) {
        bytes_read1 = fread(buffer1, 1, sizeof(buffer1), file1);
        bytes_read2 = fread(buffer2, 1, sizeof(buffer2), file2);

        if (bytes_read1 != bytes_read2 || memcmp(buffer1, buffer2, bytes_read1) != 0) {
            fclose(file1);
            fclose(file2);
            printf("no same\n");
            return 0;
        }

        if (feof(file1)) {
            break;
        }
    }

    fclose(file1);
    fclose(file2);
    printf("same\n");
    return 1;
}