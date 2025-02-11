#include "filemanage.h"

int commit_file(Pair *file, const char *cwd){
    char commit_PATH[PATH_LEN] = ".gitm/files/";
    strcat(commit_PATH, file->hashName);
    if (exists(commit_PATH)){
        return -1;
    }

    char cnt_PATH[PATH_LEN] = {};
    link_path(cnt_PATH, cwd, file->fileName);
    if (copy_file(cnt_PATH, commit_PATH)){
       ERROR("Commit Error: failed to copy %s to %s\n", cnt_PATH, commit_PATH);
        return -1;
    }
    return 0; 
}

int checkout_file(Pair *file, const char *cwd){
    char commit_PATH[PATH_LEN] = ".gitm/files/";
    strcat(commit_PATH, file->hashName);
    if (!exists(commit_PATH)){
        ERROR("Checkout Error: failed to find %s\n", commit_PATH);
        return -1;
    }
    char cnt_PATH[PATH_LEN] = {};
    link_path(cnt_PATH, cwd, file->fileName);
    if (copy_file(commit_PATH, cnt_PATH)){
        ERROR("Checkout Error: failed to copy %s to %s\n", commit_PATH, cnt_PATH);
        return -1;
    }
    return 0;
}

void commit_all(FTree *node){
    if (node->fileNum != 0){
        for (int i = 0; i < node->fileNum; i++){
            commit_file(node->fileList[i], node->cwd);
        }
    }

    if (node->dirNum != 0){
        for (int i = 0; i < node->dirNum; i++){
            commit_all(node->dirList[i]);
        }
    }
    return;
}

void checkout_all(FTree *node){
    assert(exists(node->cwd));

    if (node->fileNum != 0){
        for (int i = 0; i < node->fileNum; i++){
            checkout_file(node->fileList[i], node->cwd);
        }
    }

    if (node->dirNum != 0){
        for (int i = 0; i < node->dirNum; i++){
            if (!exists(node->dirList[i]->cwd)){
                make_directory(node->dirList[i]->cwd);
            }
            checkout_all(node->dirList[i]);
        }
    }
    return;
}

int delete_file_or_dir(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);

    if (S_ISDIR(path_stat.st_mode)) {
        return rmdir(path); 
    } else if (S_ISREG(path_stat.st_mode)) {
        return remove(path); 
    }
    return -1;
}

void clean_directory(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        ERROR("Failed to open directory");
        return;
    }

    struct dirent *entry;
    char file_path[PATH_LEN];

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 ||
            strcmp(entry->d_name, ".gitm") == 0 || strcmp(entry->d_name, "gitm") == 0) {
            continue;
        }

        snprintf(file_path, sizeof(file_path), "%s/%s", path, entry->d_name);

        if (entry->d_type == DT_DIR) {
            clean_directory(file_path);
            rmdir(file_path); 
        } else {
            remove(file_path);
        }
    }

    closedir(dir);
}
