#include "command.h"

int init(void){
    if (!exists(".gitm")){
        make_directory(".gitm");
    }

    if (!exists(".gitm/struct")){
        make_directory(".gitm/struct");
    }

    if (!exists(".gitm/files")){
        make_directory(".gitm/files");
    }
    printf("successfully init\n");
    return 0;
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
    printf("time: %s\n", timestamps);
    sha1sum(id, timestamps, TIMESTAMP_LEN);
    printf("%s\n", id);

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
    // printf("------Commit Inf------\nid: %s\nmessage: %s\ntimestamp: %s\nmode: %s\n", Cnode->id, Cnode->message, Cnode->timestamp, Cnode->mode);
    return 0;
}

int checkout(char *id){
    // 一些基础的逻辑判断
    if (!exists(".gitm")){
        return -1;
    }
    if (!exists(".gitm/log.txt")){
        return -1;
    }
    if (!exists(".gitm/files")){
        return -1;
    }
    if (!exists(".gitm/struct")){
        return -1;
    }
    if (!exists(".gitm/head.txt")){
        return -1;
    }

    FTree *node = malloc(sizeof(FTree));
    read_struct(id, node);

    if (node == NULL){
        printf("Checkout Error: Invalid commit ID %s\n", id);
        return -1;
    }

    clean_directory(".");
    checkout_all(node);

    write_head(id, ".gitm/head.txt");
    printf("Successfully Checkout: ID %s\n", id);
}


void print_log(void){
    FILE *log = fopen(".gitm/log.txt", "r");
    char id[HASH_LEN], parent[HASH_LEN], message[MESSAGE_LEN], timestamps[TIMESTAMP_LEN], mode[MODE_LEN];
    while (1){
        int res = fscanf(log, "%s %s %s %s\n", id, parent, message, mode);
        if (res != 4)
            break;

        if (fgets(timestamps, TIMESTAMP_LEN, log) == NULL)
            break;
        timestamps[strcspn(timestamps, "\n")] = '\0';

        printf("commit %s\n", id);
        if (strcmp("merge", mode) == 0)
            printf("Merge: %.7s %.7s\n", id, parent);
        printf("Date: %s\n", timestamps);
        printf("%s\n", message);
        printf("\n");
    }
    fclose(log);
}

void test(void){
    FTree *root = malloc(sizeof(FTree));
    // 假设你已经填充了文件树结构
    fill_filetree(".", root);

    FTree *new = malloc(sizeof(FTree));
    read_struct("test1", new);

    printf("\n------test------\n");
    if(!compare_filetree(root, new)){
        printf("same!!\n");
    }
    else{
        printf("not same!!\n");
    }
    write_struct("test2", new);
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
}
