#include "BinaryNode.hpp"

//Constructor completo de la estructura
template <typename T>
BinaryNode<T>::BinaryNode (T data, BinaryNode *left, BinaryNode *right, BinaryNode *parent) {
    this->data = data;
    this->left = left;
    this->right = right;
    this->parent = parent;
}

//Constructor parcial de la estructura
template <typename T>
BinaryNode<T>::BinaryNode (T data) {
    this->data = data;
    BinaryNode();
}

//Constructor por defecto
template <typename T>
BinaryNode<T>::BinaryNode () {
    this->left = nullptr;
    this->right = nullptr;
    this->parent = nullptr;
}