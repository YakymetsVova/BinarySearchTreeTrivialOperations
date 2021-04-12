#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 50
typedef struct {
    char* prod_name;
    int index;
}Data;

typedef struct Node {
    Data data;
    Node* left;
    Node* right;
}Node;

Node* newNode(Data);
Node* recursiveInsert(Node*, Data);
Node* iterativeInsert(Node*, Data);
void drawTree(Node*, int, int);
int subtreeNodesCnt(Node*);
Node* deleteNode(Node*, Data);
int getHeight(Node*);
void freeNode(Node*);
void freeTree(Node*);
int getData(Data*);
FILE* fp;

int main()
{
    char fname[] = "InputData2.txt";
    if ((fp = fopen(fname, "r")) == NULL) {
        printf("\n\t Файл %s не знайлено\n", fname);
        return 0;
    }
    Node* root = NULL;
    system("chcp 1251");
    Data tmp;
    getData(&tmp);
    root = iterativeInsert(root, tmp);
    while (getData(&tmp)) {
        iterativeInsert(root, tmp);
    }

    printf("\nTree looks like:\n\n");        
    drawTree(root, 0, 1);
    printf("\n");
    drawTree(root, 0, 0);
    int rightNodes_cnt = subtreeNodesCnt(root->right);
    int leftNodes_cnt = subtreeNodesCnt(root->left);


    int rightGreater;
    rightNodes_cnt >= leftNodes_cnt ? rightGreater = 1 : rightGreater = 0;

    int right_height = getHeight(root->right);
    int left_height = getHeight(root->left);

    printf("\nRight subtree has %d nodes. Left subtree has %d nodes"
        "\nHeight of the right tree: %d, left tree: %d"
        , rightNodes_cnt, leftNodes_cnt, right_height, left_height);

    Data tmp2;
    rightGreater ? tmp2 = root->right->data : tmp2 = root->left->data;
    root = deleteNode(root, tmp2);

    printf("\n\nAfter deletion tree looks like:\n");
    drawTree(root, 0, 1);
    printf("\n");
    drawTree(root, 0, 0);

    int height_cnt;
    if (rightGreater) {
        height_cnt = getHeight(root->right);
        if (height_cnt != right_height)
            printf("\n Height of the right tree was changed, now it's: %d, but was %d", height_cnt, right_height);
        rightNodes_cnt = subtreeNodesCnt(root->right);
    }
    else {
        height_cnt = getHeight(root->left);
        if (height_cnt != left_height)
            printf("\n Height of the right tree was changed, now it's: %d, but was %d", height_cnt, left_height);
        leftNodes_cnt = subtreeNodesCnt(root->left);
    }
    freeTree(root);
}

Node* newNode(Data data)
{
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->data = data;
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}

Node* recursiveInsert(Node* root, Data data)
{
    if (root == NULL)
        return newNode(data);
    if (data.index < root->data.index)
        root->left = recursiveInsert(root->left, data);
    else if (data.index > root->data.index)
        root->right = recursiveInsert(root->right, data);
    return root;
}


Node* iterativeInsert(Node* root, Data data)
{
    Node* temp = newNode(data);
    Node* current = root;
    Node* previous = NULL;
    while (current != NULL) {
        previous = current;
        if (data.index < current->data.index)
            current = current->left;
        else
            current = current->right;
    }
    if (previous == NULL)
        previous = temp;
    else if (data.index < previous->data.index)
        previous->left = temp;
    else
        previous->right = temp;
    return previous;
}

void drawTree(Node* root, int lev, int likeTree)
{
    if (root == NULL)return;
    drawTree(root->right, lev+1, likeTree);
    if (likeTree)
        printf("%*c%s\n", lev * 18, ' ', root->data.prod_name);
    else
        printf("%d, %s\n", root->data.index, root->data.prod_name);
    drawTree(root->left, lev+1, likeTree);
}

int subtreeNodesCnt(Node* subtree_root)
{
    if (subtree_root == NULL)
        return 0;
    return 1 + subtreeNodesCnt(subtree_root->left) + subtreeNodesCnt(subtree_root->right);
}

Node* deleteNode(Node* root, Data data)
{
    Node* proot = root;
    if (root == NULL)
        return root;
    if (data.index < root->data.index)
        root->left = deleteNode(root->left, data);
    else if (data.index > root->data.index)
        root->right = deleteNode(root->right, data);
    else {
        if (root->left == NULL && root->right == NULL)
            root = NULL;
        else if (root->left == NULL)
            root = root->right;
        else if (root->right == NULL)
            root = root->left;
        else {
            Node* right = root->right;
            root = root->left;
            Node* tmp = root;
            while (tmp->right != NULL)
                tmp = tmp->right;
            tmp->right = right;
        }
        freeNode(proot);
    }  
    return root;
}

int getHeight(Node* root)
{
    if (!root)
        return 0;
    else {
        int left_height = getHeight(root->left);
        int right_height = getHeight(root->right);
        return (left_height >= right_height) ? left_height + 1 : right_height + 1;
    }
}

void freeNode(Node* node)
{
    free(node->data.prod_name);
    free(node);
}

void freeTree(Node* root)
{
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    freeNode(root);
}

int getData(Data* data)
{
    char buf[MAX];
    char gnamebf[MAX];
    
   if (fgets(buf, MAX, fp) != NULL) {
        if (sscanf(buf, "%[^,], %d", gnamebf, &data->index)==2) {
            data->prod_name = (char*)malloc(strlen(gnamebf) + 1);
            strcpy(data->prod_name, gnamebf);
            printf("Товар -> %-20s | індекс -> %d\n", data->prod_name, data->index);
            return 1;
        }
    } 
   return 0;   
}