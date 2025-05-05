#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {

    // Reservar Memoria
    TreeMap* map = (TreeMap*) malloc(sizeof(TreeMap));
    if (map == NULL) return NULL;

    // Inicializar Variables
    map->lower_than = lower_than;
    map->root = NULL;
    map->current = NULL;

    return map;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    // Reservar memoria e inicializar nodo
    TreeNode* newNode = (TreeNode*) malloc(sizeof(TreeNode));
    newNode->pair->key = key;
    newNode->pair->value = value;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;

    // Si el tree esta vacio
    if (tree->root == NULL){
        tree->root = newNode;
        tree->current = newNode;
        return;
    }

    // El tree tiene elementos
    TreeNode* temp = tree->root;
    while (1){
        // Hacia la izquierda
        if (tree->lower_than(key, temp->pair->key)){
            if (temp->left == NULL){
                temp->left = newNode;
                newNode->parent = temp;
                tree->current = temp;
                break;
            } else {
                temp = temp->left;
            }
        // Hacia la derecha
        } else if (tree->lower_than(temp->pair->key, key)) {
            if (temp->right == NULL){
                temp->right = newNode;
                newNode->parent = temp;
                tree->current = temp;
                break;
            } else {
                temp = temp->right;
            }
        // Claves iguales
        } else {
            return;
        }
    }
}

TreeNode * minimum(TreeNode * x){

    return NULL;
}


void removeNode(TreeMap * tree, TreeNode* node) {

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}


Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode* temp = tree->root;

    // Recorremos el tree comparando los key y cambiando el temp
    while (temp != NULL){
        if (tree->lower_than(key, temp->pair->key))
            temp = temp->left;
        else if (tree->lower_than(temp->pair->key, key))
            temp = temp->right;
        else {
            // Si se encuentra el key buscado
            tree->current = temp;
            return temp->pair;
        }
    }

    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
