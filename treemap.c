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
    newNode->pair = (Pair*) malloc(sizeof(Pair));
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
                tree->current = newNode;
                break;
            } else {
                temp = temp->left;
            }
        // Hacia la derecha
        } else if (tree->lower_than(temp->pair->key, key)) {
            if (temp->right == NULL){
                temp->right = newNode;
                newNode->parent = temp;
                tree->current = newNode;
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
    TreeNode* temp = x;

    // Si el nodo no tiene hijo izquierdo, se retorna el mismo nodo
    if (temp->left == NULL) return temp;

    // Recorrer hasta encontrar un nodo que no tenga hijo izquierdo
    while (temp->left != NULL){
        temp = temp->left;
    }

    return temp;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    TreeNode* parent = node->parent;

    // Caso Nodo sin hijos
    if (node->left == NULL && node->right == NULL) {
        if (parent != NULL) {
            // Se anula el puntero en el padre del nodo
            if (parent->left == node)
                parent->left = NULL;
            else
                parent->right = NULL;
        } else {
            // Si el nodo era la raiz
            tree->root = NULL;
        }

        free(node->pair);
        free(node);
    }

    // Caso Nodo con 1 hijo
    else if (node->left == NULL || node->right == NULL) {
        TreeNode* child;

        // Determinamos donde se ubica el hijo
        if (node->left != NULL)
            child = node->left;
        else
            child = node->right;

        if (parent != NULL) {
            // Enlazar padre con hijo
            if (parent->left == node)
                parent->left = child;
            else
                parent->right = child;
        } else {
            // El hijo se vuelve la raiz
            tree->root = child;  
        }

        // Actualizar padre del hijo
        child->parent = parent;  
        free(node->pair);
        free(node);
    }

    // Caso Nodo con 2 hijos
    else {
        // Obtenemos sucesor con funcion minimum
        TreeNode* successor = minimum(node->right);

        // Guardamos datos del sucesor
        void* succKey = successor->pair->key;
        void* succValue = successor->pair->value;

        // Eliminamos al sucesor
        removeNode(tree, successor);

        // Reemplazamos datos del nodo eliminado con los del sucesor
        node->pair->key = succKey;
        node->pair->value = succValue;
    }
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


Pair* upperBound(TreeMap* tree, void* key) {
    TreeNode* temp = tree->root;
    TreeNode* ub_node = NULL;

    while (temp != NULL) {
        if (tree->lower_than(key, temp->pair->key)) {
            // key < temp->key → este nodo es posible upper bound
            ub_node = temp;
            temp = temp->left;
        } else if (tree->lower_than(temp->pair->key, key)) {
            // key > temp->key → buscamos a la derecha
            temp = temp->right;
        } else {
            // Clave exacta encontrada
            tree->current = temp;
            return temp->pair;
        }
    }

    // Si no encontramos la clave exacta
    if (ub_node != NULL) {
        tree->current = ub_node;
        return ub_node->pair;
    }

    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode* temp = tree->root;

    // Recorrer los hijos izquierdos hasta el tope y retornar el ultimo
    while (temp->left != NULL){
        temp = temp->left;
    }
    return temp->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode* temp = tree->current;
    if (temp == NULL) return NULL;

    // Si el nodo tiene hijo derecho
    if (temp->right != NULL){
        tree->current = minimum(temp->right);
        return tree->current->pair;
    } else {
        // Si no tiene hijo derecho, buscamos a un padre que sea el siguiente
        while(temp->parent != NULL && temp->parent->right == temp)
            temp = temp->parent;

        tree->current = temp->parent;

        // Si no hay siguiente
        if (tree->current == NULL) return NULL;

        return temp->parent->pair;
    }
}
