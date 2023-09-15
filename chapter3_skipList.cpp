#include<stdio.h>
#include<stdlib.h>
#define MAXSKIPLEVEL 5
struct ListNode
{
    int data;
    struct ListNode *next[1];
};
struct SkipList{
    struct ListNode *header;
    int listLevel;      // current level of list
};
struct SkipList list;
struct ListNode *insertElement(int data){
    int i, newLevel;
    struct ListNode *update[MAXSKIPLEVEL + 1];
    struct ListNode *temp;
    temp = list.header;
    for (i = list.listLevel; i >=0; i--){
        while (temp->next[i] != list.header && temp->next[i]->data < data)
            temp = temp->next[i];
        update[i] = temp;
    }
    temp = temp->next[0];
    if (temp != list.header && temp->data == data)
        return temp;
    // determine level
    for (newLevel = 0; rand() < RAND_MAX/2 && newLevel < MAXSKIPLEVEL; newLevel++);
    if (newLevel > list.listLevel){
        for (i = list.listLevel + 1; i <= newLevel; i++)
            update[i] = list.header;
        list.listLevel = newLevel;
    }
    // make new node
    if ((temp = (struct ListNode*) malloc(sizeof(ListNode) + newLevel*sizeof(ListNode*))) == 0){
        printf("insufficient memory (insertElement)\n");
        exit(1);
    }
    temp->data = data;
    for (i = 0; i <= newLevel; i++){    // update next links
        temp->next[i] = update[i]->next[i];
        update[i]->next[i] = temp;
    }
    return temp;
}
// delete node containing data
void deleteElement(int data){
    int i;
    struct ListNode *update[MAXSKIPLEVEL + 1], *temp;
    temp = list.header;
    for (i = list.listLevel; i >= 0; i--){
        while (temp->next[i] != list.header && temp->next[i]->data < data)
            temp = temp->next[i];
        update[i] = temp;
    }
    temp = temp->next[0];
    if (temp == list.header || !(temp->data == data))
        return;
    //adjust new pointers
    for (i = 0; i < list.listLevel; i++){
        if (update[i]->next[i] != temp)
            break;
        update[i]->next[i] = temp->next[i];
    }
    free(temp);
    // adjust header level
    while ((list.listLevel > 0) && (list.header->next[list.listLevel] == list.header))
        list.listLevel--;
}
// find node containing data
struct ListNode *findElement(int data){
    int i;
    struct ListNode *temp = list.header;
    for (i = list.listLevel; i >= 0; i--)
    {
        while (temp->next[i] != list.header && temp->next[i]->data < data)
            temp = temp->next[i];
    }
    temp = temp->next[0];
    if (temp != list.header && temp->data == data)
        return temp;
    return 0;
}
// initialize skip list
void initList(){
    int i;
    if ((list.header = (struct ListNode*) malloc(sizeof(struct ListNode) + MAXSKIPLEVEL*sizeof(struct ListNode*))) == 0){
        printf("Memory Error!\n");
        exit(1);
    }
    for (i = 0; i <= MAXSKIPLEVEL; i++)
        list.header->next[i] = list.header;
    list.listLevel = 0;
}
int main(int argc, char **argv){
    int i, *a, maxnum = atoi(argv[1]);
    initList();
    if ((a = (int*) malloc(maxnum*sizeof(*a))) == 0){
        fprintf(stderr, "insufficient memory (a)\n");
        exit(1);
    }
    for (i = 0; i < maxnum; i++)
        printf("Random, %d items\n", maxnum);
    for (i = 0; i < maxnum; i++)
        insertElement(a[i]);
    for (i = maxnum - 1; i >= 0; i--){
        findElement(a[i]);
    }
    for (i = maxnum - 1; i >= 0; i--)
        deleteElement(a[i]);
    return 0;
}