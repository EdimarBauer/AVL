/*
* Author: Edimar Jacob Bauer
* Email: edimarjb@gmail.com
* Date: October 1, 2017
*
* Update: February 20, 2018
*/

#include <bits/stdc++.h>

using namespace std;
#define N (1 << 24)

int total, last;

struct Tree{
    int key;
    int balance_factor;
    struct Tree *left, *right;
};

Tree* newNode(int x);
Tree* search(Tree *tree, int x);

void rotate_left(Tree *&tree);
void rotate_right(Tree *&tree);
bool balance(Tree *&tree);
bool insert(Tree *&tree, int x);

bool replace(Tree *&tree, Tree *pivo);
bool remove(Tree *&tree, int x);

void check_is_correct(Tree *tree);
int check(Tree *tree);
void print(Tree *tree, int i);

int main(){

    Tree *tree = NULL;
    int x;

    for (int j = 0; j < 2; j++){
        puts("Inserting...");
        for (int i = 0; i < N; i++){
            x = rand() % N;
            insert(tree, x);
            //check_is_correct(tree);
        }

        puts("Deleting...");
        for (int i = 0; i < N; i++){
            x = rand() % N;
            remove(tree, x);
            //check_is_correct(tree);
        }
    }
    //print(tree, 0);
    //puts("");
}


//#########################################################################################################
//#########################################################################################################



Tree* newNode(int x){

    Tree *tree = (Tree *)malloc(sizeof(Tree));
    if (tree == NULL){
        puts("Err of allocation");
        exit(0);
    }
    tree->key = x;
    tree->balance_factor = 0;
    tree->left = tree->right = NULL;
    total++;

    return tree;
}

Tree* search(Tree *tree, int x){

    while(tree != NULL){
        if (tree->key == x) return tree;

        if (x < tree->key) tree = tree->left;
        else tree = tree->right;
    }
    return NULL;
}





void rotate_left(Tree *&tree){

    Tree *aux = tree->right;
    tree->right = aux->left;
    aux->left = tree;

    if (tree->balance_factor == 2){
        //simple rotate
        if (aux->balance_factor == 2){
            tree->balance_factor = -1;
            aux->balance_factor = 0;
        }else if (aux->balance_factor == 1){
            tree->balance_factor = 0;
            aux->balance_factor = 0;
        }else{
            tree->balance_factor = 1;
            aux->balance_factor = -1;
        }
    }else{
        //double rotate
        if (aux->balance_factor == -1){
            tree->balance_factor = 0;
            aux->balance_factor = -2;
        }else if (aux->balance_factor == 1){
            tree->balance_factor = -1;
            aux->balance_factor = -1;
        }else{
            tree->balance_factor = 0;
            aux->balance_factor = -1;
        }
    }
    tree = aux;
}

void rotate_right(Tree *&tree){

    Tree *aux = tree->left;
    tree->left = aux->right;
    aux->right = tree;

    if (tree->balance_factor == -2){
        //simple rotate
        if (aux->balance_factor == -2){
            tree->balance_factor = 1;
            aux->balance_factor = 0;
        }else if (aux->balance_factor == -1){
            tree->balance_factor = 0;
            aux->balance_factor = 0;
        }else{
            tree->balance_factor = -1;
            aux->balance_factor = 1;
        }
    }else{
        //double rotate
        if (aux->balance_factor == 1){
            tree->balance_factor = 0;
            aux->balance_factor = 2;
        }else if (aux->balance_factor == -1){
            tree->balance_factor = 1;
            aux->balance_factor = 1;
        }else{
            tree->balance_factor = 0;
            aux->balance_factor = 1;
        }
    }
    tree = aux;
}

bool balance(Tree *&tree){

    if (tree->balance_factor == 2){
        if (tree->right->balance_factor < 0)
            rotate_right(tree->right);
        rotate_left(tree);
    }else if (tree->balance_factor == -2){
        if (tree->left->balance_factor > 0)
            rotate_left(tree->left);
        rotate_right(tree);
    }else if (tree->balance_factor != 0)
        return 1;

    return 0;
}

bool insert(Tree *&tree, int x){

    if (tree == NULL){
        tree = newNode(x);
        return 1;
    }

    int b;
    if (x < tree->key) b = -insert(tree->left, x);
    else if (x > tree->key) b = insert(tree->right, x);
    else return 0;

    if (b){
        tree->balance_factor += b;
        return balance(tree);
    }
    return 0;
}





bool replace(Tree *&tree, Tree *pivo){

    int b = 0;
    if (tree->left == NULL){
        pivo->key = tree->key;
        Tree *del = tree;
        tree = tree->right;
        total--;
        free(del);
        return 1;
    }else b = -replace(tree->left, pivo);

    if (b){
        tree->balance_factor -= b;
        balance(tree);
        if (tree->balance_factor == 0) return 1;
    }
    return 0;
}

bool remove(Tree *&tree, int x){

    if (tree == NULL) return 0;

    int b = 0;
    if (tree->key == x){
        if (tree->left && tree->right) b = replace(tree->right, tree);
        else{
            Tree *aux = tree;
            if (tree->left == NULL) tree = tree->right;
            else tree = tree->left;
            total--;
            free(aux);
            return 1;
        }
    }else if (x < tree->key) b = -remove(tree->left, x);
    else b = remove(tree->right, x);

    if (b){
        tree->balance_factor -= b;
        balance(tree);
        if (tree->balance_factor == 0) return 1;
    }
    return 0;
}





void print(Tree *tree, int i){

    if (tree == NULL) return;

    print(tree->left, i+1);
    printf("key %d  balance: %d  alt: %d\n", tree->key, tree->balance_factor, i);
    print(tree->right, i+1);
}

void check_is_correct(Tree *tree){

    last = -1;
    int to = total;
    check(tree);
    if (total != 0){
        printf("Diference of %d elements\n", total);
        exit(0);
    }
    total = to;
}

int check(Tree *tree){

    if (tree == NULL) return 0;

    int l, r;
    l = check(tree->left);
    total--;
    if (last >= tree->key){
        printf("Err between nodes %d and %d\n", last, tree->key);
        exit(0);
    }
    last = tree->key;
    r = check(tree->right);

    if (abs(l-r) > 1){
        printf("Err of balance in parent %d\n", tree->key);
        exit(0);
    }

    return max(l, r) + 1;
}

