#include "pair.h"

void pair_init(Pair *pair, const char *fileName, const char *dirPATH){
    assert(pair != NULL);
    assert(strlen(fileName) <= FILE_LEN);
    strcpy(pair->fileName, fileName);
    char hashName[HASH_LEN] = {};
    char PATH[PATH_LEN] = {};
    strcpy(PATH, dirPATH);
    strcat(PATH, "/");
    strcat(PATH, fileName);
    encode_filename(PATH, hashName);
    strcpy(pair->hashName, hashName);
}

void pair_write(Pair *pair, char *fileName, char *hashName){
    assert(strlen(fileName) <= FILE_LEN && strlen(hashName) <= FILE_HASH_LEN);
    assert(pair != NULL);
    strcpy(pair->fileName, fileName);
    strcpy(pair->hashName, hashName);
}

char *get_filename(Pair *pair){
    assert(pair->fileName != NULL);
    return pair->fileName;
}

char *get_hashname(Pair *pair){
    assert(pair->hashName != NULL);
    return pair->hashName;
}

int is_same_hashname(Pair *pair1, Pair *pair2){
    return strcmp(get_hashname(pair1), get_hashname(pair2));
}

int is_same_filename(Pair *pair1, Pair *pair2){
    return strcmp(get_filename(pair1), get_filename(pair2));
}

int is_same_pair(Pair *pair1, Pair *pair2){
    if (strcmp(pair1->fileName, pair2->fileName) == 0 && strcmp(pair1->hashName, pair2->hashName) == 0){
        return 0;
    }else{
        return 1;
    }
}