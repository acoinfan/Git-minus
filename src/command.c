#include "command.h"

int init(void){
    if (!exists(".gitm")){
        make_directory(".gitm");
        clear();
        return 0;
    }
    return -1;
}

int commit(char *message){
    // 一些基础的逻辑判断
    if (!exists(".gitm")){
        return -1; // 未做init直接返回 
    }

    if (!exists(".gitm/log.txt")){
        FILE *log = fopen(".gitm/log.txt","w"); // 创建log.txt
        fclose(log);
    }

    if (!exists(".gitm/files")){
        make_directory(".gitm/files");
    }

    if (!exists(".gitm/struct")){
        make_directory(".gitm/struct");
    }

    if (!exists(".gitm/head.txt")){
        FILE *file = fopen(".gitm/head.txt", "w");
        assert(file != NULL);
        fprintf(file, "0");
        fclose(file);
    }
    // 利用log.txt构建CTree
    CTree *base = read_log(".gitm/log.txt");
    CTree *head = read_head(base, ".gitm/head.txt");

    // 获取本次提交的信息(无需获取parent)
    char id[HASH_LEN] = {}, timestamps[TIMESTAMP_LEN] = {}, mode[MODE_LEN] = "commit";
    timestamp(timestamps);
    sha1sum(id, timestamps, TIMESTAMP_LEN);

    // 将工作文件夹中的文件存储在FTree中
    FTree *Fnode = malloc(sizeof(FTree));
    fill_filetree(".", Fnode);

    // 生成一个新CTree节点
    CTree *Cnode;
    Cnode = new_CNode(id, message, timestamps, mode, head, Fnode);

    // 将CTree写入log.txt
    write_log(Cnode, ".gitm/log.txt");
    write_head(Cnode->id, ".gitm/head.txt");

    // 将FTree写入id.txt
    write_struct(id, Fnode);

    // 将文件存储进files文件夹
    commit_all(Fnode);

    // 打印commit成功内容 (DEBUG用)
    //printf("------Commit Inf------\nid: %s\nmessage: %s\ntimestamp: %s\nmode: %s\nparent: %s\n", Cnode->id, Cnode->message, Cnode->timestamp, Cnode->mode, Cnode->parent->id);
    ERROR("%s\n", id);
    return 0;
}

int checkout(char *id){
    // 一些基础的逻辑判断
    if (!exists(".gitm")){
        ERROR("Checkout Error: Invalid repository\n");
        return -1;
    }
    if (!exists(".gitm/log.txt")){
        ERROR("Checkout Error: Invalid repository\n");
        return -1;
    }
    if (!exists(".gitm/files")){
        ERROR("Checkout Error: Invalid repository\n");
        return -1;
    }
    if (!exists(".gitm/struct")){
        ERROR("Checkout Error: Invalid repository\n");
        return -1;
    }
    if (!exists(".gitm/head.txt")){
        ERROR("Checkout Error: Invalid repository\n");
        return -1;
    }

    FTree *target = malloc(sizeof(FTree));
    if (read_struct(id, target)){
        ERROR("Checkout Error: Invalid commit ID %s\n", id);
        return -1;
    }

    FTree *cnt = malloc(sizeof(FTree));
    fill_filetree(".", cnt);
    if (compare_filetree(cnt, target)){
        ERROR("Dirty Checkout: Ignore\n");
        return -1;
    }

    clean_directory(".");
    checkout_all(target);

    write_head(id, ".gitm/head.txt");
    // printf("Successfully Checkout: ID %s\n", id);
    return 0;
}


void print_log(void){
    // 利用log.txt构建CTree
    CTree *base = read_log(".gitm/log.txt");
    CTree *head = read_head(base, ".gitm/head.txt");
    
    CTree *cnt = head;
    while(cnt != NULL){
        printf("commit %s\n", cnt->id);
        printf("Date: %s\n", cnt->timestamp);
        printf("%s\n\n", cnt->message);
        cnt = cnt->parent;
    }
    return;
}

void test(char *id){
    CTree *base = read_log(".gitm/log.txt");
    //CTree *head = read_head(base, ".gitm/head.txt");
    //printf("---head inf---\nid: %s\n", head->id);
    print_commit_tree(base, 0);
}

void clear(void){
    FILE *log = fopen(".gitm/log.txt","w"); // 创建log.txt
    fclose(log);

    FILE *file = fopen(".gitm/head.txt", "w");
    assert(file != NULL);
    fprintf(file, "0");
    fclose(file);   

    if (!exists(".gitm/files")){
        make_directory(".gitm/files");
    }
    else{
        clean_directory(".gitm/files");
    }

    if (!exists(".gitm/struct")){
        make_directory(".gitm/struct");
    }
    else{
        clean_directory(".gitm/struct");
    }
}
