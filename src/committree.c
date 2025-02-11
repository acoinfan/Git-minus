#include "committree.h"

CTree* insert_CTree(CTree *head, char *parent, char *id, char *message, char *timestamp, char *mode, FTree *filetree){ 
    if (head == NULL){
        head = new_CNode(id, message, timestamp, mode, NULL, filetree);
        return head;
    }

    CTree *parent_node = search_CTree(head, parent);
    assert(parent_node != NULL);
    CTree *node = new_CNode(id, message, timestamp, mode, parent_node, filetree);

    parent_node->num += 1;
    int num = parent_node->num;
    if (num == 1){
        parent_node->subtree = malloc(sizeof(CTree *));
        assert(parent_node->subtree != NULL);
        parent_node->subtree[0] = node;
    }  
    else{
        parent_node->subtree = realloc(parent_node->subtree, num * sizeof(CTree *));
        assert(parent_node->subtree != NULL);
        parent_node->subtree[num - 1] = node;
    }
    return head;
}
    
CTree *search_CTree(CTree *cnt, char *id){
    if (cnt == NULL){
        return NULL;
    }

    if (strcmp(cnt->id, id) == 0)
        return cnt;
    else{
        CTree *result = NULL;
        for (int i = 0; i < cnt->num; i++){
            result = search_CTree(cnt->subtree[i], id);
            if (result != NULL){
                return result;
            }
        }
    } 
    return NULL; 
}

CTree *new_CNode(char *id, char *message, char *timestamp, char *mode, CTree *parent_node, FTree *filetree){
    CTree *node = malloc(sizeof(CTree));
    assert(node != NULL);
    node->num = 0;
    node->subtree = NULL;
    node->parent = parent_node;
    node->filetree = filetree;
    strcpy(node->id, id);
    strcpy(node->message, message);
    strcpy(node->timestamp, timestamp);
    strcpy(node->mode, mode);
    return node;
}


CTree *read_log(char *PATH){
    FILE *log = fopen(PATH, "r");
    assert(log != NULL);
    CTree *head = NULL;

    while (1){
        char id[HASH_LEN], parent[HASH_LEN], message[MESSAGE_LEN], timestamps[TIMESTAMP_LEN], mode[MODE_LEN];
        int res = fscanf(log, "%s %s %s\n", id, parent, mode);
        if (res != 3)
            break;

        if (fgets(message, MESSAGE_LEN, log) == NULL)
            break;
        message[strcspn(message, "\n")] = '\0';

        if (fgets(timestamps, TIMESTAMP_LEN, log) == NULL)
            break;
        timestamps[strcspn(timestamps, "\n")] = '\0';
        
        FTree *filetree = malloc(sizeof(FTree));
        assert(filetree != NULL);

        read_struct(id, filetree);
        head = insert_CTree(head, parent, id, message, timestamps, mode, filetree);
    }
    fclose(log);
    return head; // 根节点
}

void write_log(CTree *node, char *PATH){
    FILE *file = fopen(PATH, "a");
    assert(file != NULL);
    if (node->parent == NULL){
        fprintf(file, "%s 0 %s\n", node->id, node->mode);
        fprintf(file, "%s\n", node->message);
    }
    else{
        fprintf(file, "%s %s %s\n", node->id, node->parent->id, node->mode);
        fprintf(file, "%s\n", node->message);
    }
    fprintf(file, "%s\n", node->timestamp);
    fclose(file);
}


void write_head(char *id, char *PATH){
    FILE *file = fopen(PATH, "w");
    assert(file != NULL);

    fprintf(file, "%s", id);
    fclose(file);
}


CTree *read_head(CTree *node, char *PATH){
    char *id = get_head_id(PATH);
    return search_CTree(node, id);
}

void print_commit_tree(CTree *node, int level) {
    if (node == NULL) {
        return;
    }

    // 打印当前节点的 ID
    for (int i = 0; i < level; i++) {
        ERROR("    ");  // 打印层级缩进
    }
    ERROR("|-- Commit ID: %s\n", node->id);

    // 如果有子树，则递归打印子树
    if (node->subtree != NULL) {
        for (int i = 0; node->subtree[i] != NULL; i++) {
            print_commit_tree(node->subtree[i], level + 1);
        }
    }
}

char *get_head_id(char *PATH){
    FILE *file = fopen(PATH, "r");
    assert(file != NULL);

    static char id[HASH_LEN] = {};
    fscanf(file, "%s", id);
    fclose(file);
    return id;
}