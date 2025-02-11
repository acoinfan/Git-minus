#include "filearr.h"

void path_to_filearr(FArr *filearr, const char *PATH, int mode){
    DIR *dir = opendir(PATH);
    if (dir == NULL) {
        return;
    }

    int count = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 &&
            strcmp(entry->d_name, ".gitm") != 0 && strcmp(entry->d_name, "gitm")) {
            if (count == 0){
                filearr->list = malloc(sizeof(char *));
                assert(filearr->list != NULL);
            }
            else{
                filearr->list = realloc(filearr->list, (count + 1) * sizeof(char *));
                assert(filearr->list != NULL);
            }
            if (mode == 0){
                // unhashed -> unhashed (包含文件夹)
                filearr->list[count] = malloc(FILE_LEN * sizeof(char));
                assert(filearr->list[count] != NULL);
                strcpy(filearr->list[count], entry->d_name);
                count++;
            }
            else if (mode == 1){
                // unhashed -> hashed (不含文件夹)
                char cnt_file[PATH_LEN] = {};
                link_path(cnt_file, PATH, entry->d_name);
                if (is_directory(cnt_file)){
                    continue;
                }
                filearr->list[count] = malloc(HASH_LEN * sizeof(char));
                assert(filearr->list[count] != NULL);
                count++;
            }
            else if (mode == 2){
                // hashed -> hashed (不含文件夹)
                filearr->list[count] = malloc(HASH_LEN * sizeof(char));
                assert(filearr->list[count] != NULL);
                strcpy(filearr->list[count], entry->d_name);
                count++;
            }
        }
    }
    closedir(dir);
    filearr->num = count;
    return;
}
